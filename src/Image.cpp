#include "../header/Image.hpp"
#include <iostream>

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
    pix_map = new Pixel*[w];
    if (pix_map == nullptr) {
        return false; 
    }

    for (uint16_t l = 0; l < w; l++){
        pix_map[l] = new Pixel[h];
        if (pix_map[l] == nullptr) {
            return false;
        }
    }

    return true;
}

void Image::read_file(){
    std::ifstream in(file);

    if (!in.is_open()){
        flag_error = false;
        return;
    }

    if (!read_dimensions(in) or !allocate_pix_map() or !read_pix_map(in)){
        flag_error = false;
    }
    
    in.close();
}

void Image::show_image() const{
    std::cout << "Width: " << w << "    Heigth: " << h << "\n";
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            std::cout << pix_map[l][c] << " ";
        }
        std::cout << "\n";
    }
}

bool Image::loading_failed() const{
    return flag_error;
}