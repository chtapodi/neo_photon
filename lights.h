#ifndef lights_H
#define lights_H
#include <neopixel.h>
#include "application.h"

class lights : Adafruit_NeoPixel {
    public:
	   //constructor
	   lights(uint16_t n, uint8_t p, uint8_t t);

	   //fancy functions
	   void
		  setup(),
		  show(),
		  rainbow(),
		  wave(),
		  spread(),
		  spectrum(),
		  off(),
		  boostio(),
		  glimmer(uint8_t r, uint8_t g, uint8_t b),
		  twinkle(uint8_t r, uint8_t g, uint8_t b),
		  setColor(uint8_t r, uint8_t g, uint8_t b),
		  setIndevColor(int n, uint8_t r, uint8_t g, uint8_t b),
		  setAlpha(uint8_t a),
		  setSpeed(uint8_t s);
	   uint32_t Wheel(byte);

    private:
	   int randInRange(int offset);
	   uint32_t boostioWheel(byte);
	   uint8_t alpha(uint8_t val);
	   uint8_t * getColor(uint8_t position);
	   uint8_t getStep(int color, int liveColor, int count);






};

#endif
