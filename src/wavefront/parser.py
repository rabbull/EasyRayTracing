import sys
from typing import *

import numpy as np

_DATA_TYPE = np.float32


def _cat(arrays: Iterable[np.ndarray]) -> Optional[np.ndarray]:
    arrays = tuple(arrays)
    if len(arrays) == 0:
        return None
    return np.stack(arrays)


class _Context(object):
    vertices: List[np.ndarray] = []
    texture_coordinates: List[np.ndarray] = []

    def __str__(self):
        return "vertices: %s\n" % self.vertices \
               + "texture_coordinates: %s\n" % self.texture_coordinates


class _Handler(object):
    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: _Context, line: str):
        raise NotImplementedError


class Parser(object):
    _handler_dict: Dict[str, _Handler] = None

    def __init__(self):
        self._handler_dict = {}
        self._context = _Context()
        for cls in _Handler.__subclasses__():
            self._handler_dict[cls.key()] = cls()

    def parse_text(self, content: str):
        for line in content.splitlines():
            self.handle(line)

    def parse(self, path: str):
        file = open(path, "r")
        for line in file.readlines():
            self.handle(line)
        file.close()

    def handle(self, line: str):
        key = line.split(maxsplit=2)[0]
        if key in self._handler_dict.keys():
            self._handler_dict.get(key)(self._context, line)
        else:
            print("Unknown Tag in Line:", line, file=sys.stderr)

    def dump(self):
        return {
            "v": _cat(self._context.vertices),
            "vt": _cat(self._context.texture_coordinates)
        }


class _VertexHandler(_Handler):
    @staticmethod
    def key():
        return "v"

    def __call__(self, context: _Context, line: str):
        splits = line.split()
        assert splits[0] == self.key()
        vertex = np.zeros(3, dtype=_DATA_TYPE)
        for i in range(3):
            vertex[i] = float(splits[i + 1])
        context.vertices.append(vertex)


class _VertexTextureHandler(_Handler):
    @staticmethod
    def key():
        return "vt"

    def __call__(self, context: _Context, line: str):
        splits = line.split()
        assert splits[0] == self.key()
        assert 3 <= len(splits) <= 4
        coordinate = np.zeros(3, dtype=_DATA_TYPE)
        for idx, val in enumerate(splits[1:]):
            coordinate[idx] = float(val)
        context.texture_coordinates.append(coordinate)


if __name__ == '__main__':
    parser = Parser()
    text = "v 0 0 1\nvt 1 0 0\ng cube"
    parser.parse("/home/karl/Projects/EasyRayTracing/test/scenes/cube/cube.obj")
    print(parser.dump())
