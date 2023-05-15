//
// Created by Gael Aglin on 2019-12-03.
//

#ifndef DL85_PY_ERROR_WRAPPER_H
#define DL85_PY_ERROR_WRAPPER_H

#include <Python.h>
#include "rCover.h"
#include "error_function.h" // cython helper file

class PyTidErrorClassWrapper {
public:
    // constructors and destructors mostly do reference counting
    PyTidErrorClassWrapper(PyObject* o): pyFunction(o) {
        Py_XINCREF(o);
    }

    PyTidErrorClassWrapper(const PyTidErrorClassWrapper& rhs): PyTidErrorClassWrapper(rhs.pyFunction) { // C++11 onwards only
    }

    PyTidErrorClassWrapper(PyTidErrorClassWrapper&& rhs): pyFunction(rhs.pyFunction) {
        rhs.pyFunction = nullptr;
    }

    // need no-arg constructor to stack allocate in Cython
    PyTidErrorClassWrapper(): PyTidErrorClassWrapper(nullptr) {
    }

    ~PyTidErrorClassWrapper() {
        Py_XDECREF(pyFunction);
    }

    PyTidErrorClassWrapper& operator=(const PyTidErrorClassWrapper& rhs) {
        PyTidErrorClassWrapper tmp = rhs;
        return (*this = std::move(tmp));
    }

    PyTidErrorClassWrapper& operator=(PyTidErrorClassWrapper&& rhs) {
        pyFunction = rhs.pyFunction;
        rhs.pyFunction = nullptr;
        return *this;
    }

    vector<float> operator()(RCover* ar) {
        PyInit_error_function();
        vector<float> result;
        if (pyFunction != nullptr) { // nullptr check
            float* result_pointer =  call_python_tid_error_class_function(pyFunction, ar); // note, no way of checking for errors until you return to Python
            result.push_back(result_pointer[0]);
            result.push_back(result_pointer[1]);
        }
        return result;
    }

    /*vector<float> operator()(RCover* ar) {
        PyInit_error_function();
        if (pyFunction) { // nullptr check
            float* result_pointer = call_python_tid_error_class_function(pyFunction, ar); // note, no way of checking for errors until you return to Python
            cout << "@re: " << result_pointer << endl;
            vector<float> result;
            int size = 2; // the function returns a vector of size 2 (error and class)
            for (int i = 0; i < size; i++) {
                result.push_back(result_pointer[i]);
            }
            // print r
            cout << "result: ";
            for (int i = 0; i < result.size(); i++) {
                cout << result[i] << " ";
            }
            cout << endl;
            return result;
        }
    }*/

    /*vector<float> operator()(RCover* ar) {
        int status = PyImport_AppendInittab("error_function", PyInit_error_function);
        if (status == -1) {
            vector<float> result;
            return result;
        }
        Py_Initialize();
        PyObject* module = PyImport_ImportModule("error_function");
        if (!module) {
            Py_Finalize();
            vector<float> result;
            return result;
        }

        vector<float> result;
        if (pyFunction) { // nullptr check
            result = *call_python_tid_error_class_function(pyFunction, ar); // note, no way of checking for errors until you return to Python
        }

        Py_Finalize();
        return result;
    }*/

private:
    PyObject* pyFunction;
};

#endif //DL85_PY_ERROR_WRAPPER_H
