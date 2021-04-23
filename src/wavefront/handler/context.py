from typing import *

import numpy as np


class ObjParserContext(object):
    vertices: List[np.ndarray] = []
    texture_coordinates: List[np.ndarray] = []
    vertex_normals: List[np.ndarray] = []
    faces: List[np.ndarray] = []

    def __str__(self):
        return "vertices: %s\n" % self.vertices \
               + "texture_coordinates: %s\n" % self.texture_coordinates \
               + "vertex_normals: %s\n" % self.vertex_normals \
               + "faces: %s\n" % self.faces
