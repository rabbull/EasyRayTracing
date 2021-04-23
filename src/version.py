from typing import Union


class Version:
    major: Union[int, str] = 0
    minor: Union[int, str] = 0
    patch: Union[int, str] = 0

    def __init__(
            self,
            major: Union[int, str] = 0,
            minor: Union[int, str] = 0,
            patch: Union[int, str] = 0
    ):
        self.major = major
        self.minor = minor
        self.patch = patch

    def __str__(self):
        return '.'.join([str(self.major), str(self.minor), str(self.patch)])


version = Version(0, "dev", 0)
