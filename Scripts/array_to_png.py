import argparse
import numpy as np
from PIL import Image
import os

def grayscale_to_image(input_file):
    # Determine the output file name based on the input file
    base_name = os.path.splitext(input_file)[0]
    output_file = f"{base_name}-grayscale.png"

    # Load from csv
    grayscale_array = np.loadtxt(input_file, delimiter=',', dtype=np.uint8)

    # Create image from grayscale values
    image = Image.fromarray(grayscale_array, mode='L')

    image.save(output_file)

    print(f"Grayscale image saved to {output_file}")

if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Convert grayscale values to a PNG image.")
    parser.add_argument('input_file', type=str, help="Path to the grayscale array file (.txt or .csv).")
    
    # Parse the arguments
    args = parser.parse_args()

    grayscale_to_image(args.input_file)
