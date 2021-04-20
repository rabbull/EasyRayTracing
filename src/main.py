import logging
import math
import time

from rt_models import Camera, Canvas, Observer, Scene

import rt_impl as rt
from cli import parse_arguments
from wavefront import Parser


def main():
    args = parse_arguments()

    # initialize logger
    logging_level = 10 * (5 - args.verbose)
    logging.basicConfig(level=logging_level)

    parser = Parser()
    parser.parse(args.obj)
    scene = Scene(**parser.dump()['aggregated'])
    camera = Camera(
        observer=Observer([0, 0, 10], [0, math.pi / 2, 0]),
        canvas=Canvas(resolution=(64, 64), size=(2, 2)),
        f=1
    )
    t = time.time()
    rt.observe(camera, scene)
    print(f"time elapsed: {time.time() - t}s")
    camera.capture().save("/srv/http/output.png")


if __name__ == '__main__':
    main()
