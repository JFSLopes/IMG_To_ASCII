import argparse
import numpy as np
from PIL import Image

def grayscale_to_image(input_file, output_file):
    # Load the grayscale values from csv (or other compatible formats)
    grayscale_array = np.loadtxt(input_file, delimiter=',', dtype=np.uint8)

    # Create an image from the grayscale values
    image = Image.fromarray(grayscale_array, mode='L')

    # Save the image to the specified output file
    image.save(output_file)

    print(f"Grayscale image saved to {output_file}")

if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Convert grayscale values to a PNG image.")
    parser.add_argument('input_file', type=str, help="Path to the grayscale array file (.csv).")
    parser.add_argument('output_file', type=str, help="Path to save the output PNG image.")

    # Parse the arguments
    args = parser.parse_args()

    # Call the function with the provided input and output paths
    grayscale_to_image(args.input_file, args.output_file)
