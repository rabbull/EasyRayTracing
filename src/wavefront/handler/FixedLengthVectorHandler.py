import numpy as np

from .Context import Context
from .Handler import Handler
from .common import _DATA_TYPE


class FixedLengthVectorHandler(Handler):
    @staticmethod
    def length() -> int:
        raise NotImplementedError

    @staticmethod
    def dump(context: Context, vector):
        raise NotImplementedError

    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: Context, line: str) -> None:
        splits = line.split()
        assert splits[0] == self.key()
        assert len(splits) == self.length() + 1
        vector = np.zeros(self.length(), dtype=_DATA_TYPE)
        for i in range(self.length()):
            vector[i] = float(splits[i + 1])
        self.dump(context, vector)
