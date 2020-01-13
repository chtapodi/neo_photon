#include "colors.h"

uint8_t codeArr[11][3] = {
    {255,255,255}, //white
    {255,0,0}, //red
    {0,0,255}, //blue
    {0,255,0}, //green
    {255,100,0}, //yellow
    {0,255,255}, //cyan
    {255,0,50}, //pink
    {255,30,0}, //orange
    {128,0,128}, //purple
    {0,255,20}, //light green
    {255,70,50}

};


std::string colorArr[11]= {
    "white",
    "red",
    "blue",
    "green",
    "yellow",
    "cyan",
    "pink",
    "orange",
    "purple",
    "light green",
    "sun"
};


//These are different ways to display


uint8_t * colors::getRGB(std::string color) {
    int loc =getLoc(color);

    if(loc==-1) {
	   return NULL;
    }
    else {


	   return codeArr[loc];
    }


}



int8_t colors::getLoc(std::string color) {


    for (int i=0; i < 11; i = i + 1 ) {

	   if ( color.compare(colorArr[i])==0 ) {

		  return i;
	   }
    }
    return -1;
}
