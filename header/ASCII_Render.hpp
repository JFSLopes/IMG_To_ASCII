#ifndef ASCII_RENDER_HPP
#define ASCII_RENDER_HPP

#include "Image.hpp"
#include <memory>

enum Mode {PHOTO, VIDEO};

struct Config_Mode{
    std::string file_path;
    Mode mode;
};

class ASCII_Render{
private:
    std::unique_ptr<Image> img;
    Config_Mode config;

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
    /**
     * This method reads the config mode file, so it knows if is going to receive a video or an image
     */
    bool read_config_mode();
public:
    /**
     * This method runs a loop that asks for image to convert to ascii art
     */
    void run();
};

#endif