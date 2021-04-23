import logging

import numpy as np

from .context import MtlParserContext, MtlParserContextEntity


class MtlHandler(object):
    @staticmethod
    def key():
        raise NotImplementedError

    @staticmethod
    def data_type() -> np.dtype:
        return np.float64

    def __call__(self, context: MtlParserContext, line: str) -> None:
        if len(context.entities) == 0:
            logging.warning("Ignoring dangling line:", line)
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
        entity.roi = 1.     # index of refraction

        context.entities.append(entity)


class AmbientHandler(MtlHandler):
    @staticmethod
    def key():
        return "ka"

    def __call__(self, context: MtlParserContext, line: str) -> None:
        super.__call__(context, line)

        splits = line.split()
        assert splits[0].lower() == self.key()
        assert len(splits) == 4

        context.entities[-1].ambient = \
            np.array([float(s) for s in splits[1:]], dtype=self.data_type())


class DiffuseHandler(MtlHandler):
    @staticmethod
    def key():
        return "kd"

    def __call__(self, context: MtlParserContext, line: str) -> None:
        super.__call__(context, line)

        splits = line.split()
        assert splits[0].lower() == self.key()
        assert len(splits) == 4

        context.entities[-1].diffuse = \
            np.array([float(s) for s in splits[1:]], dtype=self.data_type())


class SpecularHandler(MtlHandler):
    @staticmethod
    def key():
        return "kd"

    def __call__(self, context: MtlParserContext, line: str) -> None:
        super.__call__(context, line)

        splits = line.split()
        assert splits[0].lower() == self.key()
        assert len(splits) == 4

        context.entities[-1].specular = \
            np.array([float(s) for s in splits[1:]], dtype=self.data_type())


