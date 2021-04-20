import numpy as np


class Scene(object):
    _data: np.ndarray = None

    def __init__(self, faces: np.ndarray, normals: np.ndarray):
        assert faces.shape[0] == normals.shape[0]
        assert (faces.ndim == 3 and faces.shape[1] == faces.shape[2] == 3) \
               or (faces.ndim == 2 and faces.shape[1] == 9)
        assert normals.ndim == 2 and normals.shape[1] == 3

        if faces.ndim == 2:
            self._data = np.concatenate((faces, normals), 1)
        else:
            self._data = np.concatenate((faces, normals[:, np.newaxis]), 1)
