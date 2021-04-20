from typing import Tuple

import numpy as np
from PIL import Image

from src.rt_impl.rt_models.scene import Scene


class Camera(object):
    _position: np.ndarray = None
    _orient: np.ndarray = None

    def __init__(self, position: np.ndarray = np.zeros(3),
                 orient: np.ndarray = np.zeros(3)):
        self._position = position
        self._orient = orient


class Canvas(object):
    _width: int = 0
    _height: int = 0
    _data: np.ndarray = None

    def __init__(self, size: Tuple[int, int] = (800, 600),
                 bg_color: np.ndarray = np.zeros(4, dtype=np.uint8)):
        self._width, self._height = size
        self._data = np.array(
            [[bg_color] * self._width] * self._height,
            dtype=np.uint8
        )

    def as_image(self) -> Image:
        return Image.fromarray(self._data)


class Observer(object):
    _camera: Camera = None
    _canvas: Canvas = None
    _focal_dist: float = 0

    def __init__(self, camera: Camera = Camera(),
                 canvas: Canvas = Canvas(), f: float = 0):
        self._camera = camera
        self._canvas = canvas
        self._focal_dist = f

    def observe(self, scene: Scene = None) -> Image:
        # TODO: ray tracing here
        return self._canvas.as_image()

    def __call__(self, *args, **kwargs):
        return self.observe(*args, **kwargs)
