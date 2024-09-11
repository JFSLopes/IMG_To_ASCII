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

bool ASCII_Render::read_config_mode(){
    std::ifstream in("config_mode.ini");
    if (!in.is_open()){
        std::cout << "Could open config_mode.ini.\n";
        return false;
    }

    std::string name, equal, value;
    // Read mode
    in >> name;
    if (name != "[Mode]"){
        std::cout << "Invalid config_mode.ini file structure. Expected [Mode].\n";
        return false;
    }
    in >> name >> equal >> value;
    if (value == "\"Photo\""){
        config.mode = PHOTO;
    }
    else if (value == "\"Video\""){
        config.mode = VIDEO;
    } else {
        std::cout << "Invalid Mode. Expected Photo or Video.\n";
        return false;
    }

    // Read file name
    in >> name;
    if (name != "[File]"){
        std::cout << "Invalid config_mode.ini file structure. Expected [File].\n";
        return false;
    }
    in >> name >> equal >> value;

    in >> name >> equal >> value;
    config.file_path = value.substr(1, value.size() - 2);
    return true;
}

void ASCII_Render::run(){
    init_python_environment();

    while(true){
        // Read the config
        if (!read_config_mode()){
            std::cout << "Check if there is a file config_mode.ini in the project root and if it is valid.\n";
            break;
        }

        std::string output_file = remove_extension(config.file_path);
        if (output_file.empty()){
            std::cout << "Image file as an invalid extension.\n";
            continue;
        } else {
            output_file += ".txt";
        }

        // Call the script that reads the image
        if (!call_python_script_read_png(config.file_path, output_file)){
            continue;
        }

        img = std::make_unique<Image>(output_file);
        if (img->loading_failed()){
            std::cout << "Image path invalid or image is corrupted.\n";
            continue;
        }

        // Show the black and white version of the image
        std::string array_grayscale_file = remove_extension(config.file_path) + ".csv";
        if (!img->save_grayscale(array_grayscale_file)){
            std::cout << "Failed to open " << array_grayscale_file << "\n";
            continue;
        }
        std::string png_grayscale_file = remove_extension(config.file_path) + "-grayscale.png";

        if (!call_python_script_save_png(array_grayscale_file, png_grayscale_file)){
            std::cout << "Failed to store the grayscale file into " << png_grayscale_file << "\n";
            continue;
        }
        
        // Convert the image to a grayscale and resizes it
        img->convert_grayscale_to_index();

        // Clear the terminal
        system("clear");

        // Print the image on terminal and saves it on ascii_art.txt
        img->show_ascii_art();

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