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

Image::Image(const std::string& file){
    read_file(file);
    read_config_file(file);
}

Image::~Image(){
    deallocate_2d_array<Pixel>(pix_map, h);
    deallocate_2d_array<uint8_t>(grayscale, config.resize_h);
}

template<class T>
void Image::deallocate_2d_array(T** array, uint16_t num_lines) const{
    if (array != nullptr){
        for (uint16_t i = 0; i < num_lines; i++){
            delete [] array[i];
        }

        delete [] array;
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

template<class T>
T** Image::allocate_array(uint16_t lines, uint16_t cols){
    T** array = new T*[lines];
    if (array == nullptr){
        return nullptr;
    }

    for (uint16_t l = 0; l < lines; l++){
        array[l] = new T[cols];
        if (array[l] == nullptr) {
            return nullptr;
        }
    }

    return array;
}

bool Image::allocate_pix_map(){
    pix_map = allocate_array<Pixel>(h, w);
    return pix_map != nullptr;
}

bool Image::allocate_grayscale(){
    grayscale = allocate_array<uint8_t>(h, w);
    return grayscale != nullptr;
}

void Image::read_file(const std::string& file){
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

void Image::show_ascii_art() const{
    std::ofstream os("ascii_art.txt");

    for (uint16_t l = 0; l < config.resize_h; l++){
        for (uint16_t c = 0; c < config.resize_w; c++){
            os << config.characters[grayscale[l][c]];
            std::cout << config.characters[grayscale[l][c]];
        }
        std::cout << "\n";
        os << "\n";
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


void Image::convert_grayscale_to_index(){
    uint16_t group_dim = 29;
    for (uint16_t l = 0; l < h; l++){
        for (uint16_t c = 0; c < w; c++){
            grayscale[l][c] /= group_dim;
        }
    }

    resize_image();
}

void Image::resize_image(){
    uint8_t** new_grayscale = allocate_array<uint8_t>(config.resize_h, config.resize_w);
    if (new_grayscale == nullptr){
        std::cout << "An error happended while resizing image.\n";
        return;
    }

    for (uint16_t l = 0; l < config.resize_h; l++) {
        // Calculate the vertical range in the original image
        uint16_t orig_line_start = l * h / config.resize_h;
        uint16_t orig_line_end = (l + 1) * h / config.resize_h;

        for (uint16_t c = 0; c < config.resize_w; c++) {
            // Calculate the horizontal range in the original image
            uint16_t orig_col_start = c * w / config.resize_w;
            uint16_t orig_col_end = (c + 1) * w / config.resize_w;

            // Compute the average in the range
            new_grayscale[l][c] = get_average(orig_line_start, orig_col_start, orig_col_end - orig_col_start, orig_line_end - orig_line_start);
        }
    }
    // Free the memory before swapping pointers
    deallocate_2d_array(grayscale, h);

    grayscale = new_grayscale;
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

void Image::read_config_file(const std::string& file){
    std::ifstream in("config_output.ini");
    if (!in.is_open()){
        std::cout << "Couldn't open file \"config.ini\"\n";
        flag_error = true;
        return;
    }

    std::string name, equal, value;
    // Read resize dimensions
    in >> name;
    if (name != "[Settings]"){
        std::cout << "Invalid config.ini file structure. Expected [Settings].\n";
        flag_error = true;
        return;
    }
    in >> name >> equal >> value;
    config.resize_w = (uint16_t) std::stoi(value);

    in >> name >> equal >> value;
    config.resize_h = (uint16_t) std::stoi(value);


    // Read characters
    in >> name;
    if (name != "[Characters]"){
        std::cout << "Invalid config.ini file structure. Expected [Characters].\n";
        flag_error = true;
        return;
    }
    in >> name >> equal;
    std::getline(in, value);

    value = value.substr(2, value.size() - 3);
    config.num_characters = (uint16_t) value.size();
    for (const char ch : value) config.characters.push_back(ch);

    in.close();
}