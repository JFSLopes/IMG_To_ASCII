#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>
#include <string>
#include <fstream>
#include <istream>
#include <ostream>

struct Pixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

std::istream& operator>>(std::istream& in, Pixel& p);
std::ostream& operator<<(std::ostream& os, const Pixel& p);

class Image{
private:
    Pixel** pix_map;
    uint8_t** grayscale; // One byte pixel represent how much white does a image have (0-255)
    uint16_t w;
    uint16_t h;
    std::string file;
    bool flag_error = false;

    bool read_dimensions(std::ifstream& in);
    bool read_pix_map(std::ifstream& in);
    bool allocate_pix_map();
    bool allocate_grayscale();
    uint8_t** allocate_new_grayscale(uint16_t new_w, uint16_t new_h);
    void read_file();
    void make_image_grayscale_NTSC();
    void resize_image(uint16_t new_w, uint16_t new_h);

    uint8_t get_average(uint16_t line, uint16_t col, uint16_t w, uint16_t h) const;
    void dealocate_2d_array(uint8_t** array, uint16_t num_lines) const;

public:
    Image(const std::string& file);
    ~Image();

    bool loading_failed() const;
    void show_image() const;
    bool save_grayscale(const std::string& file_path) const;
    void convert_grayscale_to_index();
};

#endif