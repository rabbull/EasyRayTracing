from typing import *

import numpy as np

from .FixedLengthVectorObjHandler import FixedLengthVectorObjHandler
from ..context import ObjParserContext


class VertexNormalHandler(FixedLengthVectorObjHandler):
    @staticmethod
    def _vector_size() -> int:
        return 3

    @staticmethod
    def _dump(context: ObjParserContext, vector: List[float]):
        context.vertex_normals.append(
            np.array(vector, dtype=VertexNormalHandler.data_type())
        )

    @staticmethod
    def key():
        return "vn"
