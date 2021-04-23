import logging
from typing import *

import logging

from .handler import MtlParserContext, MtlHandler

class WaveFrontMtlParser(object):
    _handler_dict: Dict[str, MtlHandler] = {}

    def __init__(self):
        self._context = MtlParserContext()
        self.register_all_subclasses(MtlHandler)

    def register_all_subclasses(self, sup_cls: Type[MtlHandler]):
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
        key, content = line.split(maxsplit=2)
        if key in self._handler_dict.keys():
            self._handler_dict.get(key)(self._context, line)
        else:
            logging.warning("Unknown Tag in Line:", line)

    def dump(self):
        raise NotImplementedError
