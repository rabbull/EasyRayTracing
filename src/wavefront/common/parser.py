import logging
from typing import *

from .context import Context
from .handler import Handler


class BaseLinedTextFileParser:
    _context: Context = None
    _handler_dict: Dict[str, Handler] = None

    def __init__(self, *args, **kwargs):
        self._handler_dict = {}
        context_klass = self._context_base_class()
        self._context = context_klass(*args, **kwargs)
        self.register_all_subclasses(self._handler_base_class())

    def register_all_subclasses(self, base_class: Type[Handler]):
        for klass in base_class.__subclasses__():
            logging.info(f"Handler found: {klass}")
            if len(klass.__subclasses__()) != 0:
                logging.info(f"Subclass(es) detected, ignoring: {klass}")
                self.register_all_subclasses(klass)
            else:
                self.register(klass)

    def register(self, klass: Type[Handler]):
        logging.info(f"Handler registered: {klass}")
        self._handler_dict[klass.key()] = klass()

    def parse_text(self, content: str):
        for line in content.splitlines():
            self._handle(line)

    def parse(self, path: str):
        file = open(path, "r")
        for line in file.readlines():
            self._handle(line)
        file.close()

    def _handle(self, line: str):
        line = line.strip()
        logging.debug(f"Handling Line: {line}")
        if len(line) == 0 or self._should_ignore(line):
            logging.debug(f"Line is empty or should be ignored: {line}")
            return
        key = self._retrieve_key(line)
        if key in self._handler_dict.keys():
            handler = self._handler_dict.get(key)
            logging.debug(f"Use handler: {handler}")
            handler(self._context, line)
        else:
            logging.warning(f"Line unrecognized, ignoring: {line}")

    @staticmethod
    def _context_base_class() -> Type[Context]:
        return Context

    @staticmethod
    def _handler_base_class() -> Type[Handler]:
        return Handler

    @staticmethod
    def _should_ignore(line: str) -> bool:
        raise NotImplementedError

    @staticmethod
    def _retrieve_key(line: str) -> str:
        raise NotImplementedError

    def dump(self) -> Any:
        raise NotImplementedError
