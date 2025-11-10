import os
import unittest

from cwhy import clangd_lsp_integration

CPP_TEST_ROOT = os.path.join(os.path.dirname(__file__), "..", "tests", "c++")

if __name__ == "__main__":
    unittest.main()


class TestSymbols(unittest.TestCase):
    def test_no_symbol_found(self):
        self.assertTrue(clangd_lsp_integration.is_available())
        result = clangd_lsp_integration.source_for_symbol(
            os.path.join(CPP_TEST_ROOT, "missing-hash.cpp"), "random_symbol_name"
        )
        self.assertEqual(
            result,
            "Symbol 'random_symbol_name' is not replaceable.\n"
            "Replaceable symbols for 'tests/c++/missing-hash.cpp':\n"
            " -  Node (Class)\n"
            " -  bfs (Function)",
        )

    def test_list_symbols(self):
        self.assertTrue(clangd_lsp_integration.is_available())
        result = clangd_lsp_integration.document_symbols(
            os.path.join(CPP_TEST_ROOT, "missing-hash.cpp")
        )
        self.assertEqual(
            result,
            "Replaceable symbols for 'tests/c++/missing-hash.cpp':\n"
            " -  Node (Class)\n"
            " -  bfs (Function)",
        )

    def test_get_class(self):
        self.assertTrue(clangd_lsp_integration.is_available())
        result = clangd_lsp_integration.source_for_symbol(
            os.path.join(CPP_TEST_ROOT, "missing-hash.cpp"), "Node"
        )
        self.assertEqual(
            result,
            "File 'tests/c++/missing-hash.cpp':\n"
            "```\n"
            "37 struct Node {\n"
            "38     const std::pair<int, int> position;\n"
            "39     std::vector<Node*> neighbors;\n"
            "40 };\n"
            "```",
        )
