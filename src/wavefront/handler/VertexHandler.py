from .Context import Context
from .FixedLengthVectorHandler import FixedLengthVectorHandler


class VertexHandler(FixedLengthVectorHandler):
    @staticmethod
    def length() -> int:
        return 3

    @staticmethod
    def dump(context: Context, vector):
        context.vertices.append(vector)

    @staticmethod
    def key():
        return "v"
