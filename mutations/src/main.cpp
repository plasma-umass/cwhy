#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Lex/Lexer.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

size_t getRandom(size_t max) {
    std::random_device generator;
    return std::uniform_int_distribution<size_t>(0, max - 1)(generator);
}

template <typename Iterator>
Iterator getRandom(Iterator begin, Iterator end) {
    std::advance(begin, getRandom(std::distance(begin, end)));
    return begin;
}

std::vector<const clang::FunctionDecl*> getAllFunctionDeclarations(clang::ASTContext& context) {
    using namespace clang::ast_matchers;
    const auto matcher = functionDecl(isExpansionInMainFile(), unless(isImplicit())).bind("root");
    const auto matches = match(matcher, context);

    std::vector<const clang::FunctionDecl*> declarations;

    for (const auto& match : matches) {
        const auto* function = match.getNodeAs<clang::FunctionDecl>("root");
        declarations.push_back(function);
    }

    return declarations;
}

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
        std::cerr << "Error: " << e.what() << std::endl << std::endl;
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

    const auto candidates = getAllFunctionDeclarations(ASTs[0]->getASTContext());

    for (const auto& function : candidates) {
        if (function->getNumParams() < 2) {
            continue;
        }

        std::cout << function->getNameAsString() << std::endl;
        for (size_t i = 0; i < function->getNumParams(); ++i) {
            std::cout << function->getParamDecl(i)->getType().getAsString() << " "
                      << function->getParamDecl(i)->getNameAsString() << std::endl;
        }
    }
}
