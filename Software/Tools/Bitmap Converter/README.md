Bitmap Converter is a standalone utility which converts a PPM image into a uint8_t array which the graphics library then uses to display on the screen.

You can use a tool such as GIMP (GNU Image Manipulation Program) or other image editors to generate the PPM file format.

To convert a PPM file to a C file, use the following command

pnmtoc -c <source.ppm> > <dest.c>

So, for a bitmap called "avatar.ppm", you would use

pnmtoc -c avatar.ppm > avatar.c

https://www.gimp.org/