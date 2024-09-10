#include "../header/ASCII_Render.hpp"
#include <iostream>
#include <stdio.h>
#include <Python.h>

std::string ASCII_Render::swap_extension_to_txt(const std::string& file_path) const{
    std::string possible_file_extensions[3] = {".png", ".jpeg", ".jpg"};
    std::string ans;
    for (const std::string& extension : possible_file_extensions){
        std::size_t pos = file_path.find(extension);
        if (pos != std::string::npos){
            ans = file_path.substr(0, pos) + ".txt";
            break;
        }
    }
    return ans;
}

void ASCII_Render::run(){
    while(true){
        std::string file_path;
        std::cout << "Enter the file path: ";
        std::cin >> file_path;

        std::string output_file = swap_extension_to_txt(file_path);
        if (output_file.empty()){
            std::cout << "Image file as an invalid extension.\n";
            continue;
        }
        std::cout << "File: " << output_file << "\n";

        // Call the script that reads the image
        if (!call_python_script(file_path, output_file)){
            continue;
        }

        img = std::make_unique<Image>(output_file);
        if (img->loading_failed()){
            std::cout << "Image path invalid or image is corrupted.\n";
        }

        // PRINT THE IMAGE IN THE CONSOLE

        // Wait until 'Enter' is pressed
        std::getline(std::cin, file_path);

        std::cout << "Do you want to leave? [y/n]: ";
        std::string ans;
        std::cin >> ans;
        if (ans == "y"){
            break;
        }
    }
}

bool ASCII_Render::call_python_script(const std::string& file_path, const std::string& output_file_path) const{
    int argc = 3;
    const char* argv[3] = {"Scripts/png_to_array.py", file_path.c_str(), output_file_path.c_str()};
    // Initialize the Python environment
    Py_Initialize();

    // Prepare the Python script arguments
    PyObject *sys_path = PySys_GetObject("path"); // Get the sys.path
    PyList_Append(sys_path, PyUnicode_FromString(".")); // Add current directory

    // Prepare sys.argv
    PyObject *sys_argv = PyList_New(argc); // Create sys.argv list
    for (int i = 0; i < argc; i++) {
        PyList_SetItem(sys_argv, i, PyUnicode_FromString(argv[i]));
    }
    PySys_SetObject("argv", sys_argv); // Set sys.argv in Python

    // Open and execute the Python file
    FILE* fp = fopen(argv[0], "r");
    if (fp != NULL) {
        PyRun_SimpleFile(fp, argv[0]);
        fclose(fp);
    } else {
        std::cout << "Error opening Python file: " << argv[0] << "\n";
        return false;
    }

    // Clean up the Python environment
    Py_Finalize();
    return true;
}