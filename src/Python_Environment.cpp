#include "../header/Python_Environment.hpp"

void init_python_environment(){
    Py_Initialize();
}

void clean_python_environment(){
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
}