add_library(oop-pico-ST7735 INTERFACE)

target_sources(oop-pico-ST7735 INTERFACE
   oop_pico-st7735.hpp 
)


target_link_libraries(oop-pico-ST7735 INTERFACE
    pico_stdlib
)


target_include_directories(oop-pico-ST7735 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
     include
)

target_link_libraries(oop-pico-ST7735 INTERFACE
    hardware_spi
    hardware_i2c
)