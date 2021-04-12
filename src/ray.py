from typing import *
from common import _DATA_TYPE

import numpy as np


class Ray:
    _origin: np.ndarray = None
    _orient: np.ndarray = None

    def __init__(self,
                 origin: np.ndarray = np.zeros(3, dtype=_DATA_TYPE),
                 orient: np.ndarray = np.zeros(3, dtype=_DATA_TYPE)):
        self._origin = origin
        self._orient = orient
