from typing import *
import os

from .context import MtlParserContextEntity
from .parser import WaveFrontMtlParser


class MtlFileRepository:
    _mtl_files: Dict[str, str] = {}
    _cache: Dict[str, List[MtlParserContextEntity]] = {}

    def __init__(self, mtl_search_paths: Optional[Iterable[str]] = None):
        if mtl_search_paths is not None:
            for search_path in mtl_search_paths:
                for base, dirs, files in os.walk(search_path):
                    for file in files:
                        if file.endswith('.mtl'):
                            self._mtl_files[file] = os.path.join(base, file)

    def find(self, mtllib: str):
        if mtllib not in self._cache:
            if mtllib not in self._mtl_files:
                raise FileNotFoundError
            mtl_file = self._mtl_files.get(mtllib)
            mtl_parser = WaveFrontMtlParser()
            mtl_parser.parse(mtl_file)
            self._cache[mtllib] = mtl_parser.dump()
        return self._cache[mtllib]
