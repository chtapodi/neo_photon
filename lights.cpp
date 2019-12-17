#include "lights.h"
#include <neopixel.h>
#include <math.h>
#include <bitset>

#define PIXEL_COUNT 300

uint16_t j;
uint8_t speed, amult;

char debugVal[50];
int debugInt=-1;
// int** glimmerGoals;
int pixelCount=PIXEL_COUNT;
// int *glimmerGoals; //I may need to change the type with the nested arrays?


//constructor
lights::lights(uint16_t n, uint8_t p, uint8_t t) : Adafruit_NeoPixel(n,p,t) {

};

void lights::setup() {

    Adafruit_NeoPixel::begin();
    Adafruit_NeoPixel::show();
    speed=30;
    amult=100;

    Particle.variable("debug", debugVal);
    Particle.variable("debugInt", debugInt);


}


//These are different ways to display

void lights::rainbow() {
    uint16_t i;

    if(j<256) {
	   for (i = 0; i < numPixels(); i++) {

		  long lngRGB = Wheel((i + j) & 255);
		  uint8_t toSet[3];
		  toSet[0] =(uint8_t)((lngRGB >> 16) & 0xff),
		  toSet[1] = (uint8_t)((lngRGB >> 8) & 0xff),
		  toSet[2] = (uint8_t)(lngRGB & 0xff);

		  setIndevColor(i,toSet[0], toSet[1], toSet[2]);
	   }
	   Adafruit_NeoPixel::show();
	   delay(speed);
	   j++;
    }
    else
	   j=0;
}




void lights::wave() {
    uint16_t i;

    if(j<256) {
	   for (i = 0; i < (numPixels() / 2); i++) {
		  long lngRGB = Wheel((i + j) & 255);
		  uint8_t toSet[3];
		  toSet[0] =(uint8_t)((lngRGB >> 16) & 0xff),
		  toSet[1] = (uint8_t)((lngRGB >> 8) & 0xff),
		  toSet[2] = (uint8_t)(lngRGB & 0xff);

		  setIndevColor((numPixels() / 2) -1 - i,toSet[0], toSet[1], toSet[2]);
		  setIndevColor((numPixels() / 2) + i,toSet[0], toSet[1], toSet[2]);
	   }
	   Adafruit_NeoPixel::show();
	   delay(speed);
	   j++;
    }
    else
	   j=0;
}


void lights::spread() {
    uint16_t i;

    if(j<256) {
	   for (i = 0; i < ((numPixels() / 2)+1); i++) {
		  long lngRGB = Wheel((i + j) & 255);
		  uint8_t toSet[3];
		  toSet[0] =(uint8_t)((lngRGB >> 16) & 0xff),
		  toSet[1] = (uint8_t)((lngRGB >> 8) & 0xff),
		  toSet[2] = (uint8_t)(lngRGB & 0xff);

		  setIndevColor(numPixels() - i,toSet[0], toSet[1], toSet[2]);
		  setIndevColor(0 + i,toSet[0], toSet[1], toSet[2]);
	   }
	   Adafruit_NeoPixel::show();
	   delay(speed);
	   j++;
    }
    else
	   j=0;
}


void lights::spectrum() {
    uint16_t i;
    if(j<256) {
	   for (i = 0; i < numPixels(); i++) {
		  long lngRGB = Wheel((i + j) & 255);
		  uint8_t toSet[3];
		  toSet[0] =(uint8_t)((lngRGB >> 16) & 0xff),
		  toSet[1] = (uint8_t)((lngRGB >> 8) & 0xff),
		  toSet[2] = (uint8_t)(lngRGB & 0xff);

		  setIndevColor(i,toSet[0], toSet[1], toSet[2]);
	   }
	   Adafruit_NeoPixel::show();
	   delay(speed);
	   j++;
    }
    else
	   j=0;
}

void lights::off() {
    for(j=0; j<numPixels(); j++) {
	   setIndevColor(j,0, 0, 0);

    }
    Adafruit_NeoPixel::show();
    delay(speed);

}

