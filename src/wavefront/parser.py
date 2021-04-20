import sys
from typing import *

import numpy as np

from .handler import Handler, Context


def _cat(arrays: Iterable[np.ndarray]) -> Optional[np.ndarray]:
    arrays = tuple(arrays)
    if len(arrays) == 0:
        return None
    return np.stack(arrays)


class Parser(object):
    _handler_dict: Dict[str, Handler] = {}

    def __init__(self):
        self._context = Context()
        self.register_all_subclasses(Handler)

    def register_all_subclasses(self, sup_cls: Type[Handler]):
        for cls in sup_cls.__subclasses__():
            if len(cls.__subclasses__()) != 0:
                self.register_all_subclasses(cls)
            else:
                self._handler_dict[cls.key()] = cls()

    def parse_text(self, content: str):
        for line in content.splitlines():
            self._handle(line)

    def parse(self, path: str):
        file = open(path, "r")
        for line in file.readlines():
            self._handle(line)
        file.close()

    def _handle(self, line: str):
        if len(line.strip()) == 0 or line[0] == '#':
            return
        key = line.split(maxsplit=2)[0]
        if key in self._handler_dict.keys():
            self._handler_dict.get(key)(self._context, line)
        else:
            print("Unknown Tag in Line:", line, file=sys.stderr)

    def dump(self):
        raw = {
            "v": _cat(self._context.vertices),
            "vt": _cat(self._context.texture_coordinates),
            "vn": _cat(self._context.vertex_normals),
            "f": _cat(self._context.faces),
        }
        indices = raw["f"][:, :, 0] - 1
        faces = raw["v"][indices]
        normals = np.cross(faces[:, 2] - faces[:, 0], faces[:, 1] - faces[:, 0])
        normals /= np.linalg.norm(normals, axis=1)[:, np.newaxis]
        return {
            "raw": raw,
            "aggregated": {
                "faces": faces,
                "normals": normals,
            }
        }
