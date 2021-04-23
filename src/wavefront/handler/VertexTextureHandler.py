import numpy as np

from .context import ObjParserContext
from .Handler import Handler


class VertexTextureHandler(Handler):
    @staticmethod
    def key():
        return "vt"

    def __call__(self, context: ObjParserContext, line: str):
        splits = line.split()
        assert splits[0] == self.key()
        assert 3 <= len(splits) <= 4
        coordinate = np.zeros(3, dtype=self.data_type())
        for idx, val in enumerate(splits[1:]):
            coordinate[idx] = float(val)
        context.texture_coordinates.append(coordinate)
