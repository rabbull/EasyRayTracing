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
    libraries=['cblas', 'lapacke', 'gomp', 'pthread'],
    include_dirs=['src/include', numpy.get_include()],
    extra_compile_args=['-fopenmp']
)

setup(
    name="rt_impl",
    packages=['rt_models'],
    ext_modules=[rt_impl_ext]
)
