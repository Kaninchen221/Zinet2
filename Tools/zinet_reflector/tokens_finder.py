from zinet_reflector.parser_result import *


class TokensFinder:
    def __init__(self):
        pass

    def find_tokens(self, parser_result):
        self._find_tokens_internal(parser_result)

    def _find_tokens_internal(self, parent):
        for child in parent.children:
            if (reflection_cursor := child.reflection_cursor) is not None:
                raw_tokens = list(reflection_cursor.get_tokens())

                # Remove reflection macro and '('
                raw_tokens = raw_tokens[2:]
                # Remove ')'
                raw_tokens = raw_tokens[:-1]

                # Remove all ','
                tokens = []
                for raw_token in raw_tokens:
                    if raw_token.spelling != ',':
                        tokens.append(raw_token)

                for token in tokens:
                    child.tokens.append(token.spelling)

            self._find_tokens_internal(child)
