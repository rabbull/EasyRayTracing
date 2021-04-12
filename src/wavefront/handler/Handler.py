import numpy as np

from .Context import Context


class Handler(object):
    @staticmethod
    def data_type() -> np.dtype:
        return np.float32

    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: Context, line: str) -> None:
        raise NotImplementedError
