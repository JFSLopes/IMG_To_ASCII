#include "../header/ASCII_Render.hpp"
#include <iostream>
#include <stdio.h>
#include "../header/Python_Environment.hpp"

std::string ASCII_Render::remove_extension(const std::string& file_path) const{
    std::string possible_file_extensions[3] = {".png", ".jpeg", ".jpg"};
    std::string ans;
    for (const std::string& extension : possible_file_extensions){
        std::size_t pos = file_path.find(extension);
        if (pos != std::string::npos){
            ans = file_path.substr(0, pos);
            break;
        }
    }
    return ans;
}

void ASCII_Render::run(){
    init_python_environment();
    while(true){
        std::string file_path;
        std::cout << "Enter the file path: ";
        std::cin >> file_path;

        std::string output_file = remove_extension(file_path);
        if (output_file.empty()){
            std::cout << "Image file as an invalid extension.\n";
            continue;
        } else {
            output_file += ".txt";
        }

        // Call the script that reads the image
        if (!call_python_script_read_png(file_path, output_file)){
            continue;
        }

        img = std::make_unique<Image>(output_file);
        if (img->loading_failed()){
            std::cout << "Image path invalid or image is corrupted.\n";
            continue;
        }

        // Show the black and white version of the image
        std::string array_grayscale_file = remove_extension(file_path) + ".csv";
        if (!img->save_grayscale(array_grayscale_file)){
            std::cout << "Failed to open " << array_grayscale_file << "\n";
            continue;
        }
        std::string png_grayscale_file = remove_extension(file_path) + "-grayscale.png";

        if (!call_python_script_save_png(array_grayscale_file, png_grayscale_file)){
            std::cout << "Failed to store the grayscale file into " << png_grayscale_file << "\n";
            continue;
        }
        
        // Convert the image to a grayscale and resizes it
        img->convert_grayscale_to_index();

        // Print the image on terminal and saves it on ascii_art.txt
        img->show_image();

        std::cout << "Do you want to leave? [y/n]: ";
        std::string ans;
        std::cin >> ans;
        if (ans == "y"){
            break;
        }
    }
    clean_python_environment();
}

bool ASCII_Render::call_python_script_read_png(const std::string& file_path, const std::string& output_file_path) const{
    int argc = 3;
    const char* argv[3] = {"Scripts/png_to_array.py", file_path.c_str(), output_file_path.c_str()};

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

    return true;
}

bool ASCII_Render::call_python_script_save_png(const std::string& file_path, const std::string& output_file_path) const{
    int argc = 3;
    const char* argv[3] = {"Scripts/array_to_png.py", file_path.c_str(), output_file_path.c_str()};

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

    return true;
}