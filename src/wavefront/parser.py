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
    vertex_normals: List[np.ndarray] = []
    faces: List[np.ndarray] = []

    def __str__(self):
        return "vertices: %s\n" % self.vertices \
               + "texture_coordinates: %s\n" % self.texture_coordinates \
               + "vertex_normals: %s\n" % self.vertex_normals \
               + "faces: %s\n" % self.faces


class _Handler(object):
    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: _Context, line: str) -> None:
        raise NotImplementedError


class Parser(object):
    _handler_dict: Dict[str, _Handler] = {}

    def __init__(self):
        self._context = _Context()
        self.register_all_subclasses(_Handler)

    def register_all_subclasses(self, sup_cls: Type[_Handler]):
        for cls in sup_cls.__subclasses__():
            if len(cls.__subclasses__()) != 0:
                self.register_all_subclasses(cls)
            else:
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
            "vt": _cat(self._context.texture_coordinates),
            "vn": _cat(self._context.vertex_normals),
            "f": _cat(self._context.faces),
        }


class _FixedLengthVectorHandler(_Handler):
    @staticmethod
    def length() -> int:
        raise NotImplementedError

    @staticmethod
    def dump(context: _Context, vector):
        raise NotImplementedError

    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: _Context, line: str) -> None:
        splits = line.split()
        assert splits[0] == self.key()
        assert len(splits) == self.length() + 1
        vector = np.zeros(self.length(), dtype=_DATA_TYPE)
        for i in range(self.length()):
            vector[i] = float(splits[i + 1])
        self.dump(context, vector)


class _VertexHandler(_FixedLengthVectorHandler):
    @staticmethod
    def length() -> int:
        return 3

    @staticmethod
    def dump(context: _Context, vector):
        context.vertices.append(vector)

    @staticmethod
    def key():
        return "v"


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


class _VertexNormalHandler(_FixedLengthVectorHandler):
    @staticmethod
    def length() -> int:
        return 3

    @staticmethod
    def dump(context: _Context, vector):
        context.vertex_normals.append(vector)

    @staticmethod
    def key():
        return "vn"


class _FaceHandler(_Handler):
    @staticmethod
    def key():
        return "f"

    def __call__(self, context: _Context, line: str) -> None:
        splits = line.split()
        assert splits[0] == self.key()
        vertex_count = len(splits[1:])

        # TODO: impl. of case of variable number of vertices
        if vertex_count != 3:
            raise NotImplementedError

        face = np.zeros((vertex_count, 3), dtype=np.uint32)
        for i in range(vertex_count):
            split = splits[i + 1]
            fields = split.split('/')
            if len(fields) == 1:
                indices = [0]
            elif len(fields) == 2:
                indices = [0, 1]
            elif len(fields) == 3:
                if fields[1] == "":
                    indices = [0, 2]
                else:
                    indices = [0, 1, 2]
            else:
                raise ValueError
            for idx in indices:
                field = int(fields[idx])
                assert field > 0
                face[i, idx] = int(fields[idx])

        context.faces.append(face)
