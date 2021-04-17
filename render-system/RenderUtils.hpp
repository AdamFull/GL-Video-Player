#ifndef __SE_UTILS__
#define __SE_UTILS__

#include "stdint.h"
#include <random>

/*****************************************************************************************/
typedef struct{
    float r, g, b;
} Color;

typedef struct{
    float x, y, z;
} Position;

typedef struct{
    Position pos;
    Color color;
} Pixel;


/*****************************************************************************************/
/**************************************Static utils***************************************/
/*****************************************************************************************/
static float getRandomFloat(){
    union UNION {
        uint32_t i;
        float f;
    } r;
    r.i = (rand() & 0xffffff) + ((rand() & 0x3fffff) << 16);
    return r.f;
}

/*****************************************************************************************/
static Color RGBtoColor(float r, float g, float b){
    Color color = {r, g, b};
    return color;
}

/*****************************************************************************************/
static Color getRandomColor(){
    return RGBtoColor(getRandomFloat(), getRandomFloat(), getRandomFloat());
}

/*****************************************************************************************/
static bool isColorsSame(Color col1, Color col2){
    if(col1.r != col2.r) return false;
    else if(col1.g != col2.g) return false;
    else if(col1.b != col2.b) return false;
    else return true;
}

//Color definitions
static Color CL_BLACK()   { return RGBtoColor(0.0f, 0.0f, 0.0f); }
static Color CL_WHITE()   { return RGBtoColor(1.0f, 1.0f, 1.0f); }
static Color CL_RED()     { return RGBtoColor(1, 0.0f, 0.0f); }
static Color CL_GREEN()   { return RGBtoColor(0.0f, 1.0f, 0.0f); }
static Color CL_BLUE()    { return RGBtoColor(0.0f, 0.0f, 1.0f); }
static Color CL_GREY()    { return RGBtoColor(0.5f, 0.5f, 0.5f); }
static Color CL_YELLOW()  { return RGBtoColor(1.0f, 1.0f, 0.0f); }
static Color CL_MAGENTA() { return RGBtoColor(0.55f, 0.0f, 0.55f); }
static Color CL_PINK()    { return RGBtoColor(1.0f, 0.43f, 0.71f); }

#endif