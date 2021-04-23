//
// Created by Karl on 4/11/2021.
//

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <numpy/ndarrayobject.h>

#include <patch.h>
#include <scene.h>

#include "include/alg.h"

static PyObject *SpamError;

static PyObject *
rt_impl_hit(PyObject *self, PyObject *args);

static PyObject *
rt_impl_color(PyObject *self, PyObject *args);

static PyObject *
rt_impl_observe(PyObject *self, PyObject *args);

static PyMethodDef RtImplMethods[] = {
        {"hit",     rt_impl_hit,     METH_VARARGS, "hit"},
        {"color",   rt_impl_color,   METH_VARARGS, "color"},
        {"observe", rt_impl_observe, METH_VARARGS, "observe"},
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
rt_impl_hit(PyObject *self, PyObject *args) {
    PyObject *patch, *ray;
    real_t distance;

    if (!PyArg_ParseTuple(args, "OO", &patch, &ray)) {
        return NULL;
    }

    patch_t *patch_ptr = PyArray_DATA(patch);
    ray_t *ray_ptr = PyArray_DATA(ray);
    print_vec3(&patch_ptr->normal, "normal");
    print_vec3(&ray_ptr->origin, "origin");
    print_vec3(&ray_ptr->direction, "direction");

    bool_t flag = patch_hit(PyArray_DATA(patch), PyArray_DATA(ray), &distance, NULL);
    return Py_BuildValue("(if)", flag, distance);
}

static PyObject *
rt_impl_color(PyObject *self, PyObject *args) {
    PyObject *pix_obj, *patches_obj, *ray_obj;
    scene_t scene;

    ray_t *ray;
    pix_t *pix;
    bool_t flag;

    if (!PyArg_ParseTuple(args, "OOO", &pix_obj, &patches_obj, &ray_obj)) {
        return NULL;
    }

    scene.num_patches = PyArray_DIMS(patches_obj)[0];
    scene.patches = PyArray_DATA(patches_obj);
    ray = PyArray_DATA(ray_obj);
    pix = PyArray_DATA(pix_obj);

    flag = fill_color(pix, ray, &scene, 0, 1);
    return PyLong_FromLong(flag);
}

static PyObject *
rt_impl_observe(PyObject *self, PyObject *args) {
    size_t i;
    PyObject *scene_obj;
    PyObject *camera_obj;
    PyObject *observer_obj;
    PyObject *canvas_obj;
    PyObject *origin_obj;
    PyObject *orient_obj;
    PyObject *array_obj;
    scene_t scene;
    camera_t camera;
    real_t *p;

    if (!PyArg_ParseTuple(args, "OO", &camera_obj, &scene_obj)) {
        return NULL;
    }
    array_obj = PyObject_GetAttrString(scene_obj, "_data");
    scene.num_patches = PyArray_DIM(array_obj, 0);
    scene.patches = PyArray_DATA(array_obj);
    array_obj = PyObject_GetAttrString(scene_obj, "_light");
    scene.num_lights = PyArray_DIM(array_obj, 0);
    scene.lights = PyArray_DATA(array_obj);

    canvas_obj = PyObject_GetAttrString(camera_obj, "_canvas");
    observer_obj = PyObject_GetAttrString(camera_obj, "_observer");
    origin_obj = PyObject_GetAttrString(observer_obj, "_origin");
    orient_obj = PyObject_GetAttrString(observer_obj, "_orient");
    for (i = 0; i < 3; ++i) {
        p = PyArray_GETPTR1(origin_obj, i);
        camera.observer.origin.d[i] = *p;
        p = PyArray_GETPTR1(orient_obj, i);
        camera.observer.orientation.d[i] = *p;
    }
    camera.canvas.res_x = PyLong_AsLong(
            PyObject_GetAttrString(canvas_obj, "_res_x"));
    camera.canvas.res_y = PyLong_AsLong(
            PyObject_GetAttrString(canvas_obj, "_res_y"));
    camera.canvas.width = PyFloat_AsDouble(
            PyObject_GetAttrString(canvas_obj, "_width"));
    camera.canvas.height = PyFloat_AsDouble(
            PyObject_GetAttrString(canvas_obj, "_height"));
    camera.canvas.data = PyArray_DATA(
            PyObject_GetAttrString(canvas_obj, "_data"));

    camera.focal_length = PyFloat_AsDouble(
            PyObject_GetAttrString(camera_obj, "_focal_length"));

    rt_impl(&camera, &scene);
    Py_RETURN_NONE;
}

PyMODINIT_FUNC
PyInit_rt_impl(void) {
    PyObject *m;

    m = PyModule_Create(&rt_impl_module);
    if (m == NULL)
        return NULL;

    import_array()

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
