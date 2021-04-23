import numpy as np

from .context import ObjParserContext
from .Handler import Handler


class FixedLengthVectorHandler(Handler):
    @staticmethod
    def length() -> int:
        raise NotImplementedError

    @staticmethod
    def dump(context: ObjParserContext, vector):
        raise NotImplementedError

    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: ObjParserContext, line: str) -> None:
        splits = line.split()
        assert splits[0] == self.key()
        assert len(splits) == self.length() + 1
        vector = np.zeros(self.length(), dtype=self.data_type())
        for i in range(self.length()):
            vector[i] = float(splits[i + 1])
        self.dump(context, vector)
