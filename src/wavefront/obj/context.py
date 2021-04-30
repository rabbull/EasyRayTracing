import logging
from typing import *

import numpy as np

from .repo import MtlFileRepository
from ..common import Context
from ..mtl import WaveFrontMaterial


class ObjParserContext(Context):
    mtl_repository: MtlFileRepository = None
    material_dict: Dict[str, WaveFrontMaterial] = {}
    current_material: WaveFrontMaterial = None

    vertices: List[np.ndarray] = []
    texture_coordinates: List[np.ndarray] = []
    vertex_normals: List[np.ndarray] = []
    faces: List[np.ndarray] = []
    materials: List[Optional[str]] = []

    def __init__(self, mtl_search_paths: Iterable[str], *args, **kwargs):
        super().__init__(*args, **kwargs)
        logging.info(f"Initializing MTL file repository, "
                     f"search paths: {mtl_search_paths}")
        self.mtl_repository = MtlFileRepository(mtl_search_paths)

    def __str__(self):
        return "vertices: %s\n" % self.vertices \
               + "texture_coordinates: %s\n" % self.texture_coordinates \
               + "vertex_normals: %s\n" % self.vertex_normals \
               + "faces: %s\n" % self.faces \
               + "materials: %s\n" % self.materials
