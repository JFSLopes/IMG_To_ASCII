- The bit map will be stored in a file in output/ with the same name has the image selected

- The flow is:
    1. A (png/jpg/jpep) named 'X.png' is converted into an array of RGB and stored in the file named 'X.txt'.
    2. The array is read to a Image object and them converted into a grayscale array, stored into 'X.csv'
    3. The black and white image is also stored in 'X-grayscale.png' (it does not serve any purpose)