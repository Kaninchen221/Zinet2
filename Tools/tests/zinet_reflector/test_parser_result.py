from zinet_reflector.parser_result import *


class TestParserResult:

    parser_result = ParserResult()

    def test_namespace(self):
        assert hasattr(self.parser_result, "cursor")
        assert hasattr(self.parser_result, "reflection_kind")
        assert hasattr(self.parser_result, "reflection_cursor")
        assert hasattr(self.parser_result, "children")
        assert hasattr(self.parser_result, "tokens")

