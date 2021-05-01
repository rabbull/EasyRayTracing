import numpy as np

from .ObjHandler import ObjHandler
from ..context import ObjParserContext


class FaceHandler(ObjHandler):
    @staticmethod
    def key():
        return "f"

    def __call__(self, context: ObjParserContext, line: str, *args,
                 **kwargs) -> None:
        splits = line.split()
        assert splits[0] == self.key()
        vertex_count = len(splits[1:])

        # TODO: implement cases with variable number of vertices
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
                if fields[1] == "" and fields[2] == "":
                    indices = [0]
                elif fields[1] == "":
                    indices = [0, 2]
                else:
                    indices = [0, 1, 2]
            else:
                raise ValueError
            for idx in indices:
                field = int(fields[idx])
                assert field > 0
                face[i, idx] = int(fields[idx])

        if context.current_material is not None:
            context.materials.append(context.current_material.name)
        else:
            context.materials.append(None)

        context.faces.append(face)
