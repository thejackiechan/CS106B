/*
 * File: fractals.cpp
 * --------------------------
 * Name: Jackie Chan and Ike Osafo
 * Section leader: Jordan Rosen-Kaplan
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>
#include "complex.h"
#include "grid.h"

using namespace std;

void drawTree(GWindow& gw, double x, double y, double size, int order);
void drawTree(GWindow &gw, double x, double y, double size, int order, int angle, double const &const_size, double const &const_order) ;
const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */

// This function takes in user input (starting coordinates corresponding to the top left corner of the triangle, size, and order)
// and recursively draws equilateral triangles. Invalid inputs result in an exception being thrown. The base case is order 1, in
// which an upside down triangle is drawn. The recursive case recalculates the coordinates and draws higher order triangles within
// the large one.

void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    if(order == 1){
        double x1 = x + size;
        double x2 = x + size/2;
        double y2 = y + size * cosDegrees(30);

        gw.drawLine(x,y,x1,y);
        gw.drawLine(x,y,x2,y2);
        gw.drawLine(x1,y,x2,y2);
    }else if(order < 1 || x < 0.0 || y < 0.0 || size < 0.0){
        throw("Please enter a positive value for x, y, size, and order.");
    }else{
        double h = size * cosDegrees(30);
        double x1 = x + size/2;
        double x2 = x + size/4;
        double y2 = y + h/2;
        double x3 = x + 3 * size/4;

        gw.drawLine(x1,y,x2,y2);
        gw.drawLine(x1,y,x3,y2);
        gw.drawLine(x2,y2,x3,y2);
        drawSierpinskiTriangle(gw,x,y,size/2,order - 1);
        drawSierpinskiTriangle(gw,x1,y,size/2,order - 1);
        drawSierpinskiTriangle(gw,x2,y2,size/2,order - 1);
    }
}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */

// This function passes in the parameters related to the order 1 Tree and calls the recursive function with these parameters.
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    int angle = 90;
    double x1 = x + size/2;
    double y1 = y + size;
    double const_order = order;
    double const_size = size;
    drawTree(gw, x1, y1, size/2, order, angle, const_size, const_order);
}

void drawTree(GWindow &gw, double x, double y, double size, int order, int angle, const double &const_size, const double &const_order){
    if(order == 0){ // base case when order == 0, draw nothing
        return;
    }else{
        if(size == const_size/pow(2.0,const_order)){ // sets color for the leaves at the end of the tree
            gw.setColor(LEAF_COLOR);
        }else{
            gw.setColor(BRANCH_COLOR); // sets the rest of the tree to brown
        }
        gw.drawPolarLine(x, y, size, angle);
        x -= size*cosDegrees(180 - angle); // calculates the positions of the next branching point
        y -= size*sinDegrees(180 - angle);
        angle -= 45;
        for(int i = 0; i < 7; i++){
            drawTree(gw, x, y, size/2, order - 1, angle, const_size, const_order); // draws subsequent branches (7 of them)
            angle += 15;
        }
    }
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */

// This function is responsible for calling the overloaded (non-recursive function) which in turn calls the recursive one to
// ultimately determine the value of iterations. If this value is equal to the maxIterations value passed in, then the pixel is
// the same color as the one passed in; otherwise, there is a formula that recolors that pixel.

void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];

    Vector<int> palette = setPalette();
    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    for(int r = 0; r < pixels.numRows(); r++){
        for(int c = 0; c < pixels.numCols(); c++){
            Complex check = Complex(minX + c * incX, minY + r * incY);
            int iterations = mandelbrotSetIterations(check, maxIterations);
            if(iterations == maxIterations){
                pixels[r][c] = color;
            }else{
                pixels[r][c] = palette[iterations % palette.size()];
            }
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */

//This function calls the recursive function that check whether a complex number is in MandelBrot set. It also updates
//remainingIterations for use in the main function.
int mandelbrotSetIterations(Complex c, int maxIterations) {
    Complex z = Complex(0.0,0.0);
    int remainingIterations = mandelbrotSetIterations(z,c,maxIterations);

    remainingIterations = maxIterations - remainingIterations;
    return remainingIterations;
}

/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */

//This is our recursive function that checks if the magnitude of the complex number is greater than 4.
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    if(z.abs() > 4 || remainingIterations == 0){
        return remainingIterations;
    }else{
        z = z * z + c;
        return mandelbrotSetIterations(z,c,remainingIterations - 1);
    }
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
