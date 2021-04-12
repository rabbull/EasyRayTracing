from typing import List

import numpy as np


class Scene(object):
    _objects: np.ndarray = None

    def __init__(self, objects: np.ndarray):
        self._objects = objects
