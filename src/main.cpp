#include <stdio.h>
#include <Python.h>
#include "../header/ASCII_Render.hpp"

/**
 * Receives the following arguments:
 *      -> Path of the python executable
 *		-> Path to the image to be converted
 *		-> Path to the file where the array will be stored
 */
int main(int argc, char *argv[])
{
    ASCII_Render render;
    render.run();
    return 0;
}