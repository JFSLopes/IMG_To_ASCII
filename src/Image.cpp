#include "../header/Image.hpp"
#include <iostream>
#include <algorithm>

std::istream& operator>>(std::istream& in, Pixel& p){
    if (in){
        uint16_t r, g, b;

        in >> r >> g >> b;

        p.r = static_cast<uint8_t>(r);
        p.g = static_cast<uint8_t>(g);
        p.b = static_cast<uint8_t>(b);
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, const Pixel& p){
    os << (int)p.r << ' ' << (int)p.g << ' ' << (int)p.b;
    return os;
}

Image::Image(const std::string& file) : file(std::move(file)){
    read_file();
}

Image::~Image(){
    if (pix_map != nullptr){
        for (uint16_t l = 0; l < w; l++){
            delete [] pix_map[l];
        }

        delete [] pix_map;
    }

    delete [] grayscale;
}

bool Image::read_dimensions(std::ifstream& in){
    in >> w;
    in >> h;
    return !in.fail();
}

bool Image::read_pix_map(std::ifstream& in){
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            in >> pix_map[l][c];
            if (in.fail()){
                // Make sure is not in the end of the file or that a pixel is not represented correcly (with letters for instance)
                return false;
            }
        }
    }

    return true;
}

bool Image::allocate_pix_map(){
    pix_map = new Pixel*[h];
    if (pix_map == nullptr) {
        return false; 
    }

    for (uint16_t l = 0; l < h; l++){
        pix_map[l] = new Pixel[w];
        if (pix_map[l] == nullptr) {
            return false;
        }
    }

    return true;
}

uint8_t** Image::allocate_new_grayscale(uint16_t new_w, uint16_t new_h){
    uint8_t** new_grayscale = new uint8_t*[new_h];
    if (new_grayscale == nullptr){
        return nullptr;
    }

    for (uint16_t l = 0; l < new_h; l++){
        new_grayscale[l] = new uint8_t[new_w];
        if (new_grayscale[l] == nullptr) {
            return nullptr;
        }
    }

    return new_grayscale;
}

bool Image::allocate_grayscale(){
    grayscale = new uint8_t*[h];
    if (grayscale == nullptr){
        return false;
    }

    for (uint16_t l = 0; l < h; l++){
        grayscale[l] = new uint8_t[w];
        if (grayscale[l] == nullptr) {
            return false;
        }
    }

    return true;
}

void Image::read_file(){
    std::ifstream in(file);

    if (!in.is_open()){
        flag_error = true;
        return;
    }

    if (!read_dimensions(in) or !allocate_pix_map() or !read_pix_map(in)){
        flag_error = true;
        return;
    }

    if (allocate_grayscale()){
        make_image_grayscale_NTSC();
    } else {
        flag_error = true;
    }
    
    in.close();
}

void Image::show_image() const{
    std::cout << "Width: " << w << "    Heigth: " << h << "\n";
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            std::cout << (uint16_t)grayscale[l][c] << " ";
        }
        std::cout << "\n";
    }
}

bool Image::loading_failed() const{
    return flag_error;
}

void Image::make_image_grayscale_NTSC(){
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            uint8_t value = (float) pix_map[l][c].r * 0.299 + (float) pix_map[l][c].g * 0.587 + (float) pix_map[l][c].b * 0.114;
            grayscale[l][c] = value;
        }
    }
}

bool Image::save_grayscale(const std::string& file_path) const{
    std::ofstream os(file_path);

    if (!os.is_open()){
        return false;
    }

    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            os << (uint16_t) grayscale[l][c];
            if (c != w - 1) os << ",";
        }
        os << "\n";
    }
    os.close();
    return true;
}

char characters[9] = {' ', '.', ':', '-', '+', '*', '$', '@', '#'};

void Image::convert_grayscale_to_index(){
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            grayscale[l][c] /= 29;
        }
    }

    resize_image(200, 100);

    std::ofstream os("final.txt");

    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            os << characters[grayscale[l][c]];      
        }
        os << "\n";
    }    
}

void Image::resize_image(uint16_t new_w, uint16_t new_h){
    uint8_t** new_grayscale = allocate_new_grayscale(new_w, new_h);
    if (new_grayscale == nullptr){
        std::cout << "An error happended while resizing image.\n";
        return;
    }

    uint16_t jump_w = w / new_w;
    uint16_t jump_h = h / new_h;
    for (uint16_t l = 0; l < new_h; l++){
        for (uint16_t c = 0; c < new_w; c++){
            new_grayscale[l][c] = get_average(l * jump_h, c * jump_w, jump_w, jump_h);
        }
    }
    // Free the memory before swapping pointers
    dealocate_2d_array(grayscale, h);

    grayscale = new_grayscale;
    w = new_w;
    h = new_h;
}

uint8_t Image::get_average(uint16_t line, uint16_t col, uint16_t width, uint16_t height) const{
    uint16_t sum = 0;
    uint16_t num = 0;
    for (uint16_t l = line; l < std::min(static_cast<uint16_t>(line + height), h); l++){
        for (uint16_t c = col; c < std::min(static_cast<uint16_t>(col + width), w); c++){
            sum += grayscale[l][c];
            num++;
        }
    }
    if (num == 0) return 0;
    return static_cast<uint8_t>(sum / num);
}

void Image::dealocate_2d_array(uint8_t** array, uint16_t num_lines) const{
    if (array != nullptr){
        for (uint16_t i = 0; i < num_lines; i++){
            delete [] array[i];
        }

        delete [] array;
    }
}