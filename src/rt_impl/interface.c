//
// Created by Karl on 4/11/2021.
//

#define PY_SSIZE_T_CLEAN

#include <Python.h>

static PyObject *SpamError;

PyObject *
rt_impl_system(PyObject *self, PyObject *args);

static PyMethodDef RtImplMethods[] = {
        {"system", rt_impl_system, METH_VARARGS,
                "Execute a shell command."},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef rt_impl_module = {
        PyModuleDef_HEAD_INIT,
        "rt_impl",
        NULL,
        -1,
        RtImplMethods
};

static PyObject *
rt_impl_system(PyObject *self, PyObject *args) {
    char const *cmd;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &cmd)) {
        return NULL;
    }
    sts = system(cmd);
    if (sts < 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }
    return PyLong_FromLong(sts);
}

PyMODINIT_FUNC
PyInit_rt_impl(void) {
    PyObject *m;

    m = PyModule_Create(&rt_impl_module);
    if (m == NULL)
        return NULL;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_XINCREF(SpamError);
    if (PyModule_AddObject(m, "error", SpamError) < 0) {
        Py_XDECREF(SpamError);
        Py_CLEAR(SpamError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
