from typing import *

from .context import MtlParserContext
from .handler import MtlHandler
from ..common import BaseLinedTextFileParser, Context, Handler


class WaveFrontMtlParser(BaseLinedTextFileParser):
    _context: MtlParserContext
    _handler_dict: Dict[str, MtlHandler]

    def __init__(self):
        super().__init__()

    @staticmethod
    def _context_base_class() -> Type[Context]:
        return MtlParserContext

    @staticmethod
    def _handler_base_class() -> Type[Handler]:
        return MtlHandler

    @staticmethod
    def _should_ignore(line: str) -> bool:
        return line[0] == '#'

    @staticmethod
    def _retrieve_key(line: str) -> str:
        return line.split(maxsplit=2)[0]

    def dump(self):
        ret = {}
        for entry in self._context.entities:
            ret[entry.name] = entry
        return ret
