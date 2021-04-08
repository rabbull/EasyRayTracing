from .Context import Context
from .FixedLengthVectorHandler import FixedLengthVectorHandler


class VertexNormalHandler(FixedLengthVectorHandler):
    @staticmethod
    def length() -> int:
        return 3

    @staticmethod
    def dump(context: Context, vector):
        context.vertex_normals.append(vector)

    @staticmethod
    def key():
        return "vn"
