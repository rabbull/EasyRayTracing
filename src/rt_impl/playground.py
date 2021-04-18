import numpy as np

import rt_impl

if __name__ == '__main__':
    patch = np.array([[0, 0, 0], [0, 1, 0], [0, 0, 1], [1, 0, 0]], dtype=np.double)
    ray = np.array([[1, 0.1, 0.1], [-1, 0, 0]], dtype=np.double)
    print(rt_impl.hit(patch, ray))
