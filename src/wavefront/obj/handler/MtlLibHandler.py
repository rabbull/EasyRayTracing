import logging

from .ObjHandler import ObjHandler
from ..context import ObjParserContext


class MtlLibHandler(ObjHandler):
    @staticmethod
    def key() -> str:
        return "mtllib"

    def __call__(self, context: ObjParserContext, line: str, *args,
                 **kwargs) -> None:
        splits = line.split(maxsplit=2)
        assert splits[0] == self.key()
        material_dict = context.mtl_repository.find(splits[1])
        for name in material_dict:
            if name in context.material_dict:
                logging.warning("Repetitive name detected, ignoring:", name)
                continue
            context.material_dict[name] = material_dict[name]
