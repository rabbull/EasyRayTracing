import os
from distutils.core import setup, Extension

import numpy

sources = []
for base, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.c'):
            sources.append(os.path.join(base, file))

rt_impl_ext = Extension(
    'rt_impl',
    sources=sources,
    libraries=['cblas', 'lapacke'],
    include_dirs=['src/include', numpy.get_include()]
)

setup(name="rt_impl", ext_modules=[rt_impl_ext])