//Glimmery effect
void lights::glimmer(uint8_t r, uint8_t g, uint8_t b) {
    int maxDiff=60;
    uint8_t  goalArr[3];
    goalArr[0]=alpha(r);
    goalArr[1]=alpha(g);
    goalArr[2]=alpha(b);


    for (int n =0; n<numPixels(); n++) {


	   int toSet[3];


	   long lngRGB = Adafruit_NeoPixel::getPixelColor(n);
	   uint8_t currCol[3];
	   currCol[0] =(uint8_t)((lngRGB >> 16) & 0xff),
	   currCol[1] = (uint8_t)((lngRGB >> 8) & 0xff),
	   currCol[2] = (uint8_t)(lngRGB & 0xff);

	   int diff=abs(currCol[0]-goalArr[0])+abs(currCol[1]-goalArr[1])+abs(currCol[2]-goalArr[2]);

	   for (int i=0; i<3; i++) {

		  if (diff>=(maxDiff*3)) { //if the color is either too far or too close to the color.
			int newVal=goalArr[i] +randInRange(maxDiff); //random value +- currCol

			 toSet[i]=newVal;

		  }

		  //simply move the value closer to the goal value
		  else {
			 toSet[i]=goalArr[i]+randInRange(10);
		  }


		 //if the new value with the random factor exceeds the limits
		 if (toSet[i]>255) {
			toSet[i]=255;
		 }
		 else if (toSet[i]<0) {
			toSet[i]=abs(toSet[i]);
		 }
	   }


	   setIndevColor(n,(uint8_t)toSet[0],(uint8_t)toSet[1], (uint8_t)toSet[2]);


    }
    Adafruit_NeoPixel::show();
    delay(speed);
}


//Has a slight twinkling effect
void lights::twinkle(uint8_t r, uint8_t g, uint8_t b) {
    int maxDiff=10;
    uint8_t  goalArr[3];
    goalArr[0]=alpha(r);
    goalArr[1]=alpha(g);
    goalArr[2]=alpha(b);

    for (int n =0; n<numPixels(); n++) {
	   uint8_t toSet[3];

	   //getting the pixel colors into a useful format
	   long lngRGB = Adafruit_NeoPixel::getPixelColor(n);
	   uint8_t currCol[3];
	   currCol[0] =(uint8_t)((lngRGB >> 16) & 0xff),
	   currCol[1] = (uint8_t)((lngRGB >> 8) & 0xff),
	   currCol[2] = (uint8_t)(lngRGB & 0xff);

	   int diff=abs(currCol[0]-goalArr[0])+abs(currCol[1]-goalArr[1])+abs(currCol[2]-goalArr[2]);

	   for (int i=0; i<3; i++) {

		  if (diff>=(maxDiff*3) || diff<1) { //if the color is either too far or too close to the color.

		    int newVal=goalArr[i] +randInRange(maxDiff); //random value +- currCol
		  // int newVal=goalArr[i] +-2*rand()%(maxDiff+2); //random value +- currCol

			//if the new value with the random factor exceeds the limits
			if (newVal>255) {
			    newVal=255;
			}
			else if (newVal<0) {
			    newVal=abs(newVal);
			}

			 toSet[i]=newVal;
		  }

		  //simply move the value closer to the goal value
		  else {
			 if (currCol[i]>goalArr[i]) {
				toSet[i]=currCol[i]-1;
			 }
			 else {
				toSet[i]=currCol[i]+1;

			 }
		  }
	   }


	   // setIndevColor(n,(uint8_t)toSet[0],(uint8_t)toSet[1], (uint8_t)toSet[2]);
	   Adafruit_NeoPixel::setPixelColor(n, (uint8_t)toSet[0],(uint8_t)toSet[1], (uint8_t)toSet[2]);

    }
    Adafruit_NeoPixel::show();
    delay(speed*3);
}


//sets rgb color to whole strip
void lights::setColor(uint8_t r, uint8_t g, uint8_t b) {


    for (int n = 0; n < numPixels(); n++) { //iterate through number of pixels
	   setIndevColor(n, alpha(r), alpha(g), alpha(b));
    }

    Adafruit_NeoPixel::show();
    delay(speed);

}


