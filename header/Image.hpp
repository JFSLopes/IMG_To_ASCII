#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <opencv2/opencv.hpp>
#include <iostream>

/**
 * This structure holds the red, green, and blue (RGB) components of a pixel.
 */
struct Pixel{
    uint8_t r; /**< Red channel (0-255). */
    uint8_t g; /**< Green channel (0-255). */
    uint8_t b; /**< Blue channel (0-255). */
};

/**
 * This structure contains settings such as the desired resized dimensions for the image,
 * the number of characters to use in the ASCII art, and the list of characters for ASCII representation.
 */
struct Config_Output{
    uint16_t resize_w;            /**< Desired width of the resized image. */
    uint16_t resize_h;            /**< Desired height of the resized image. */
    uint16_t num_characters;      /**< Number of characters to use for ASCII art. */
    std::vector<char> characters; /**< A list of characters ordered by intensity for ASCII art. */
};

/**
 * This function reads the red, green, and blue (RGB) values of a Pixel from an input stream.
 * It expects the RGB values to be provided in a specific format (space-separated values).
 */
std::istream& operator>>(std::istream& in, Pixel& p);

class Image{
private:
    Pixel** pix_map;          /**< 2D array of Pixels representing the RGB image data. */
    uint8_t** grayscale;      /**< 2D array representing the grayscale version of the image. Each pixel is a value from 0 (black) to 255 (white). */
    uint16_t w;               /**< Image width in pixels. */
    uint16_t h;               /**< Image height in pixels. */
    Config_Output config;     /**< Configuration settings for image resizing and ASCII generation. */
    bool flag_error = false;  /**< Flag to indicate if an error occurred during config or pixel map loading. */

    /**
     * This function reads the dimensions from the pixmap file
     */
    bool read_dimensions(std::ifstream& in);

    /**
     * This function reads the pixels from the pixmap file
     */
    bool read_pix_map(std::ifstream& in);

    /**
     * This function commands how the pixmap file is read and, in case of an error, sets flag_error to true.
     */
    void read_file(const std::string& file);

    /**
     * This function reads the config related to the resize of the image from the config.ini
     */
    void read_config_file(const std::string& file);

    /**
     * Allocates memory for a 2D array of the given dimensions (lines x columns).
     */

    template<class T>
    T** allocate_array(uint16_t lines, uint16_t cols);
    /**
     * Allocates memory for the 2D array of Pixels based on the current image dimensions.
     */
    bool allocate_pix_map();

    /**
     * Allocates memory for the 2D array of grayscale values (uint8_t) based on the current image dimensions.
     */
    bool allocate_grayscale();

    /**
     * Converts the RGB image data (pix_map) to grayscale (grayscale array) using the NTSC standard:
     * grayscale = 0.299 * R + 0.587 * G + 0.114 * B.
     */
    void make_image_grayscale_NTSC();

    /**
     * Converts a RGB pixel to grayscale value using the NTSC standard:
     * grayscale = 0.299 * R + 0.587 * G + 0.114 * B.
     */
    uint8_t rgb_to_grayscale(uint8_t r, uint8_t g, uint8_t b) const;

    /**
     * Resizes the grayscale version of the image to match the width and height specified in the `config` object.
     * The resize is done by averaging pixel blocks based on the new dimensions.
     */
    void resize_image();

    /**
     * Computes the average grayscale value of a rectangular region within the image, defined by
     * the starting line, column, width, and height.
     */
    uint8_t get_average(uint16_t line, uint16_t col, uint16_t w, uint16_t h) const;

    /**
     * Frees the memory allocated for a 2D array of type T. This function ensures that all rows
     * and the array itself are properly deallocated.
     */
    template<class T>
    void deallocate_2d_array(T** array, uint16_t num_lines) const;


public:
    Image(const std::string& file, bool is_photo);
    ~Image();

    /**
     * This function returns `true` if the image loading or configuration reading failed, 
     * as indicated by the `flag_error`. Otherwise, it returns `false`.
     */
    bool loading_failed() const;

    /**
     * This function outputs ascii art to the console and to a .txt file called ascii_art_txt.
     */
    void show_ascii_art() const;

    /**
     * This function writes the grayscale pixel data to a specified file path in a suitable format (CSV),
     * one row per line
     */
    bool save_grayscale(const std::string& file_path) const;

    /**
     * This function converts the grayscale values in the image to corresponding index values,
     * mapping them to ASCII characters based on their intensity.
     */
    void convert_grayscale_to_index();

    /**
     * This function takes an OpenCV Mat (RGB frame), extracts its pixel data,
     * and stores the RGB values into the Image class's pix_map (Pixel**) array. It also 
     * converts the RGB values into grayscale.
     */
    void store_opencv_array_pix_map(const cv::Mat& frame);

    /**
     * This function updates the dimensions (w and h) of the Image object based on 
     * the width and height of the provided OpenCV Mat. It ensures that subsequent 
     * operations on the Image object use the correct dimensions.
     */
    void store_video_dimensions(const cv::Mat& frame);
};

#endif