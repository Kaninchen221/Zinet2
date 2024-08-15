from zinet_generator import safe_dict as sd


class TestSafeDict:
    def test_inheritance(self):
        assert issubclass(sd.SafeDict, dict)

    def test_missing(self):
        key = "key"
        result = self.safeDict.__missing__(key);
        assert result == "{key}"

    safeDict = sd.SafeDict()
