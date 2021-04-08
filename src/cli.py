import argparse
from version import version


def parse_arguments():
    parser = argparse.ArgumentParser("ert")
    parser.add_argument("-V", "--version", action="version",
                        version=f"%(prog)s {version}",
                        help="print the version info and exit")
    parser.add_argument("-v", "--verbose", type=int, action="store", default=2,
                        help="set verbosity from 0 (quiet) to 5, default: [2]")
    parser.add_argument("obj", action="store", type=str,
                        help="required, path .OBJ file to render")
    parser.add_argument("--mtl", action="extend", nargs="+", type=str,
                        help=".MTL file search paths, default: [.]")
    parser.add_argument("--camera-pos", action="store", default="0,0,0",
                        help="coordinate of the camera, default: [0,0,0]")
    parser.add_argument("--camera-orient", action="store", default="1,0,0",
                        help="orientation of the camera, default: [1,0,0]")
    parser.add_argument("--focal-length", action="store", type=float, default=1,
                        help="focal length, default: [1]")
    parser.add_argument("-o", "--output", action="store", default="./scene.jpg",
                        help="path to rendered image, default: [./scene.jpg]")
    parser.add_argument("--resolution", action="store", default="800x600",
                        help="resolution of rendered image, default: [800x600]")
    return parser.parse_args()