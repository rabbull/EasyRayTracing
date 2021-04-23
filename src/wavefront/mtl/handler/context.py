from typing import *

import numpy as np


class MtlParserContextEntity(object):
    name: str = None                    # usemtl
    ambient: np.ndarray = None          # Ka
    diffuse: np.ndarray = None          # Kd
    specular: np.ndarray = None         # Ks
    transparency: float = None          # d & Tr
    roi: float = None                   # Ni


class MtlParserContext(object):
    entities: List[MtlParserContextEntity] = []
