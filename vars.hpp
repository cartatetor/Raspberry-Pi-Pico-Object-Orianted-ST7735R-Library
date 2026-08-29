/*
Raspberry Pi Pico ST7735R library
Copyright (C) 2026  Carter Hertter (cartatetor)

Based on Boblokb's pico-st7735 repo.

Boblokb's work:
<https://github.com/bablokb/pico-st7735>
Boblok's work is a port of:
<https://github.com/gavinlyonsrepo/pic_16F18346_projects>

Used <https://github.com/licenses/license-templates/tree/master> as a license header template

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#pragma pack(push, 1)

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

namespace ST7735 {
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
}