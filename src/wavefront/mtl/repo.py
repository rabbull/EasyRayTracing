from typing import *
import os


class MtlRepository:
    _mtl_files: Dict[str, str] = {}
    _cache = None

    def __init__(self, mtl_search_paths: Optional[Iterable[str]] = None):
        if mtl_search_paths is not None:
            for search_path in mtl_search_paths:
                for base, dirs, files in os.walk(search_path):
                    for file in files:
                        if file.endswith('.mtl'):
                            self._mtl_files[file] = os.path.join(base, file)

    def find(self, mtllib: str):
        return self._mtl_files.get(mtllib)
