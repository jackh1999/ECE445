#include <stdio.h>
#include <string.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "ascii.h"

// xFFFF = BLACK
// x0000 = WHITE

uint16_t *pixels;
int lcd_width;
int lcd_height;

static inline uint16_t get_pixel(int x, int y){
    return (uint16_t) * (pixels + x + y * lcd_width);    
}

void display(uint16_t *pixel_lines, int current_line, int lines_drawn){
    for(int y = current_line; y < current_line + lines_drawn; y++){
        for(int x = 0; x < lcd_width; x++){
            *(pixel_lines + x + (y - current_line) * lcd_width) = get_pixel(x, y);
        }
    }
}

void display_init(int line_width, int total_lines){
    lcd_width = line_width;
    lcd_height = total_lines;
    pixels = malloc(line_width * total_lines * sizeof(uint16_t));
}

void display_message(char message[], int message_length){
    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){

            if(x >= 32 && x < 32 + message_length * 8 && y >= 36 && y < 48){

                char to_print = message[(x - 32) / 8];

                if((ASCII[(uint8_t) to_print - 32][y - 36] & (1ULL << (7 - (x % 8)))) > 0){
                    *(pixels + x + y * lcd_width) = 0x0000;
                }else{
                    *(pixels + x + y * lcd_width) = 0xFFFF;
                }
            }else{
                *(pixels + x + y * lcd_width) = 0xFFFF;
            }
        }
    }
}

void end_of_menu(uint16_t *pixels){
    free(pixels);
}