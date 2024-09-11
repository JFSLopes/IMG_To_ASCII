#ifndef PYTHON_ENVIRONMENT_HPP
#define PYTHON_ENVIRONMENT_HPP

#include <Python.h>
/**
 * Initializes the Python environment for use within the program.
 */
void init_python_environment();
/**
 * Cleans up the Python environment once it is no longer needed.
 */
void clean_python_environment();

#endif