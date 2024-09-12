# ASCII Art Image and Video Converter

This project converts images and videos into ASCII art, displaying them in the terminal or saving them as a file. It uses configurable settings for both photo and video modes, offering flexibility in output dimensions and character sets.

## Overview

The application reads images or video frames, converts them to grayscale, and maps pixel brightness to ASCII characters, ranging from darkest to lightest. The resulting ASCII art can be printed directly in the terminal or saved to `ascii_art.txt`. For videos, it processes frames in real-time, displaying them at a specified frame rate.

## Features

- **Configurable operation mode** (photo or video) through `config_mode.ini`.
- **Grayscale conversion**: Converts RGB images to grayscale to simplify the ASCII rendering process.
- **ASCII character mapping**: Uses a sequence of characters, ordered by brightness, to recreate the image or video in ASCII form.
- **Video processing**: Extracts frames from videos using OpenCV and converts them to ASCII at the specified frame rate.
- **Flexible image resizing**: Set custom dimensions for image output.
- **Terminal output and file saving**: ASCII art is printed in the terminal and saved to a file.

## How It Works

1. **Configuration files**:
   - Users configure settings in two `.ini` files:
     1. `config_mode.ini`: Controls whether the program operates in photo or video mode and provides additional settings.
     2. `config_output.ini`: Specifies image dimensions and the ASCII characters used for rendering.

2. **Image Processing**:
   - Images are read via a Python script, which converts the image into an RGB array and writes it to a text file.
   - The C++ program processes this text file, converting the RGB data to grayscale, then mapping it to ASCII characters.
   
3. **Video Processing**:
   - For video files, frames are extracted using OpenCV and treated as static images. These frames are converted to ASCII art and displayed at the configured frame rate (fps).

4. **User Interaction**:
   - After displaying an image or video, the user is prompted to either continue or exit. Changing the configuration files between operations allows different outputs for each run.

## Configuration Files

### 1. `config_mode.ini`
This file controls the application mode and other optional settings like saving auxiliary files or generating a grayscale image. Example:

```
[Mode]
operation_mode = <"Photo" or "Video">

[File]
file_name = <path to image or video>

[Extra]
save_black_white_image = <0 or 1, generates a grayscale image for debugging purposes>
remove_auxiliar_files = <0 or 1, cleans up auxiliary files like the text file storing the pixel map>

[FPS]
fps = <value, only used in video mode>
```

You can find the `config_mode.ini` file in the root directory [here](./config_mode.ini).

### 2. `config_output.ini`
This file defines the settings for resizing the output and the characters used in the ASCII art. Example:

```
[Settings]
resize_width = <desired width> resize_height = <desired height>

[Characters]
characters = <sequence of ASCII characters from darkest to lightest>
```

The ASCII characters are selected to best match the brightness levels of the grayscale pixels. The more space a character takes up, the brighter it appears in the output. You can find the `config_output.ini` file in the root directory [here](./config_output.ini).

## Grayscale Conversion in ASCII Art

Grayscale conversion is crucial for generating accurate ASCII art. Each pixel in an image has a brightness level, represented as a value between 0 (black) and 255 (white). The program converts these brightness values to a sequence of ASCII characters.

The ASCII characters are chosen because:
- **Darker characters** like `.` or `:` take up less visual space, representing darker areas.
- **Brighter characters** like `@` or `#` occupy more space and are used for lighter areas.

This mapping allows the ASCII art to represent the tonal variations of the original image.

## Program Flow

1. **Image/Video Reading**: The Python script reads the image or video file and outputs a text file containing pixel data.
   - The pixel data is formatted as:
     ```
     <number of lines> <number of columns>
     <Each pixel’s RGB components, separated by spaces> (one row per line)
     ```

2. **ASCII Conversion**: The C++ program reads the pixel data, converts the RGB values to grayscale, and maps each grayscale value to a corresponding ASCII character.

3. **Output**: 
   - The resulting ASCII art is printed in the terminal and saved to a file called `ascii_art.txt`.
   - For videos, this file is updated with each frame, allowing you to see the ongoing output.

4. **Looping**: After processing an image or video, the program asks whether to continue. If "continue" is selected, it reloads the configuration files, so you can modify the settings for the next run.

## Viewing in the Terminal

The terminal has size limitations (around 85 characters per line). If the output dimensions are too large, zoom out in your terminal to fit the entire image. For video, since the ASCII art file (`ascii_art.txt`) is constantly overwritten with each frame, you can observe the video output in that file.

## Requirements

- **OpenCV**: Required for extracting frames from video files.
- **Python**: Used for reading image files (png/jpeg/jpg) and converting them to arrays.
   - The Python script is located here: [Scripts/png_to_array.py](./Scripts/png_to_array.py).

## Makefile

The provided `Makefile` is configured for Python 3.11. If you are using a different version of Python, you may need to adjust the `CXXFLAGS` and `LDFLAGS` to match your Python environment.

## Dependencies

- **OpenCV**: Used for video frame extraction.
- **Python**: Used for image file reading and conversion.

## Example Workflow

1. Configure `config_mode.ini` for photo or video mode.
2. Set output dimensions and characters in `config_output.ini`.
3. Run the program to generate ASCII art.
4. Modify the configuration files and choose to continue for a different output.

## Improvements

- **Character Sets**: You can experiment with different character sets to achieve varying levels of detail in the ASCII art.