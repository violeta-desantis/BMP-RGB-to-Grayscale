# RGB to Grayscale .BMP File Converter

A tool that converts BMP images from RGB to grayscale, written in C.

Supports only 24-bit color depth, the BITMAPINFOHEADER type, and no file compression - read more here:

https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header)

Example usage:

> ./gray filename.bmp
```
[see grayscale output in filename_gs.bmp]
```

EDIT: Just for fun, I made an extremely shortened version of this code - which you can see here:
https://github.com/violeta-desantis/CodeGolf/tree/main/bmp_gray

(Warning: not for the faint of heart!)