/*==================================================================
Carter Hertter
Gemini assised in "diesecting" (kind of)
June 6, 2026

A header only Object oriented port (?) ( C -> C++ ) of bablok's pico-st7735.
 It is desined to mirror adafruits st7735 library, 
 but for the raspberry pi pico. Parts of my code is really 
 just a wrapper. Probely would have been easyier to
 create a compatibility layer insteared of porting a port.

the 'asm valatile()' tells the compiler that there is assembly 
 that shouldn't be edited. the '"nop \n nop \n nop"' is assembly 
 for no operations (no changes in CPU registurs,  memory, etc)
 for 3 clock cycles. The assembly will be inserted direcly
 into the machine code.

Boblok's work:
https://github.com/bablokb/pic-st7735
Boblok's work is a port of:
https://github.com/gavinlyonsrepo/pic_16F18346_projects

==================================================================*/

#pragma once
#pragma pack(push, 1)

#include <cstdint>
#include <string>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "stdint.h"

// copied and pasted
#define _swap(a, b) { int16_t t; t = a; a = b; b = t;}
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_VSCRSADD 0x37
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_PWCTR6  0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions?
#define ST7735_BLACK 0x0000
#define ST7735_BLUE 0x001F
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_WHITE 0xFFFF

#define LCD_ASCII_OFFSET 0x20 //0x20, ASCII character for Space, The font table starts with this character

//end copied and pasted


//The class
class ST7735 {
    private:
        uint16_t BackColor = ST7735_BLACK;

        uint8_t TFT_CS, TFT_CLK, TFT_RST, TFT_MOSI, TFT_DC;
        spi_inst_t* TFT_SPI = spi0;

        uint16_t TextColor = ST7735_WHITE;

        uint8_t _width = 128;
        uint8_t _height = 160;

        uint16_t cursor_x = 0;
        uint16_t cursor_y = 0;

        uint8_t size = 1; //text size.

        const uint8_t hi = 1;
        const uint8_t lo = 0;

        uint8_t _colstart = 0, _rowstart = 0, _tft_type, _rotation = 0, _xstart = 0, _ystart = 0;

        bool _wrap = true;

