from typing import *

import numpy as np
from PIL import Image


class Observer(object):
    _origin: np.ndarray = None
    _orient: np.ndarray = None

    def __init__(self, origin: Union[np.ndarray, List[float]] = np.zeros(3),
                 orient: Union[np.ndarray, List[float]] = np.zeros(3)):
        self._origin = np.asarray(origin, dtype=np.float64)
        self._orient = np.asarray(orient, dtype=np.float64)


class Canvas(object):
    _res_x: int = 0
    _res_y: int = 0
    _width: float = 0
    _height: float = 0
    _data: np.ndarray = None

    def __init__(self, resolution: Tuple[int, int] = (800, 600),
                 size: Tuple[float, float] = (1, 1),
                 bg_color: np.ndarray = np.zeros(4, dtype=np.uint8)):
        self._res_x, self._res_y = resolution
        self._width, self._height = size
        self._data = np.array(
            [[bg_color] * self._res_x] * self._res_y,
            dtype=np.uint8
        )

    def as_image(self) -> Image:
        self._data[:, :, 3] = np.ones((self._res_y, self._res_x)) * 255
        return Image.fromarray(self._data)


class Camera(object):
    _observer: Observer = None
    _canvas: Canvas = None
    _focal_length: float = 0

    def __init__(self, observer: Observer = Observer(),
                 canvas: Canvas = Canvas(), f: float = 0):
        self._observer = observer
        self._canvas = canvas
        self._focal_length = f

    def capture(self) -> Image:
        return self._canvas.as_image()
