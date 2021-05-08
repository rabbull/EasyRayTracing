import argparse
from version import version


def parse_arguments():
    parser = argparse.ArgumentParser("ert")

    # logging
    parser.add_argument("-V", "--version", action="version",
                        version=f"%(prog)s {version}",
                        help="print the version info and exit")
    parser.add_argument("-v", "--verbose", type=int, action="store", default=2,
                        help="set verbosity from 0 (quiet) to 5, default: [2]")

    # scene
    parser.add_argument("--obj", action="store", type=str,
                        help="*REQUIRED*, the .OBJ file to be rendered")
    parser.add_argument("--lights", action="store", type=str,
                        help="*REQUIRED*, positions and colors of the light "
                             "sources, separated by [;], e.g.: "
                             "[0,0,0;1,1,1,255,0,0]; if the color is not given,"
                             "a white color will be used as default")
    parser.add_argument("--mtl", action="extend", nargs="+", type=str,
                        help=".MTL file search paths, default: [.]")

    # camera
    parser.add_argument("--camera-origin", action="store", default="0,0,0",
                        help="coordinate of the camera, default: [0,0,0]")
    parser.add_argument("--camera-orient", action="store", default="0,0,0",
                        help="orientation of the camera, default: [0,0,0]")
    parser.add_argument("--focal-length", action="store", type=float, default=1,
                        help="focal length, default: [1]")
    parser.add_argument("--canvas-size", action="store", default="4x3",
                        help="size of the canvas, default: [4x3]")

    # output
    parser.add_argument("--resolution", action="store", default="800x600",
                        help="resolution of rendered image, default: [800x600]")
    parser.add_argument("-o", "--output", action="store", default="./scene.png",
                        help="path to rendered image, default: [./scene.png]")

    return parser.parse_args()
