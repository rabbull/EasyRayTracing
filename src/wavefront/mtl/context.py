from typing import *

import numpy as np

from ..common import Context


class MtlParserContextEntry:
    name: str = None                    # usemtl
    ambient: np.ndarray = None          # Ka
    diffuse: np.ndarray = None          # Kd
    specular: np.ndarray = None         # Ks
    transparency: float = None          # d & Tr
    roi: float = None                   # Ni


class MtlParserContext(Context):
    entries: List[MtlParserContextEntry] = []
