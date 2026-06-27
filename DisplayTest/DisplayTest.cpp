#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

//The dispay lib
#include "oop_pico-st7735.hpp"

#define MOSI 19             //TX
#define DC 6                //Data/Command
#define PIN_CS 17           //chip select
#define PIN_SCK 18          //spi clock
#define PIN_RST 22          //reset pin

#define LED 25

int main() {
    stdio_init_all();

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    ST7735 tft = ST7735(PIN_CS, DC, MOSI, PIN_SCK, PIN_RST, spi0);

    tft.initR("INIT_REDTAB");

    gpio_put(LED, 1);
    sleep_ms(5000);

    tft.setBackgroundColor(ST7735_BLACK);
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);

    gpio_put(LED, 0);
    sleep_ms(5000);

    tft.print("Hello World!");
    tft.println("New Line");
    
    for(;;) {
        gpio_put(LED, 1);
        sleep_ms(100);
        gpio_put(LED, 0);
        sleep_ms(100);
    }
}