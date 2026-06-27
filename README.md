# Object Orenated Rpi Pico ST7735

A header only Object oriented port (?) ( C -> C++ ) of bablok's pico-st7735 that is also desined to slighly mirror Adafruits st7735 library (<a href="github.com/adafruit/Adafruit-ST7735-Library/blob/master/Adafruit_ST77xx.cpp">link here</a>), but for the raspberry pi pico. Parts of my code is really just a wrapper.

Probely would have been easier to create a compatibility layer insteared of porting a port.

## Notes

The pico-st7735 reposetory (Bobloks work) is currently not required.

Google gemini helped a little with disecting Bobloks work.

Had to use ChatGPT for some debugging, I'm unflimilar with embeded systems protocals.

The 'asm valatile()' tells the compiler that there is assembly that shouldn't be edited. the '"nop \n nop \n nop"' is assembly for no operations (no changes in CPU registurs,  memory, etc) for 3 clock cycles. The assembly will be inserted direcly into the machine code.

DC        -> Data/Command<br>
SCK/CLK   -> SPI Clock<br>
CS/SS     -> Chip Select<br>
RST       -> Reset<br>

Boblok's work: <br>
<https://github.com/bablokb/pic-st7735> <br>
Boblok's work is a port of: <br>
<https://github.com/gavinlyonsrepo/pic_16F18346_projects>

## Usage

See "DisplayTest/"

 1: Initialize stdio (stdio_ini_all())<br>
 2: create the onject and assign variale with the constructer<br>
 3: call the initR() method with recoured tab:

- INITR_REDTAB
- INITR_BLACKTAB
- INITR_GREENTAB
- INITR_B 

 4: Set your desired colors and curser location.<br>
 5: Print to the display.

## Future Plans

### Capibilities

- invertDisplay()
- enableDisplay()
- further mirror Adafruits ST7735 library

### Other

- Reorganize and restrucure
- commets (commits?)
- Fix my Spelling
