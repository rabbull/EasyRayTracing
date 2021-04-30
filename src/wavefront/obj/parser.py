import logging
from typing import *

import numpy as np

from .context import ObjParserContext
from .handler import ObjHandler
from ..common import BaseLinedTextFileParser, Handler, Context
from ..common import cat


class WaveFrontObjParser(BaseLinedTextFileParser):
    _context: ObjParserContext

    def __init__(self, mtl_search_paths: Optional[Iterable[str]] = None, *args,
                 **kwargs):
        super().__init__(mtl_search_paths, *args, **kwargs)
        logging.info(f"MTL file search paths: {mtl_search_paths}")

    @staticmethod
    def _context_base_class() -> Type[Context]:
        return ObjParserContext

    @staticmethod
    def _handler_base_class() -> Type[Handler]:
        return ObjHandler

    @staticmethod
    def _should_ignore(line: str) -> bool:
        return line[0] == '#'

    @staticmethod
    def _retrieve_key(line: str) -> str:
        return line.split(maxsplit=2)[0]

    def dump(self) -> Dict[str, Any]:
        context = self._context
        raw = {
            "v": cat(context.vertices),
            "vt": cat(context.texture_coordinates),
            "vn": cat(context.vertex_normals),
            "f": cat(context.faces),
        }
        num_faces = raw["f"].shape[0]
        vertex_indices = raw["f"][:, :, 0] - 1
        faces = raw["v"][vertex_indices]

        normal_indices = np.any(raw["f"][:, :, 2] != 0, axis=1)
        normals = np.zeros((num_faces, 3), dtype=np.float64)

        if np.any(normal_indices):
            normals[normal_indices, :] = \
                np.average(
                    raw["vn"][raw["f"][normal_indices, :, 2] - 1],
                    axis=1
                )
        if np.any(~normal_indices):
            normals[~normal_indices, :] = np.cross(
                faces[~normal_indices, 2] - faces[~normal_indices, 0],
                faces[~normal_indices, 1] - faces[~normal_indices, 0]
            )
            normals[~normal_indices, :] /= \
                np.linalg.norm(normals, axis=1)[:, np.newaxis]

        material_indices = []
        materials = []
        material_name_index_mapping = {}
        for material_name in context.materials:
            if material_name is None:
                material_indices.append(0)
            else:
                if material_name not in material_name_index_mapping:
                    materials.append(context.material_dict[material_name])
                    material_name_index_mapping[material_name] = len(materials)
                material_index = material_name_index_mapping[material_name]
                material_indices.append(material_index)
        material_indices = np.array(material_indices, dtype=np.int64)

        return {
            "raw": raw,
            "aggregated": {
                "faces": faces,
                "normals": normals,
                "materials": materials,
                "mtl_indices": material_indices
            }
        }
