#ifndef ASCII_RENDER_HPP
#define ASCII_RENDER_HPP

#include "Image.hpp"
#include <memory>


class ASCII_Render{
private:
    std::unique_ptr<Image> img;

    std::string swap_extension_to_txt(const std::string& file_path) const;
    bool call_python_script(const std::string& file_path, const std::string& output_file_path) const;
public:
    void run();

};

#endif