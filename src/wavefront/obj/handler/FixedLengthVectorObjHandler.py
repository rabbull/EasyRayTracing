from typing import List

from .ObjHandler import ObjHandler
from ..context import ObjParserContext
from ...common.handler import FixedLengthVectorHandler


class FixedLengthVectorObjHandler(FixedLengthVectorHandler, ObjHandler):
    @staticmethod
    def key() -> str:
        raise NotImplementedError

    @staticmethod
    def _num_prefixes() -> int:
        return 1

    @staticmethod
    def _num_suffixes() -> int:
        return 0

    @staticmethod
    def _vector_size() -> int:
        raise NotImplementedError

    @staticmethod
    def _dump(context: ObjParserContext, vector: List[float]) -> None:
        raise NotImplementedError
