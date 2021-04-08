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