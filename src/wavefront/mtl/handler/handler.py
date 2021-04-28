import logging
from typing import *

import numpy as np

from ..context import MtlParserContext, MtlParserContextEntity
from ...common import Handler
from ...common.handler import FixedLengthVectorHandler


class MtlHandler(Handler):
    @staticmethod
    def key():
        raise NotImplementedError

    @staticmethod
    def data_type() -> Type[np.dtype]:
        return np.float64

    def __call__(self, context: MtlParserContext, line: str) -> None:
        if len(context.entities) == 0:
            logging.warning("Line out of a MTL, ignoring:", line)
            return


class NewMtlHandler(MtlHandler):
    @staticmethod
    def key():
        return "newmtl"

    def __call__(self, context: MtlParserContext, line: str) -> None:
        entity = MtlParserContextEntity()
        entity.name = line.split(maxsplit=2)[1]

        # Default values are given by
        # `https://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html`
        entity.ambient = np.array([0.2, 0.2, 0.2], dtype=self.data_type())
        entity.diffuse = np.array([0.8, 0.8, 0.8], dtype=self.data_type())
        entity.specular = np.array([1., 1., 1.], dtype=self.data_type())
        entity.transparency = 1.
        entity.shininess = 0.
        entity.roi = 1.  # index of refraction

        context.entities.append(entity)


class FixedLengthVectorMtlHandler(FixedLengthVectorHandler, MtlHandler):
    @staticmethod
    def key() -> str:
        raise NotImplementedError

    @staticmethod
    def _num_prefixes() -> int:
        return 1

    @staticmethod
    def _num_suffixes() -> int:
        return 0

    @staticmethod
    def _vector_size() -> int:
        return 3

    @staticmethod
    def _dump(context: MtlParserContext, vector: List[float]) -> None:
        raise NotImplementedError


class AmbientHandler(FixedLengthVectorMtlHandler):
    @staticmethod
    def key():
        return "ka"

    @staticmethod
    def _dump(context: MtlParserContext, vector: List[float]) -> None:
        context.entities[-1].ambient = \
            np.array(vector, dtype=AmbientHandler.data_type())


class DiffuseHandler(FixedLengthVectorMtlHandler):
    @staticmethod
    def key():
        return "kd"

    @staticmethod
    def _dump(context: MtlParserContext, vector: List[float]) -> None:
        context.entities[-1].diffuse = \
            np.array(vector, dtype=AmbientHandler.data_type())


class SpecularHandler(FixedLengthVectorMtlHandler):
    @staticmethod
    def key():
        return "ks"

    @staticmethod
    def _dump(context: MtlParserContext, vector: List[float]) -> None:
        context.entities[-1].specular = \
            np.array(vector, dtype=AmbientHandler.data_type())
