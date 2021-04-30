from typing import *

from .context import Context


class Handler:
    @staticmethod
    def key() -> str:
        raise NotImplementedError

    def __call__(self, context: Context, line: str, *args, **kwargs):
        raise NotImplementedError


class FixedLengthVectorHandler(Handler):
    @staticmethod
    def key() -> str:
        raise NotImplementedError

    @staticmethod
    def _num_prefixes() -> int:
        raise NotImplementedError

    @staticmethod
    def _num_suffixes() -> int:
        raise NotImplementedError

    @staticmethod
    def _vector_size() -> int:
        raise NotImplementedError

    @staticmethod
    def _dump(context: Context, vector: List[float]):
        raise NotImplementedError

    def __call__(self, context: Context, line: str, *args, **kwargs):
        splits = line.split()
        num_splits = \
            self._vector_size() + self._num_prefixes() + self._num_suffixes()
        assert len(splits) == num_splits
        vector = [
            float(split) for split in
            splits[self._num_prefixes():num_splits - self._num_suffixes()]
        ]
        self._dump(context, vector)