    //low leval stuff
        void tft_cs_low() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_CS, 0);
            asm volatile("nop \n nop \n nop");
        }
        void tft_cs_high() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_CS, 1);
            asm volatile("nop \n nop \n nop");
        }
        void tft_dc_low() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_DC, 0);
            asm volatile("nop \n nop \n nop");
        }
        void tft_dc_high() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_DC, 1);
            asm volatile("nop \n nop \n nop");
        }
        void tft_rst_low() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_RST, 0);
            asm volatile("nop \n nop \n nop");
        }
        void tft_rst_high() {
            asm volatile("nop \n nop \n nop");
            gpio_put(TFT_RST, 1);
            asm volatile("nop \n nop \n nop");
        }

        //write spi command
        void write_command(uint8_t cmd_) {
            tft_dc_low();
            tft_cs_low();
            spi_write_blocking(TFT_SPI, &cmd_, 1);
            tft_dc_high();
        }

        // Write SPI data
        void write_data(uint8_t data_) {
            spi_write_blocking(TFT_SPI, &data_, 1);
        }

        void end_write() {
            tft_cs_high();
        }


    //some SPI hex stuff
        const uint8_t Font[500] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00,
            0x14, 0x7F, 0x14, 0x7F, 0x14, 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x23, 0x13, 0x08, 0x64, 0x62,
            0x36, 0x49, 0x56, 0x20, 0x50, 0x00, 0x08, 0x07, 0x03, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x00,
            0x00, 0x41, 0x22, 0x1C, 0x00, 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x08, 0x08, 0x3E, 0x08, 0x08,
            0x00, 0x80, 0x70, 0x30, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x60, 0x60, 0x00,
            0x20, 0x10, 0x08, 0x04, 0x02, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00,
            0x72, 0x49, 0x49, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D, 0x33, 0x18, 0x14, 0x12, 0x7F, 0x10,
            0x27, 0x45, 0x45, 0x45, 0x39, 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x41, 0x21, 0x11, 0x09, 0x07,
            0x36, 0x49, 0x49, 0x49, 0x36, 0x46, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x00,
            0x00, 0x40, 0x34, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14,
            0x00, 0x41, 0x22, 0x14, 0x08, 0x02, 0x01, 0x59, 0x09, 0x06, 0x3E, 0x41, 0x5D, 0x59, 0x4E,
            0x7C, 0x12, 0x11, 0x12, 0x7C, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x3E, 0x41, 0x41, 0x41, 0x22,
            0x7F, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x7F, 0x09, 0x09, 0x09, 0x01,
            0x3E, 0x41, 0x41, 0x51, 0x73, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41, 0x7F, 0x41, 0x00,
            0x20, 0x40, 0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x40, 0x40, 0x40, 0x40,
            0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x3E, 0x41, 0x41, 0x41, 0x3E,
            0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x51, 0x21, 0x5E, 
            0x7F, 0x09, 0x19, 0x29, 0x46,  // R
            0x26, 0x49, 0x49, 0x49, 0x32, // S
            0x03, 0x01, 0x7F, 0x01, 0x03, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x1F, 0x20, 0x40, 0x20, 0x1F,
            0x3F, 0x40, 0x38, 0x40, 0x3F, 0x63, 0x14, 0x08, 0x14, 0x63, 0x03, 0x04, 0x78, 0x04, 0x03,
            0x61, 0x59, 0x49, 0x4D, 0x43, 0x00, 0x7F, 0x41, 0x41, 0x41, 0x02, 0x04, 0x08, 0x10, 0x20,
            0x00, 0x41, 0x41, 0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40, 
            0x00, 0x03, 0x07, 0x08, 0x00, 0x20, 0x54, 0x54, 0x78, 0x40, 0x7F, 0x28, 0x44, 0x44, 0x38, 
            0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44, 0x44, 0x28, 0x7F, 0x38, 0x54, 0x54, 0x54, 0x18, 
            0x00, 0x08, 0x7E, 0x09, 0x02, 0x18, 0xA4, 0xA4, 0x9C, 0x78, 0x7F, 0x08, 0x04, 0x04, 0x78, 
            0x00, 0x44, 0x7D, 0x40, 0x00, 0x20, 0x40, 0x40, 0x3D, 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,
            0x00, 0x41, 0x7F, 0x40, 0x00, 0x7C, 0x04, 0x78, 0x04, 0x78, 0x7C, 0x08, 0x04, 0x04, 0x78, 
            0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x18, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x18, 0xFC, 
            0x7C, 0x08, 0x04, 0x04, 0x08, 0x48, 0x54, 0x54, 0x54, 0x24, 0x04, 0x04, 0x3F, 0x44, 0x24, 
            0x3C, 0x40, 0x40, 0x20, 0x7C, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x3C, 0x40, 0x30, 0x40, 0x3C, 
            0x44, 0x28, 0x10, 0x28, 0x44, 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x44, 0x64, 0x54, 0x4C, 0x44, 
            0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 
            0x02, 0x01, 0x02, 0x04, 0x02
        };

        void Bcmd(){
            write_command(ST7735_SWRESET);
            sleep_ms(50);
            end_write();

            write_command(ST7735_SLPOUT);
            sleep_ms(250);
            sleep_ms(250);
            end_write();

            write_command(ST7735_COLMOD);
            write_data(0x05);
            sleep_ms(10);
            end_write();

            write_command(ST7735_FRMCTR1);
            write_data(0x00);
            write_data(0x06);
            write_data(0x03);
            sleep_ms(10);
            end_write();

            write_command(ST7735_MADCTL);
            write_data(0x08);
            end_write();

            write_command(ST7735_DISSET5);
            write_data(0x15);
            write_data(0x02);
            end_write();

            write_command(ST7735_INVCTR);
            write_data(0x00);
            end_write();

            write_command(ST7735_PWCTR1);
            write_data(0x02);
            write_data(0x70);
            sleep_ms(10);
            end_write();

            write_command(ST7735_PWCTR2);
            write_data(0x05);
            end_write();

            write_command(ST7735_PWCTR3);
            write_data(0x01);
            write_data(0x02);
            end_write();

            write_command(ST7735_VMCTR1);
            write_data(0x3C);
            write_data(0x38);
            sleep_ms(10);
            end_write();

            write_command(ST7735_PWCTR6);
            write_data(0x11);
            write_data(0x15);
            end_write();

            write_command(ST7735_GMCTRP1);
            write_data(0x09); write_data(0x16); write_data(0x09); write_data(0x20);
            write_data(0x21); write_data(0x1B); write_data(0x13); write_data(0x19);
            write_data(0x17); write_data(0x15); write_data(0x1E); write_data(0x2B);
            write_data(0x04); write_data(0x05); write_data(0x02); write_data(0x0E);
            end_write();

            write_command(ST7735_GMCTRN1);
            write_data(0x0B); write_data(0x14); write_data(0x08); write_data(0x1E);
            write_data(0x22); write_data(0x1D); write_data(0x18); write_data(0x1E);
            write_data(0x1B); write_data(0x1A); write_data(0x24); write_data(0x2B);
            write_data(0x06); write_data(0x06); write_data(0x02); write_data(0x0F);
            sleep_ms(10);
            end_write();

            write_command(ST7735_CASET);
            write_data(0x00); write_data(0x02); write_data(0x08); write_data(0x81);
            end_write();

            write_command(ST7735_RASET);
            write_data(0x00); write_data(0x01); write_data(0x08); write_data(0xA0);
            end_write();

            write_command(ST7735_NORON);
            sleep_ms(10);
            end_write();

            write_command(ST7735_DISPON);
            sleep_ms(250);
            sleep_ms(250);
            end_write();
        }

        void Rcmd1(){
            write_command(ST7735_SWRESET);
            sleep_ms(150);
            end_write();

            write_command(ST7735_SLPOUT);
            sleep_ms(250);
            sleep_ms(250);
            end_write();

            write_command(ST7735_FRMCTR1);
            write_data(0x01);
            write_data(0x2C);
            write_data(0x2D);
            end_write();

            write_command(ST7735_FRMCTR2);
            write_data(0x01);
            write_data(0x2C);
            write_data(0x2D);
            end_write();
            
            write_command(ST7735_FRMCTR3);
            write_data(0x01); write_data(0x2C); write_data(0x2D);
            write_data(0x01); write_data(0x2C); write_data(0x2D);
            end_write();

            write_command(ST7735_INVCTR);
            write_data(0x07);
            end_write();

            write_command(ST7735_PWCTR1);
            write_data(0xA2);
            write_data(0x02);
            write_data(0x84);
            end_write();

            write_command(ST7735_PWCTR2);
            write_data(0xC5);
            end_write();

            write_command(ST7735_PWCTR3);
            write_data(0x0A);
            write_data(0x00);
            end_write();
            
            write_command(ST7735_PWCTR4);
            write_data(0x8A);
            write_data(0x2A);
            end_write();

            write_command(ST7735_PWCTR5);
            write_data(0x8A);
            write_data(0xEE);
            end_write();

            write_command(ST7735_VMCTR1);
            write_data(0x0E);
            end_write();

            write_command(ST7735_INVOFF);
            end_write();

            write_command(ST7735_MADCTL);            
            write_data(0xC8);
            end_write();

            write_command(ST7735_COLMOD);
            write_data(0x05);
            end_write();
        }

        void Rcmd2green(){
            write_command(ST7735_CASET);
            write_data(0x00); write_data(0x02);
            write_data(0x00); write_data(0x7F + 0x02);
            end_write();

            write_command(ST7735_RASET);
            write_data(0x00); write_data(0x01);
            write_data(0x00); write_data(0x9F + 0x01);
            end_write();            
        }

        void Rcmd2red(){
            write_command(ST7735_CASET);
            write_data(0x00); write_data(0x00);
            write_data(0x00); write_data(0x7F);
            end_write();
            
            write_command(ST7735_RASET);
            write_data(0x00); write_data(0x00);
            write_data(0x00); write_data(0x9F);
            end_write();
        }

        void Rcmd3(){
            write_command(ST7735_GMCTRP1);
            write_data(0x02); write_data(0x1C); write_data(0x07); write_data(0x12);
            write_data(0x37); write_data(0x32); write_data(0x29); write_data(0x2D);
            write_data(0x29); write_data(0x25); write_data(0x2B); write_data(0x39);
            write_data(0x00); write_data(0x01); write_data(0x03); write_data(0x10);
            end_write();
            
            write_command(ST7735_GMCTRN1);
            write_data(0x03); write_data(0x1D); write_data(0x07); write_data(0x06);
            write_data(0x2E); write_data(0x2C); write_data(0x29); write_data(0x2D);
            write_data(0x2E); write_data(0x2E); write_data(0x37); write_data(0x3F);
            write_data(0x00); write_data(0x00); write_data(0x02); write_data(0x10);
            end_write();
            
            write_command(ST7735_NORON);
            sleep_ms(10);
            end_write();

            write_command(ST7735_DISPON);
            sleep_ms(100);
            end_write();
        }
    
        void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
            write_command(ST7735_CASET);
            write_data(0x00);
            write_data(x0 += _xstart);
            write_data(0x00);
            write_data(x1 += _xstart);
            end_write();

            write_command(ST7735_RASET);
            write_data(0x00);
            write_data(y0 += _ystart);
            write_data(0x00);
            write_data(y1 += _ystart);
            end_write();

            write_command(ST7735_RAMWR); // Write to ram
        }

        void drawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg,  uint8_t size) {
            int8_t i, j;
            if((x >= _width) || (y >= _height))
                return;
            if(size < 1) size = 1;
            if((c < ' ') || (c > '~'))
                c = '?';
            for(i=0; i<5; i++ ) {
                uint8_t line;
                line = Font[(c - LCD_ASCII_OFFSET)*5 + i];
                for(j=0; j<7; j++, line >>= 1) {
                    if(line & 0x01) {
                        if(size == 1) 
                            drawPixel(x+i, y+j, color);
                        else          
                            fillRect(x+(i*size), y+(j*size), size, size, color);
                    }
                    else if(bg != color) {
                        if(size == 1) 
                            drawPixel(x+i, y+j, bg);
                        else          
                            fillRect(x+i*size, y+j*size, size, size, bg);
                    }
                }
            }
        }

        void SetSPI() {
            spi_init(TFT_SPI, 1200 * 10000);   //spi go vroom vroom (12Mhz)
            spi_set_format(TFT_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            //General SPI and clock
            gpio_set_function(TFT_MOSI, GPIO_FUNC_SPI);
            gpio_set_function(TFT_CLK,  GPIO_FUNC_SPI);

            //Data/Command
            gpio_init(TFT_DC);
            gpio_set_dir(TFT_DC, GPIO_OUT);
            gpio_put(TFT_DC, 1);
            gpio_put(TFT_DC, 0);
            gpio_put(TFT_DC, 1); 

            //Chip Select
            gpio_init(TFT_CS);
            gpio_set_dir(TFT_CS, GPIO_OUT);
            gpio_put(TFT_CS, 1);

            //reset
            gpio_init(TFT_RST);
            gpio_set_dir(TFT_RST, GPIO_OUT);
            gpio_put(TFT_RST, 0);
            sleep_ms(60);
            gpio_put(TFT_RST, 1);
            sleep_ms(120);
        }
//==================================================================================================
//==================================================================================================
    public:

    //============================================================================================//
        //class initalizer. Uses predefinded SPI pins 
        ST7735(uint8_t cs, uint8_t clk, uint8_t rst, spi_inst_t* spi) {
            //add inpt validation
            TFT_CS = cs;
            TFT_CLK = clk;
            TFT_RST = rst;
            TFT_MOSI = 19;
            TFT_DC = 16;
            TFT_SPI = spi;

            SetSPI();
        }

        //overloaded initalizer, custom SPI pins
        ST7735(uint8_t cs, uint8_t dc, uint8_t mosi, uint8_t clk, uint8_t rst, spi_inst_t* spi) {
            TFT_CS = cs;
            TFT_CLK = clk;
            TFT_RST = rst;
            TFT_MOSI = mosi;
            TFT_DC = dc;
            TFT_SPI = spi;

            SetSPI();
        }
    //============================================================================================//

    //set up the disply
        void initR(std::string init) {
            Rcmd1();
            if (init == "INITR_REDTAB") {
                Rcmd2red();
                Rcmd3();
                _tft_type = 0;
            }
            else if (init == "INITR_GREENTAB") {
                Rcmd2green();
                Rcmd3();
                _colstart = 2;
                _rowstart = 1;
                _tft_type = 0;
            }
            else if (init == "INITR_BLACKTAB") {
                Rcmd2red();
                Rcmd3();
                write_command(ST7735_MADCTL);
                write_data(0xC0);
                _tft_type = 1;
            }
            else if (init == "INITR_B") {
                Bcmd();
                _tft_type = 2;
            }
            //defualt, assumes blacktab
            else {
                Rcmd2red();
                Rcmd3();
                write_command(ST7735_MADCTL);
                write_data(0xC0);
                _tft_type = 1;
            }
        }

        void setTextColor(uint16_t color) {
            TextColor = color;
        }

        void setTextSize(uint8_t Tsize) {
            size = Tsize;
        }

        void setBackgroundColor(uint16_t color) {
            BackColor = color;
            fillScreen(color);
        }

        void setCursor(uint8_t x, uint8_t y) {
            cursor_y = y;
            cursor_x = x;
        }
    //basic screen functions
        void drawPixel(uint8_t x, uint8_t y, uint16_t color){
            if((x >= _width) || (y >= _height)) 
                return;
            setAddrWindow(x,y,x,y);
            write_data(color >> 8);
            write_data(color & 0xFF);
            end_write();
        }

        void fillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
            uint8_t hi, lo;
            if((x >= _width) || (y >= _height))
                return;
            if((x + w - 1) >= _width)  
                w = _width  - x;
            if((y + h - 1) >= _height) 
                h = _height - y;
            setAddrWindow(x, y, x+w-1, y+h-1);
            hi = color >> 8; lo = color;
            for(uint8_t row = 0; row < h; row++) {
                for(uint8_t col = 0; col < w; col++) {
                    spi_write_blocking(TFT_SPI, &hi, 1);
                    spi_write_blocking(TFT_SPI, &lo, 1);
                }
            }
            tft_cs_high();
        }

        void fillScreen(uint16_t color) {
            fillRectangle(0,0, _width, _height, color);
        }

        void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
            uint8_t hi, lo;
            if((x >= _width) || (y >= _height))
                return;
            if((y + h - 1) >= _height)
                h = _height - y;
            hi = color >> 8; lo = color;
            setAddrWindow(x, y, x, y + h - 1);
            while (h--) {
                spi_write_blocking(TFT_SPI, &hi, 1);
                spi_write_blocking(TFT_SPI, &lo, 1);
            }
            tft_cs_high() ;
        }

        void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
            int16_t i;
            for (i = x; i < x + w; i++) {
                drawFastVLine(i, y, h, color);
            }
        }

        
    //text functions
        void print(std::string _text) {
            uint16_t x, y;
            uint16_t textsize, i;
            x = cursor_x; 
            y = cursor_y;
            textsize = (uint16_t)_text.length();
            for(i = 0; i < textsize; i++){
                if (_text[i] == '\n') {
                    cursor_x = 0;
                    cursor_y += size * 8;
                    if(cursor_y > _height) cursor_y = _height;
                    continue;
                }

                if(_wrap && ((cursor_x + size * 5) > _width)) {
                    cursor_x = 0;
                    cursor_y = cursor_y + size * 7 + 3 ;
                    if(cursor_y > _height) cursor_y = _height;
                    if(_text[i] == LCD_ASCII_OFFSET) {
                        continue;
                    }
                }
                drawChar(cursor_x, cursor_y, _text[i], TextColor, BackColor, size);
                cursor_x = cursor_x + size * 5;
                if(cursor_x > _width) {
                    cursor_x = _width;
                }
            }
        }

        void println(std::string text) {
            print("\n" + text);
        }

    //
        void setRotation(uint8_t m) {
            // m can be 0-3
            uint8_t madctl = 0;

            _rotation = m % 4;

            switch (_rotation) {
            case 0:
                madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
                _height = 160;
                _width = 128;
                _xstart = _colstart;
                _ystart = _rowstart;
                break;
            case 1:
                madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
                _width = 160;
                _height = 128;
                _ystart = _colstart;
                _xstart = _rowstart;
                break;
            case 2:
                madctl = ST7735_MADCTL_RGB;
                _height = 160;
                _width = 128;
                _xstart = _colstart;
                _ystart = _rowstart;
                break;
            case 3:
                madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
                _width = 160;
                _height = 128;
                _ystart = _colstart;
                _xstart = _rowstart;
                break;
            default:
                println("Not a vaid rotation");
            }

            write_command(ST7735_MADCTL);
            write_data(madctl);
            end_write();
        }
};