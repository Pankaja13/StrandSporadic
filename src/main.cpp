#include <NeoPixelBus.h>
#include <Arduino.h>


#define PIXEL_COUNT 50
#define MAX_LIT_PIXELS 4
const uint8_t PixelPin = 10;
auto BLACK = HslColor(0, 0, 0);

// three element pixels, in different order and speeds
NeoPixelBus<NeoRgbFeature, NeoWs2812xMethod> strip(PIXEL_COUNT, PixelPin);

int count = 0;
RgbColor color;
uint8_t pos;

enum Mode {
    FadeIn,
    FadeOut,
    Dead
};

struct LitPixel {
    int loc;
    float hue;
    float sat;
    float lum;
    Mode mode;
    float fadeRate;
};

LitPixel data[MAX_LIT_PIXELS];
int lit_pixels = 0;

void updatePixels();
LitPixel newPixel();
float getRandomHue();

void setup() {
    Serial.begin(115200);
    Serial.println("Hello");
    randomSeed(analogRead(1));

    strip.Begin();
    strip.ClearTo(BLACK);
    strip.Show();

}


void loop() {

    if (lit_pixels < MAX_LIT_PIXELS) {
        data[lit_pixels] = newPixel();
        lit_pixels++;
    }
    updatePixels();
    delay(10);
}


void updatePixels() {
    for (int i = 0; i < lit_pixels; ++i) {
        LitPixel litPixel = data[i];
        Serial.print("mode ");
        Serial.println(litPixel.mode);
        Serial.println(litPixel.lum);
        Serial.println("-----");

        strip.SetPixelColor(litPixel.loc,  HslColor(litPixel.hue, 1.0, litPixel.lum));
        if (litPixel.mode == FadeIn) {
            litPixel.lum = (float ) (litPixel.lum + litPixel.fadeRate);
            if (litPixel.lum >= 0.5) litPixel.mode = FadeOut;
        } else if (litPixel.mode == FadeOut){
            litPixel.lum = (float ) (litPixel.lum - litPixel.fadeRate);
            if (litPixel.lum <= 0.0) litPixel.mode = Dead;
        } else {
            strip.SetPixelColor(litPixel.loc,  BLACK);
            litPixel = newPixel();
        }

        data[i] = litPixel;
        strip.Show();
    }
}

LitPixel newPixel() {
    int pixel_loc = (int) random(0, PIXEL_COUNT);
    LitPixel litPixel{
    pixel_loc,
    getRandomHue(),
    1.0,
    0.0,
    FadeIn,
    (((float) random(1, 5))/1000)
    };
    return litPixel;
}

float getRandomHue() {
    return (float) random(0, 360) / 360;
}
