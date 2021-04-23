from .context import ObjParserContext
from .FixedLengthVectorHandler import FixedLengthVectorHandler


class VertexNormalHandler(FixedLengthVectorHandler):
    @staticmethod
    def length() -> int:
        return 3

    @staticmethod
    def dump(context: ObjParserContext, vector):
        context.vertex_normals.append(vector)

    @staticmethod
    def key():
        return "vn"
