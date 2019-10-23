// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// This #include statement was automatically added by the Particle IDE.
#include "lights.h"

// This #include statement was automatically added by the Particle IDE.
#include "colors.h"

// This #include statement was automatically added by the Particle IDE.
#include "colors.h"

#include "lights.h"
#include <neopixel.h>
#include "application.h"
#include <algorithm>
#include <string> 



#define DEFAULT_SPEED 60

#define DEFAULT_R 0
#define DEFAULT_G 0
#define DEFAULT_B 0
#define DEFAULT_A 100

#define PIXEL_PIN D0


#define PIXEL_TYPE WS2812B


#define PIXEL_COUNT 60

#define USE_POT 0
#define NAME [REDACTED]

byte select;

uint8_t r=DEFAULT_R;
uint8_t g=DEFAULT_G;
uint8_t b=DEFAULT_B;
uint8_t speedVal=DEFAULT_SPEED;
int potPin = A0; 
bool hasPublished=false;

uint8_t potPrev= 255*(analogRead(potPin)/4095.0);
    


//constructor for the library
lights lightstrip = lights(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);





void setup() {
    Particle.subscribe(NAME, general);
    Particle.subscribe(NAME, specific);
    lightstrip.setup();
    RGB.control(true);
    RGB.brightness(0);
    Serial.begin(9600);
}

void loop() {
    control();
    
    if(USE_POT) {
    
        //  Serial.println(analogRead(potPin));
        uint8_t potScaled= 255*(analogRead(potPin)/4095.0);
        
        
        if (abs(potScaled-potPrev)>2) { //if the pot value has significantly changed
            
            int potWheel=lightstrip.Wheel(potScaled);
            r = (uint8_t)((potWheel >> 16) & 0xff), //splits into rgb
            g = (uint8_t)((potWheel >> 8) & 0xff),
            b = (uint8_t)(potWheel & 0xff);
            select=0;
            
            hasPublished=false;
        }
        else if (hasPublished==false) {
            
            
            char post[15];
            
            sprintf(post, "rgb:%03d,%03d,%03d", r,g,b);
            
            Particle.publish("chtapodiEcho", post);
            hasPublished=true;
            
        }
        
        potPrev=potScaled;
    
    }

     
     //  Adafruit_NeoPixel::setPixelColor(i, Wheel((j) & 255));

}





void general(const char *event, const char *data)
{
    process(data);

}


void specific(const char *event, const char *data)
{
    process(data);

}


//This function takes the input string and selects the appropriate statement.
//There is likely a more effective way to do this in C++, and I will have to update accordingly
void process ( const char *data) {
    
    std::string s = data;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    
    if (strcmp(data,"rainbow")==0) {
      select=1;
    }
  
    else if (strcmp(data,"wave")==0) {
        select=2;
    }
    else if (strcmp(data,"spread")==0) {
        select=3;
    }
    else if (strcmp(data,"spectrum")==0) {
        select=4;
    }
    else if (strcmp(data,"lightsOff")==0 || strcmp(data,"night")==0) {
      select=5;
    }
    
    else if (strcmp(data,"boostio")==0) {
        select=6;
    }
  
    else if (s.find("speed") !=std::string::npos) {
        s.erase(0, s.find(":") + 1);
        speedVal=atoi(s.c_str());
        lightstrip.setSpeed(speedVal);
        
    }
    else if (s.find("alpha") !=std::string::npos) {
        s.erase(0, s.find(":") + 1);
        uint8_t a=(std::atoi(s.c_str()));
        lightstrip.setAlpha(a);
        
    }
    else if (s.find("rgb") !=std::string::npos) {
        
        
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;

        s.erase(0, s.find(":") + delimiter.length());
        pos = s.find(delimiter);
        token = s.substr(0, pos);
        r = std::atoi (token.c_str());
        
        s.erase(0, pos + delimiter.length());
        pos = s.find(delimiter);
        token = s.substr(0, pos);
        g = std::atoi (token.c_str());
        
        s.erase(0, pos + delimiter.length());
        b = std::atoi (s.c_str());
        select=0;
        
    }
    
    else {
    // if the data is something else, don't do anything.
        uint8_t * colorArr = colors::getRGB(s);
        if(colorArr!=NULL) {
            r=colorArr[0];
            g=colorArr[1];
            b=colorArr[2];
            String data="";
            select=0;
        }
        
    }
    
}


void control() {
    
    switch(select) {
        
        case 0: lightstrip.setColor(r,g,b);
            break;
        
        case 1: lightstrip.rainbow();
            break;
        
        case 2: lightstrip.wave();
            break;
        
        case 3: lightstrip.spread();
            break;
        
        case 4: lightstrip.spectrum();
            break;
            
        case 5: lightstrip.off();
            break;
        case 6: lightstrip.boostio();
            break;
    }
    
}



//TODO: Setup setcolor so its a seperate function that operates on a pixel by pixel level, and then integrate into other functions to make them less harsh




