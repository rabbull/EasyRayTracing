import logging

import numpy as np
from PIL import Image

from cli import parse_arguments
from camera import Observer, Canvas


def main():
    args = parse_arguments()

    # initialize logger
    logging_level = 10 * (5 - args.verbose)
    logging.basicConfig(level=logging_level)

    observer = Observer(canvas=Canvas(bg_color=np.array([255, 0, 0, 0])))
    img: Image = observer.observe()
    img.show()


if __name__ == '__main__':
    main()
