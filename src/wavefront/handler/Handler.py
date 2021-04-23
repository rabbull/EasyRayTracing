import numpy as np

from .context import ObjParserContext


class Handler(object):
    @staticmethod
    def data_type() -> np.dtype:
        return np.float64

    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: ObjParserContext, line: str) -> None:
        raise NotImplementedError
