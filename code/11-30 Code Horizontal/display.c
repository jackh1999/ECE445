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

void set_pixel(char to_print, int x, int y, int x_offset, int y_offset){
    if((ASCII[(uint8_t) to_print - 32][y - y_offset] & (1ULL << (7 - ((x - x_offset) % 8)))) > 0){
        *(pixels + x + y * lcd_width) = 0x0000;
    }else{
        *(pixels + x + y * lcd_width) = 0xFFFF;
    }
}

void display_start_screen(){
    int x_offset = 0;
    int y_offset = 0;
    char to_print = '0'; 
    char* line_1 = "ECE 445 Final Demo"; // 18
    char* line_2 = "Group Members:"; // 14
    char* line_3 = "Dorian Tricaud"; // 14
    char* line_4 = "Ming-Yan Hsiao"; // 14
    char* line_5 = "Simon Danthinne"; // 15
    char* line_6 = "Press Any Button to Continue"; // 28 

    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){
            if(x >= 88 && x < 88 + 18*8 && y >= 36 && y < 48){
                x_offset = 88;
                y_offset = 36; 
                to_print = line_1[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset);
            } else if(x >= 104 && x < 104 + 14*8 && y >= 120 && y < 132){
                x_offset = 104;
                y_offset = 120; 
                to_print = line_2[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset);  
            } else if(x >= 104 && x < 104 + 14*8 && y >= 132 && y < 144){
                x_offset = 104;
                y_offset = 132; 
                to_print = line_3[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset);  
            } else if(x >= 104 && x < 104 + 14*8 && y >= 144 && y < 156){
                x_offset = 104;
                y_offset = 144; 
                to_print = line_4[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset);    
            } else if(x >= 100 && x < 100 + 15*8 && y >= 156 && y < 168){
                x_offset = 100;
                y_offset = 156; 
                to_print = line_5[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset);  
            } else if(x >= 48 && x < 48 + 28*8 && y >= 204 && y < 216){
                x_offset = 48;
                y_offset = 204;
                to_print = line_6[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset);  
            } else{
                *(pixels + x + y * lcd_width) = 0xFFFF; 
            }
        }
    }
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