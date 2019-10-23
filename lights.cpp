#include "lights.h"
#include <neopixel.h>
#include <math.h>
#include <bitset>

uint16_t j;
uint8_t speed, amult;

char debugVal[50];
int debugInt=-1;

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
            Adafruit_NeoPixel::setPixelColor(i, Wheel((i + j) & 255));
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
            Adafruit_NeoPixel::setPixelColor((numPixels() / 2) -1 - i, Wheel((i + j) & 255));
            Adafruit_NeoPixel::setPixelColor((numPixels() / 2) + i, Wheel((i + j) & 255));
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
            Adafruit_NeoPixel::setPixelColor(numPixels() - i, Wheel((i + j) & 255));
            Adafruit_NeoPixel::setPixelColor(0 + i, Wheel((i + j) & 255));
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
            Adafruit_NeoPixel::setPixelColor(i, Wheel((j) & 255));
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
        Adafruit_NeoPixel::setPixelColor(j, 0);
        Adafruit_NeoPixel::show();
        delay(speed);
  }
    
}



void lights::setColor(uint8_t r, uint8_t g, uint8_t b) {
    //runs the goal color through alpha()
    uint8_t goalColor[3];
    goalColor[0]=alpha(r);
    goalColor[1]=alpha(g);
    goalColor[2]=alpha(b);
   
    uint8_t size=3;
    uint8_t toSet[3];

    long lngRGB = Adafruit_NeoPixel::getPixelColor(0);
    uint8_t currCol[3];
    currCol[0] = (uint8_t)((lngRGB >> 16) & 0xff),
    currCol[1] = (uint8_t)((lngRGB >> 8) & 0xff),
    currCol[2] = (uint8_t)(lngRGB & 0xff);
    
    
    
    
    for (int n = 0; n < numPixels(); n++) { //iterate through number of pixels
        
        
        long lngRGB = Adafruit_NeoPixel::getPixelColor(n); //gets current color value in 32 bits
        uint8_t currCol[3];
        currCol[0] = (uint8_t)((lngRGB >> 16) & 0xff), //splits into rgb
        currCol[1] = (uint8_t)((lngRGB >> 8) & 0xff),
        currCol[2] = (uint8_t)(lngRGB & 0xff);
        
        
        //Yes this is a little striated, but it was built up in pieces.
        
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
        


        //char *output = &binRGB[0]; //converts the string to a char*
        
        //Particle.publish(":D", output);        
        Adafruit_NeoPixel::setPixelColor(n, (int)toSet[0],(int)toSet[1], (int)toSet[2]); //doesn't have alpha because the goalColor has already been run through alpha()
            
        
       
        
    }
    Adafruit_NeoPixel::show();
    delay(speed);

    
}

void lights::setAlpha(uint8_t a) {
   amult=a;
}

void lights::setSpeed(uint8_t s) {
    speed=s;
}


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
uint32_t lights::boostioWheel(byte WheelPos) {
    if (WheelPos < 128) {
        return Color(alpha(255 - WheelPos * 2), alpha(255), 0);
    } else {
        WheelPos -= 128;
        return Color(alpha(WheelPos * 2), alpha(255),0);
    }
}


uint8_t lights::alpha(uint8_t val) {
    double mult=(double)amult/255;
    return (int)((double)val*mult);
}

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

uint8_t lights::getStep(int color, int liveColor, int count) {
    
    double step = double(color-liveColor)/count;
    if (abs(step)<1) {
        return step/abs(step); //Creative way to round to 1 but keep the sign
    }
    else {
        return step;
    }
}