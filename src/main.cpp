#include <NeoPixelBus.h>
#include <Arduino.h>


#define PIXEL_COUNT 1
const uint8_t PixelPin = 10;
auto BLACK = HslColor(0, 0, 0);
float progress = 0.0;
int colorIndex = 0;

#define COLORS_LENGTH 3
HtmlColor targetColors[COLORS_LENGTH] = {
        HtmlColor(0xff0000),
        HtmlColor(0x00ff00),
        HtmlColor(0x0000ff),
};

// three element pixels, in different order and speeds
NeoPixelBus<NeoRbgFeature, NeoWs2811Method> strip(PIXEL_COUNT, PixelPin);

void setup() {
    Serial.begin(115200);
    Serial.println("Hello");
    randomSeed(analogRead(1));

    strip.Begin();
    strip.ClearTo(BLACK);
    strip.Show();

    for(unsigned int i = 0; i < COLORS_LENGTH; i++)
    {

            Serial.println(i);
            Serial.println(targetColors[i].Color);
            Serial.println("---------------");

    }


}

RgbColor getNextColor() {
    int nextIndex = (colorIndex + 1) % COLORS_LENGTH;
    Serial.println(nextIndex);
    return targetColors[nextIndex];
}

void loop() {

    RgbColor current = targetColors[colorIndex];
    RgbColor nextColor = getNextColor();

    RgbColor color = RgbColor::LinearBlend(current, nextColor, (float) min(progress, 1.0));

    strip.SetPixelColor(0, color);
    strip.Show();

    progress += 0.005;
//    Serial.print(progress);
//    Serial.print("\t");
//    Serial.println(colorIndex);
    if (progress > 2.0) {
        progress = 0;
        colorIndex = (colorIndex + 1) % COLORS_LENGTH;
    }
    delay(10);
}

