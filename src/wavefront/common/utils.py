from typing import *

import numpy as np


def cat(arrays: Iterable[np.ndarray]) -> Optional[np.ndarray]:
    arrays = tuple(arrays)
    if len(arrays) == 0:
        return None
    return np.stack(arrays)
