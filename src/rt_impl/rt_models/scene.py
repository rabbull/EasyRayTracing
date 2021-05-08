from typing import *

import numpy as np


class Material:
    k: np.ndarray  # shape: [9,], [ka, kd, ks]
    a_texture: Optional[np.ndarray]  # shape: [w, h, 4], dtype: uint8
    d_texture: Optional[np.ndarray]  # shape: [w, h, 4], dtype: uint8
    s_texture: Optional[np.ndarray]  # shape: [w, h, 4], dtype: uint8


class Light:
    origin: np.ndarray  # shape: [3,]
    color: np.ndarray  # shape: [3,], dtype: uint8


class Scene(object):
    _data: np.ndarray = None  # shape: [N, 12]
    _lights: List[Light] = None
    _mtl_indices: np.ndarray = None  # shape: [N,]
    _materials: List[Material] = None

    def __init__(self, faces: np.ndarray, normals: np.ndarray,
                 mtl_indices: np.ndarray, materials: List[Material],
                 lights: List[Light]):
        assert faces.shape[0] == normals.shape[0]
        assert (faces.ndim == 3 and faces.shape[1] == faces.shape[2] == 3) \
               or (faces.ndim == 2 and faces.shape[1] == 9)
        assert normals.ndim == 2 and normals.shape[1] == 3

        if faces.ndim == 2:
            self._data = np.concatenate((faces, normals), 1)
        else:
            self._data = np.concatenate((faces, normals[:, np.newaxis]), 1)

        self._mtl_indices = mtl_indices
        self._materials = materials
        self._lights = lights