//sets an indevidual pixels color smooothly
void lights::setIndevColor(int n, uint8_t r, uint8_t g, uint8_t b) {
    //runs the goal color through alpha()
    uint8_t goalColor[3];
    goalColor[0]=r;
    goalColor[1]=g;
    goalColor[2]=b;

    uint8_t size=3;
    uint8_t toSet[3];



    long lngRGB = Adafruit_NeoPixel::getPixelColor(n); //gets current color value in 32 bits
    uint8_t currCol[3];
    currCol[0] = (uint8_t)((lngRGB >> 16) & 0xff), //splits into rgb
    currCol[1] = (uint8_t)((lngRGB >> 8) & 0xff),
    currCol[2] = (uint8_t)(lngRGB & 0xff);


    // uint8_t* currCol=getColor(n);


    for (int i=0; i<size; i++) {  //This  loops through colors
	   uint8_t gapSize =abs(goalColor[i]>currCol[i]);
	   float stepRatio=0.5;
	   int step = stepRatio*gapSize;


	   if(step<1) {
		  step=1;
	   }


	   if (goalColor[i]>currCol[i]) { //if the color needs to increment
		  toSet[i]=currCol[i]+step;

	   }
	   else if (goalColor[i]<currCol[i]) { //if it neeeds to decrement
		  toSet[i]=currCol[i]-step;

	   }
	   else if(goalColor[i]==currCol[i]){                          //if its there or also this is a catch
		  toSet[i]=goalColor[i];

		  // Particle.publish("success:", output);
	   }

    }

    Adafruit_NeoPixel::setPixelColor(n, (int)toSet[0],(int)toSet[1], (int)toSet[2]); //doesn't have alpha because the goalColor has already been run through alpha()

}


//changes alpha
void lights::setAlpha(uint8_t a) {
   amult=a;
}

//sets speed
void lights::setSpeed(uint8_t s) {
    speed=s;
}



//yellow green effect, outdated
void lights::boostio() {
    uint16_t i;

    if(j<256) {
	   for (i = 0; i < numPixels(); i++) {
		  Adafruit_NeoPixel::setPixelColor(i, boostioWheel((i + j) & 255));
		  Adafruit_NeoPixel::show();
		  delay(speed/2);
		  j++;
	   }

    }
    else
	   j=0;
}




//taken from adafruit
uint32_t lights::Wheel(byte WheelPos) {
    if (WheelPos < 85) {
	   return Color(alpha(WheelPos * 3), alpha(255 - WheelPos * 3), 0);
    } else if (WheelPos < 170) {
	   WheelPos -= 85;
	   return Color(alpha(255 - WheelPos * 3), 0, alpha(WheelPos * 3));
    } else {
	   WheelPos -= 170;
	   return Color(0, alpha(WheelPos * 3), alpha(255 - WheelPos * 3));
    }
}




//Private

//wheel for the boostio function
uint32_t lights::boostioWheel(byte WheelPos) {
    if (WheelPos < 128) {
	   return Color(alpha(255 - WheelPos * 2), alpha(255), 0);
    } else {
	   WheelPos -= 128;
	   return Color(alpha(WheelPos * 2), alpha(255),0);
    }
}

//calculates alpha of value
uint8_t lights::alpha(uint8_t val) {
    double mult=(double)amult/255;
    return (int)((double)val*mult);
}


//gets color, needs testing
uint8_t * lights::getColor(uint8_t position) {
    double mult=(double)amult/255;


    int r =(Adafruit_NeoPixel::getPixelColor(position) >> 16);
    int g =(Adafruit_NeoPixel::getPixelColor(position) >> 8);
    int b =(Adafruit_NeoPixel::getPixelColor(position));


    static uint8_t arr[3];
    arr[0]= r / mult;
    arr[1]= g / mult;
    arr[2]= b / mult;
    return arr;
}

//returns a random int between the range of -offset and offset
int lights::randInRange(int offset) {
    int randVal=-2*rand()%(offset+2);


    return randVal;
}


//gets step size
uint8_t lights::getStep(int color, int liveColor, int count) {

    double step = double(color-liveColor)/count;
    if (abs(step)<1) {
	   return step/abs(step); //Creative way to round to 1 but keep the sign
    }
    else {
	   return step;
    }
}
