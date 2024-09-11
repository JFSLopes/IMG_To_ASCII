#ifndef ASCII_RENDER_HPP
#define ASCII_RENDER_HPP

#include "Image.hpp"
#include <memory>

class ASCII_Render{
private:
    std::unique_ptr<Image> img;

    /**
     * This method removes the extension from 'file_path' so the path and name can be used to store the needed auxiliar files
     */
    std::string remove_extension(const std::string& file_path) const;
    /**
     * This method calls the python script that reads image 'file_path' and stores the pix map in 'output_file_path'
     */
    bool call_python_script_read_png(const std::string& file_path, const std::string& output_file_path) const;
    /**
     * This method calls the python script that reads the array from 'file_path' and stores the png in 'output_file_path'
     */
    bool call_python_script_save_png(const std::string& file_path, const std::string& output_file_path) const;
public:
    /**
     * This method runs a loop that asks for image to convert to ascii art
     */
    void run();
};

#endif