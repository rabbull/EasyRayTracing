import logging
import time

import numpy as np
from rt_models import Camera, Canvas, Observer, Scene

import rt_impl as rt
from cli import parse_arguments
from src.rt_impl.rt_models.scene import Material
from wavefront import WaveFrontObjParser


def main():
    args = parse_arguments()

    # initialize logger
    logging_level = 10 * (5 - args.verbose)
    logging.basicConfig(level=logging_level)

    logging.debug(str(args))

    parser = WaveFrontObjParser(args.mtl)
    parser.parse(args.obj)
    data = parser.dump()['aggregated']

    materials = data['materials']
    data['materials'] = []
    for entry in materials:
        material = Material()
        material.k = np.concatenate(
            (entry.ambient, entry.diffuse, entry.specular)
        )
        material.a_texture = None
        material.d_texture = None
        material.s_texture = None
        data['materials'].append(material)
    scene = Scene(**data)

    observer_origin = [float(e) for e in args.camera_origin.split(',')]
    observer_orient = [float(e) for e in args.camera_orient.split(',')]
    assert len(observer_origin) == len(observer_orient) == 3
    observer = Observer(observer_origin, observer_orient)

    canvas_res = tuple(int(e) for e in args.resolution.split('x'))
    canvas_size = tuple(float(e) for e in args.canvas_size.split('x'))
    assert len(canvas_res) == len(canvas_size) == 2
    canvas = Canvas(resolution=canvas_res, size=canvas_size)

    camera = Camera(observer=observer, canvas=canvas, f=args.focal_length)
    t = time.time()
    rt.observe(camera, scene)
    logging.info(f"Time elapsed: {time.time() - t}s")
    camera.capture().save(args.output)


if __name__ == '__main__':
    main()
