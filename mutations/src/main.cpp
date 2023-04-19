#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

#include <boost/program_options.hpp>

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Lex/Lexer.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

std::vector<const clang::FunctionDecl*> getAllFunctionsInFile(clang::ASTContext& context) {
    using namespace clang::ast_matchers;
    const auto matcher = functionDecl(isExpansionInMainFile(), unless(isImplicit())).bind("root");
    const auto matches = match(matcher, context);

    std::vector<const clang::FunctionDecl*> declarations;
    std::transform(matches.begin(), matches.end(), std::back_inserter(declarations),
                   [](const auto& match) { return match.template getNodeAs<clang::FunctionDecl>("root"); });
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

    const auto functions = getAllFunctionsInFile(ASTs[0]->getASTContext());
    for (const auto& function : functions) {
        std::cout << function->getNameAsString() << std::endl;
    }
}
