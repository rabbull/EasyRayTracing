import logging

from .ObjHandler import ObjHandler
from ..context import ObjParserContext


class UseMtlHandler(ObjHandler):
    @staticmethod
    def key() -> str:
        return "usemtl"

    def __call__(self, context: ObjParserContext, line: str, *args,
                 **kwargs) -> None:
        splits = line.split(maxsplit=2)
        assert splits[0] == self.key()
        mtl_name = splits[1]
        if mtl_name not in context.material_dict:
            logging.error("Unknown mtl:", mtl_name)
        context.current_material = context.material_dict[mtl_name]
