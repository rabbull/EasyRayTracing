//
// Created by Karl on 4/11/2021.
//

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <numpy/ndarrayobject.h>

#include <camera.h>
#include <patch.h>
#include <scene.h>

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
        "observe",
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

    bool_t flag = patch_hit(PyArray_DATA(patch), PyArray_DATA(ray), &distance,
                            NULL);
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

    flag = fill_color(pix, ray, &scene, 0, 1, NULL, NULL);
    return PyLong_FromLong(flag);
}

static PyObject *
rt_impl_observe(PyObject *self, PyObject *args) {
    int i;
    size_t mtl_idx;
    PyObject *scene_obj;
    PyObject *lights_obj;
    PyObject *light_obj;
    PyObject *camera_obj;
    PyObject *observer_obj;
    PyObject *canvas_obj;
    PyObject *origin_obj;
    PyObject *orient_obj;
    PyObject *array_obj;
    PyObject *material_list_obj;
    PyObject *material_obj;
    scene_t scene;
    camera_t camera;
    real_t *p;
    char PTRC method;

    if (!PyArg_ParseTuple(args, "sOO", &method, &camera_obj, &scene_obj)) {
        return NULL;
    }
    array_obj = PyObject_GetAttrString(scene_obj, "_data");
    if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
        fprintf(stderr, "scene._data is not C contiguous!\n");
        return NULL;
    }
    scene.num_patches = PyArray_DIM(array_obj, 0);
    scene.patches = PyArray_DATA(array_obj);
    lights_obj = PyObject_GetAttrString(scene_obj, "_lights");
    if (array_obj != Py_None) {
        scene.num_lights = PyList_Size(lights_obj);
        scene.lights = calloc(scene.num_lights, sizeof(light_t));
        for (i = 0; i < scene.num_lights; ++i) {
            light_obj = PyList_GetItem(lights_obj, i);
            array_obj = PyObject_GetAttrString(light_obj, "origin");
            if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
                fprintf(stderr,
                        "scene._lights[%d].origin is not C contiguous!\n", i);
                return NULL;
            }
            vec3_copy(&scene.lights[i].origin, PyArray_DATA(array_obj));
            array_obj = PyObject_GetAttrString(light_obj, "color");
            if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
                fprintf(stderr,
                        "scene._lights[%d].color is not C contiguous!\n", i);
                return NULL;
            }
            memcpy(&scene.lights[i].color, PyArray_DATA(array_obj), 3);
        }
    } else {
        scene.num_lights = 0;
        scene.lights = NULL;
    }

    material_list_obj = PyObject_GetAttrString(scene_obj, "_materials");
    scene.num_materials = PyList_Size(material_list_obj);
    scene.materials = calloc(sizeof(material_t), scene.num_materials);
    for (i = 0; i < scene.num_materials; ++i) {
        material_obj = PyList_GetItem(material_list_obj, i);
        array_obj = PyObject_GetAttrString(material_obj, "k");
        if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
            fprintf(stderr, "scene._materials[%d].k is not C contiguous!\n", i);
            return NULL;
        }
        scene.materials[i] = *(material_t *) PyArray_DATA(array_obj);
    }
    array_obj = PyObject_GetAttrString(scene_obj, "_mtl_indices");
    if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
        fprintf(stderr, "scene._mtl_indices is not C contiguous!\n");
    }
    scene.patch_material = PyArray_DATA(array_obj);
    for (i = 0; i < scene.num_patches; ++i) {
        mtl_idx = ((npy_intp *) scene.patch_material)[i];
        printf("%d %p\n", i, mtl_idx);
        if (mtl_idx != 0) {
            scene.patch_material[i] = scene.materials + mtl_idx - 1;
        }
    }

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

    array_obj = PyObject_GetAttrString(canvas_obj, "_data");
    if (!PyArray_IS_C_CONTIGUOUS(array_obj)) {
        fprintf(stderr, "camera._canvas._data is not C contiguous!\n");
        return NULL;
    }
    camera.canvas.data = PyArray_DATA(array_obj);

    camera.focal_length = PyFloat_AsDouble(
            PyObject_GetAttrString(camera_obj, "_focal_length"));

    observe(&camera, &scene, method);
    free(scene.materials);
    free(scene.lights);
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
