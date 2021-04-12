from distutils.core import setup, Extension

rt_impl_ext = Extension('rt_impl', sources=["rt_impl.c"])

setup(name="rt_impl", ext_modules=[rt_impl_ext])
