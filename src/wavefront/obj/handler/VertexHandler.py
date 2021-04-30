from typing import *

import numpy as np

from .FixedLengthVectorObjHandler import FixedLengthVectorObjHandler
from ..context import ObjParserContext


class VertexHandler(FixedLengthVectorObjHandler):
    @staticmethod
    def key() -> str:
        return "v"

    @staticmethod
    def _vector_size() -> int:
        return 3

    @staticmethod
    def _dump(context: ObjParserContext, vector: List[float]) -> None:
        context.vertices.append(
            np.array(vector, dtype=VertexHandler.data_type())
        )
