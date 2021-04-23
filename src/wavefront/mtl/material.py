import numpy as np


class Material(object):
    _name: str = None
    _ambient: np.ndarray = None
    _diffuse: np.ndarray = None
    _specular: np.ndarray = None
    
