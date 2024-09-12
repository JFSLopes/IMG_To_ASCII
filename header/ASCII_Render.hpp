#ifndef ASCII_RENDER_HPP
#define ASCII_RENDER_HPP

#include "Image.hpp"
#include <memory>

enum Mode {
    PHOTO,  /**< Mode for processing a single image. */
    VIDEO   /**< Mode for processing a video. */
};

struct Config_Mode{
    std::string file_path;        /**< Path to the input file (image or video). */
    Mode mode;                    /**< Operation mode (PHOTO or VIDEO). */
    bool save_black_white_image;  /**< Flag indicating whether to save a black-and-white version of the image. */
    bool remove_auxiliar_files;   /**< Flag indicating whether to remove auxiliary files after processing. */
    int fps;                      /**< Frames per second to be used when processing a video. */
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
    /**
     * Function that controls the conversion of an image to ascii art.
     */
    bool run_for_photo();
    /**
     * Function that controls the conversion of a video to ascii art.
     */
    bool run_for_video();
public:
    /**
     * This method runs a loop that asks for image or video to convert to ascii art
     */
    void run();
};

#endif