#include <stdio.h>
#include <Python.h>

/**
 * Receives the following arguments:
 *      -> Path of the python executable
 *		-> Path to the image to be converted
 *		-> Path to the file where the array will be stored
 */
int main(int argc, char *argv[])
{

    if (argc < 4) {
        printf("Usage: Missing one or more arguments\n");
        return 1;
    }

    // Initialize the Python environment
    Py_Initialize();

    // Prepare the Python script arguments
    PyObject *sys_path = PySys_GetObject("path"); // Get the sys.path
    PyList_Append(sys_path, PyUnicode_FromString(".")); // Add current directory

    // Prepare sys.argv
    PyObject *sys_argv = PyList_New(argc - 1); // Create sys.argv list (excluding C's program name)
    for (int i = 1; i < argc; i++) {
        PyList_SetItem(sys_argv, i - 1, PyUnicode_FromString(argv[i]));
    }
    PySys_SetObject("argv", sys_argv); // Set sys.argv in Python

    // Open and execute the Python file
    FILE* fp = fopen(argv[1], "r");
    if (fp != NULL) {
        PyRun_SimpleFile(fp, argv[1]);
        fclose(fp);
    } else {
        printf("Error opening Python file: %s\n", argv[1]);
    }

    // Clean up the Python environment
    Py_Finalize();

    return 0;
}