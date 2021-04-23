import os
from distutils.core import setup, Extension

import numpy

sources = []
for base, dirs, files in os.walk('cpu_impl'):
    for file in files:
        if file.endswith('.c'):
            sources.append(os.path.join(base, file))

rt_cpu_impl = Extension(
    'rt_impl',
    sources=sources,
    libraries=['cblas', 'lapacke', 'gomp', 'pthread'],
    include_dirs=['cpu_impl/include', numpy.get_include()],
    extra_compile_args=['-fopenmp'],
    define_macros=[('DOUBLE_PRECISION', '')]
)

setup(
    name="rt_impl",
    packages=['rt_models'],
    ext_modules=[rt_cpu_impl]
)
