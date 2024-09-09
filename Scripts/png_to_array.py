import argparse
from PIL import Image
import numpy as np

# This file will read a PNG image and convert it to an RGB structure with one byte per component.
# It will then write the array to the specified output file. 
# The file starts with the width and height on the first line, followed by the pixel data, with one row per line.
# The image and output file must be passed through the command line

def image_to_rgb_array(image_path, output_file):
    # Load the image
    image = Image.open(image_path)
    
    # Convert all images to RGB
    image = image.convert('RGB')
    
    # Convert image to a numpy array
    pixel_array = np.array(image)

    height, width, _ = pixel_array.shape
    
    # Save the RGB array to the specified output file
    with open(output_file, 'w') as f:

        f.write(f"{width} {height}\n")
        
        for row in pixel_array:
            for pixel in row:
                f.write(f"{pixel[0]} {pixel[1]} {pixel[2]} ")
            f.write('\n')

    print(f"RGB pixel array saved to {output_file}")

if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Convert an image to RGB pixel array.")
    parser.add_argument('image_path', type=str, help="Path to the image file.")
    parser.add_argument('output_file', type=str, help="Output file for saving the RGB pixel array.")
    
    # Parse the arguments
    args = parser.parse_args()
    
    # Call the function with the provided image path and output file
    image_to_rgb_array(args.image_path, args.output_file)
