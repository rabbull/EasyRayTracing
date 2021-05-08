import logging
import time

import numpy as np
import rt_impl as rt
from rt_models import Camera, Canvas, Observer, Scene
from rt_models.scene import Light, Material

from cli import parse_arguments
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

    lights = []
    for light_str in args.lights.split(';'):
        splits = light_str.split(',')
        origin_splits = splits[:3]
        if len(splits) == 6:
            color_splits = splits[3:]
        else:
            color_splits = ['255', '255', '255']
        light = Light()
        light.origin = np.array(origin_splits, dtype=np.float64)
        light.color = np.array(color_splits, dtype=np.uint8)
        lights.append(light)
    data['lights'] = lights

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
    print(data['mtl_indices'])
    print(data['materials'])
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
    rt.observe("naive", camera, scene)
    logging.info(f"Time elapsed: {time.time() - t}s")
    camera.capture().save(args.output)


if __name__ == '__main__':
    main()
