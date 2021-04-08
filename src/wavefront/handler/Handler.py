from .Context import Context


class Handler(object):
    @staticmethod
    def key():
        raise NotImplementedError

    def __call__(self, context: Context, line: str) -> None:
        raise NotImplementedError
