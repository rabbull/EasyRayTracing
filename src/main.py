import logging

from cli import parse_arguments
from wavefront import Parser


def main():
    args = parse_arguments()
    logging.basicConfig(level=10 * (5 - args.verbose))
    logging.debug("debug")
    logging.info("info")
    logging.warning("warning")
    logging.error("error")
    logging.fatal("fatal")
    parser = Parser()
    parser.parse(args.obj)
    print(parser.dump())


if __name__ == '__main__':
    main()
