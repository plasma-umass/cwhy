#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Lex/Lexer.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <range/v3/all.hpp>

namespace {
std::size_t getRandom(std::size_t max) {
    std::random_device generator;
    return std::uniform_int_distribution<std::size_t>(0, max - 1)(generator);
}

template <typename Iterator>
Iterator getRandom(Iterator begin, Iterator end) {
    std::advance(begin, getRandom(std::distance(begin, end)));
    return begin;
}

[[maybe_unused]] std::vector<const clang::FunctionDecl*> getAllFunctionDeclarations(clang::ASTContext& context) {
    using namespace clang::ast_matchers;
    const auto matcher = functionDecl(isExpansionInMainFile(), isDefinition(), unless(isImplicit())).bind("root");
    const auto matches = match(matcher, context);

    std::vector<const clang::FunctionDecl*> declarations;
    for (const auto& match : matches) {
        declarations.push_back(match.getNodeAs<clang::FunctionDecl>("root"));
    }

    return declarations;
}

[[maybe_unused]] std::vector<const clang::CallExpr*> getAllFunctionCallExpressions(clang::ASTContext& context) {
    using namespace clang::ast_matchers;
    const auto matcher = callExpr(isExpansionInMainFile(), unless(cxxOperatorCallExpr())).bind("root");
    const auto matches = match(matcher, context);

    std::vector<const clang::CallExpr*> declarations;
    for (const auto& match : matches) {
        declarations.push_back(match.getNodeAs<clang::CallExpr>("root"));
    }

    return declarations;
}

std::optional<clang::tooling::Replacements> flipSourceRanges(const clang::SourceRange& a, const clang::SourceRange& b,
                                                             const clang::SourceManager& sm,
                                                             const clang::LangOptions& lo) {
    const auto firstSourceRange = clang::CharSourceRange::getTokenRange(a);
    const auto secondSourceRange = clang::CharSourceRange::getTokenRange(b);

    const auto firstSourceText = clang::Lexer::getSourceText(firstSourceRange, sm, lo);
    const auto secondSourceText = clang::Lexer::getSourceText(secondSourceRange, sm, lo);

    clang::tooling::Replacements replacements;
    if (auto error = replacements.add(clang::tooling::Replacement(sm, firstSourceRange, secondSourceText, lo))) {
        return std::nullopt;
    }

    if (auto error = replacements.add(clang::tooling::Replacement(sm, secondSourceRange, firstSourceText, lo))) {
        return std::nullopt;
    }

    return replacements;
}

std::optional<clang::tooling::Replacements> flipFunctionParameters(const clang::FunctionDecl& f,
                                                                   clang::ASTContext& context) {
    if (f.getNumParams() < 2) {
        return std::nullopt;
    }

    const auto first = getRandom(f.getNumParams());
    const auto options = ranges::views::iota(0u, f.getNumParams()) | ranges::views::filter([&](auto index) {
                             return f.getParamDecl(index)->getType() != f.getParamDecl(first)->getType();
                         })
                         | ranges::to<std::vector>();

    if (options.empty()) {
        return std::nullopt;
    }

    const auto second = *getRandom(options.begin(), options.end());

    return flipSourceRanges(f.getParamDecl(first)->getSourceRange(), f.getParamDecl(second)->getSourceRange(),
                            context.getSourceManager(), context.getLangOpts());
}

std::optional<clang::tooling::Replacements> flipFunctionCallArguments(const clang::CallExpr& e,
                                                                      clang::ASTContext& context) {
    if (e.getNumArgs() < 2) {
        return std::nullopt;
    }

    const auto nonDefaultArguments = ranges::views::iota(0u, e.getNumArgs()) | ranges::views::filter([&](auto index) {
                                         using namespace clang::ast_matchers;
                                         const auto matches = match(cxxDefaultArgExpr(), *e.getArg(index), context);
                                         return matches.empty();
                                     })
                                     | ranges::to<std::vector>();

    const auto first = *getRandom(nonDefaultArguments.begin(), nonDefaultArguments.end());
    const auto options
        = nonDefaultArguments
          | ranges::views::filter([&](auto index) { return e.getArg(index)->getType() != e.getArg(first)->getType(); })
          | ranges::to<std::vector>();

    if (options.empty()) {
        return std::nullopt;
    }

    const auto second = *getRandom(options.begin(), options.end());

    return flipSourceRanges(e.getArg(first)->getSourceRange(), e.getArg(second)->getSourceRange(),
                            context.getSourceManager(), context.getLangOpts());
}
} // namespace

int main(int argc, char** argv) {
    namespace po = boost::program_options;

    po::options_description required("Required options");
    po::options_description optional("Optional options");

    // clang-format off
    required.add_options()
        ("compile-commands,p", po::value<std::string>()->required(), "path to compile_commands.json")
        ("filename", po::value<std::string>()->required(), "path to the file to mutate");

    optional.add_options()
        ("help", "produce help message");
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, po::options_description().add(required).add(optional)), vm);

    if (vm.count("help")) {
        std::cout << required << std::endl;
        std::cout << optional << std::endl;
        return 1;
    }

    try {
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << std::endl;
        std::cerr << required << std::endl;
        return 1;
    }

    std::string error;
    const auto database = clang::tooling::JSONCompilationDatabase::loadFromFile(
        vm.at("compile-commands").as<std::string>(), error, clang::tooling::JSONCommandLineSyntax::AutoDetect);
    if (!database) {
        std::cerr << "Could not load compilation database." << std::endl << error << std::endl;
        return 1;
    }

    clang::tooling::ClangTool tool(*database, {vm.at("filename").as<std::string>()});
    std::vector<std::unique_ptr<clang::ASTUnit>> ASTs;
    if (tool.buildASTs(ASTs) != 0) {
        std::cerr << "Failed to build the AST." << std::endl;
        return 1;
    }

    assert(ASTs.size() == 1);
    auto& sm = ASTs[0]->getSourceManager();
    const auto& lo = ASTs[0]->getLangOpts();

    // auto candidates = getAllFunctionDeclarations(ASTs[0]->getASTContext());
    auto candidates = getAllFunctionCallExpressions(ASTs[0]->getASTContext());
    std::shuffle(candidates.begin(), candidates.end(), std::random_device());

    for (const auto& function : candidates) {
        // const auto replacements = flipFunctionParameters(*function, sm, lo);
        const auto replacements = flipFunctionCallArguments(*function, ASTs[0]->getASTContext());
        if (!replacements) {
            continue;
        }

        clang::Rewriter rewriter(sm, lo);
        if (!clang::tooling::applyAllReplacements(*replacements, rewriter)) {
            std::cerr << "Error: could not apply replacements." << std::endl;
            return 1;
        }

        rewriter.overwriteChangedFiles();

        return 0;
    }

    std::cout << "Could not find any suitable candidates." << std::endl;
    return 1;
}
