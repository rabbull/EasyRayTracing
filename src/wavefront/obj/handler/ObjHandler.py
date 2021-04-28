from typing import Type

import numpy as np

from ..context import ObjParserContext
from ...common import Handler


class ObjHandler(Handler):
    @staticmethod
    def key() -> str:
        raise NotImplementedError

    @staticmethod
    def data_type() -> Type[np.dtype]:
        return np.float64

    def __call__(self, context: ObjParserContext, line: str, *args,
                 **kwargs) -> None:
        raise NotImplementedError
