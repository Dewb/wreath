#include <Arduino.h>
#include <FastLED.h>

// 2017 Holiday LED Wreaths

// quick CA test code for ESP8266/WS2811 

#define LED_PIN     12
#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define NUM_LEDS    50

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];
CRGBPalette16 gPalette;

static byte cells[NUM_LEDS];


void setup() {

    delay(1000); 

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    gPalette = CRGBPalette16(CRGB::Red, CRGB::White, CRGB::Green, CRGB::Red);

}

void loop()
{
    random16_add_entropy(random(561));
    int threshhold = 120;
    int boost_low = 1;
    int boost_high = 4;

    // simple continuous CA based loosely on Rule 30
    for(int c = 0; c < NUM_LEDS; c++) {
        
        int neighbors = 0;
        
        if (c > 0 && cells[c-1] > threshhold) {
            neighbors++;
        }
        if (c < NUM_LEDS - 1 && cells[c+1] > threshhold) {
            neighbors++;
        }
        if (cells[c] > threshhold && random(0, 2) == 1) { 
            neighbors++;
        }

        if (neighbors == 1) {
            cells[c] += random(boost_low, boost_high);
        } else {
            cells[c] -= random(boost_low, boost_high);
        }
    }

    for(int i = 0; i < NUM_LEDS; i++) {
      byte colorindex = scale8(cells[i], 240);
      CRGB color = ColorFromPalette(gPalette, colorindex);
      leds[i] = color;
    }
  
    FastLED.show(); 
    FastLED.delay(1000 / FRAMES_PER_SECOND);

}


