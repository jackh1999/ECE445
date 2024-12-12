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

void set_pixel(char to_print, int x, int y, int x_offset, int y_offset, uint16_t inverse){
    if((ASCII[(uint8_t) to_print - 32][y - y_offset] & (1ULL << (7 - ((x - x_offset) % 8)))) > 0){
        *(pixels + x + y * lcd_width) = 0x0000 ^ inverse;
    }else{
        *(pixels + x + y * lcd_width) = 0xFFFF ^ inverse;
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
            if(x >= 48 && x < 48 + 18*8 && y >= 36 && y < 48){
                x_offset = 48;
                y_offset = 36; 
                to_print = line_1[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            } else if(x >= 64 && x < 64 + 14*8 && y >= 204 && y < 216){
                x_offset = 64;
                y_offset = 204; 
                to_print = line_2[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);  
            } else if(x >= 64 && x < 64 + 14*8 && y >= 216 && y < 228){
                x_offset = 64;
                y_offset = 216; 
                to_print = line_3[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);  
            } else if(x >= 64 && x < 64 + 14*8 && y >= 228 && y < 240){
                x_offset = 64;
                y_offset = 228; 
                to_print = line_4[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);    
            } else if(x >= 60 && x < 60 + 15*8 && y >= 240 && y < 252){
                x_offset = 60;
                y_offset = 240; 
                to_print = line_5[(x - x_offset) / 8];
                
                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);  
            } else if(x >= 8 && x < 8 + 28*8 && y >= 288 && y < 300){
                x_offset = 8;
                y_offset = 288;
                to_print = line_6[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);  
            } else{
                *(pixels + x + y * lcd_width) = 0xFFFF; 
            }
        }
    }
}

void display_main_menu(int selected_line, int wafer_size, char wafer_type){
    int x_offset = 0;
    int y_offset = 0;
    char to_print = '0'; 
    uint16_t selected = 0x0000;
    char* line_1 = "Main Menu"; // 9
    char* line_2 = "Wafer Size"; // 10
    char* line_3 = "-Inch Wafer"; // 11
    char* line_4 = "Doping Type"; // 11
    char* line_5 = "-Type"; // 15
    char* line_6 = "Bonus Information"; // 17
    char* line_7 = "Perform Measurement"; // 19 

    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){
            selected = 0x0000;
            if(x >= 84 && x < 84 + 9*8 && y >= 36 && y < 48){
                x_offset = 84;
                y_offset = 36; 
                to_print = line_1[(x - x_offset) / 8];
                if(selected_line == 1){
                    selected = 0xFFFF;
                }

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 80 && x < 80 + 10*8 && y >= 96 && y < 108){
                x_offset = 80;
                y_offset = 96;
                to_print = line_2[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else if(wafer_size < 10 && y >= 108 && y < 120){
                y_offset = 108;
                if(selected_line == 3){
                    selected = 0xFFFF;
                }

                if(x >= 72 && x < 80){
                    x_offset = 72;
                    to_print = wafer_size + 48;

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else if(x >= 80 && x < 80 + 11*8){
                    x_offset = 80;
                    to_print = line_3[(x - x_offset) / 8];

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else{
                    *(pixels + x + y * lcd_width) = 0xFFFF;
                }
            }else if(wafer_size >= 10 && y >= 108 && y < 120){
                y_offset = 108;
                if(selected_line == 3){
                    selected = 0xFFFF;
                }

                if(x >= 68 && x < 76){
                    x_offset = 68;
                    to_print = '1';

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else if(x >= 76 && x < 84){
                    x_offset = 76;
                    to_print = wafer_size % 10 + 48;

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else if(x >= 84 && x < 84 + 11*8){
                    x_offset = 84;
                    to_print = line_3[(x - x_offset) / 8];

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else{
                    *(pixels + x + y * lcd_width) = 0xFFFF;
                }
            }else if(x >= 76 && x < 76 + 11*8 && y >= 168 && y < 180){
                x_offset = 76;
                y_offset = 168;
                to_print = line_4[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else if(y >= 180 && y < 192){
                y_offset = 180;
                if(selected_line == 5){
                    selected = 0xFFFF;
                }

                if(x >= 96 && x < 104){
                    x_offset = 96;
                    to_print = wafer_type;

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else if(x >= 104 && x < 104 + 5*8){
                    x_offset = 104;
                    to_print = line_5[(x - x_offset) / 8];

                    set_pixel(to_print, x, y, x_offset, y_offset, selected);
                }else{
                    *(pixels + x + y * lcd_width) = 0xFFFF;
                }
            }else if(x >= 52 && x < 52 + 17*8 && y >= 240 && y < 252){
                if(selected_line == 6){
                    selected = 0xFFFF;
                }
                x_offset = 52;
                y_offset = 240;
                to_print = line_6[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 44 && x < 44 + 19*8 && y >= 288 && y < 300){
                if(selected_line == 7){
                    selected = 0xFFFF;
                }
                x_offset = 44;
                y_offset = 288;
                to_print = line_7[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else{
                *(pixels + x + y * lcd_width) = 0xFFFF;
            }
        }
    }
}

void display_wafer_size_selection(int wafer_size){
    int x_offset = 0;
    int y_offset = 0;
    char to_print = '0'; 
    uint16_t selected = 0x0000;
    char* line_1 = "Wafer Size Selection"; // 20
    char* line_2 = "2-Inch Wafer"; // 12
    char* line_3 = "3-Inch Wafer"; // 12
    char* line_4 = "4-Inch Wafer"; // 12
    char* line_5 = "8-Inch Wafer"; // 12 
    char* line_6 = "10-Inch Wafer"; // 13
    char* line_7 = "12-Inch Wafer"; // 13

    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){
            selected = 0x0000;
            if(x >= 40 && x < 40 + 20*8 && y >= 36 && y < 48){
                x_offset = 40;
                y_offset = 36;
                to_print = line_1[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 108 && y < 120){
                if(wafer_size == 2){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 108;
                to_print = line_2[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 144 && y < 156){
                if(wafer_size == 3){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 144;
                to_print = line_3[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 180 && y < 192){
                if(wafer_size == 4){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 180;
                to_print = line_4[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 216 && y < 228){
                if(wafer_size == 8){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 216;
                to_print = line_5[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 68 && x < 68 + 13*8 && y >= 252 && y < 264){
                if(wafer_size == 10){
                    selected = 0xFFFF;
                }

                x_offset = 68;
                y_offset = 252;
                to_print = line_6[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 68 && x < 68 + 13*8 && y >= 288 && y < 300){
                if(wafer_size == 12){
                    selected = 0xFFFF;
                }

                x_offset = 68;
                y_offset = 288;
                to_print = line_7[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else{
                *(pixels + x + y * lcd_width) = 0xFFFF;
            }
        }
    }
}

void display_wafer_type_selection(char wafer_type){
    int x_offset = 0;
    int y_offset = 0;
    char to_print = '0'; 
    uint16_t selected = 0x0000;
    char* line_1 = "Wafer Type Selection"; // 20
    char* line_2 = "N-Type Wafer"; // 12
    char* line_3 = "P-Type Wafer"; // 12

    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){
            selected = 0x0000;
            if(x >= 40 && x < 40 + 20*8 && y >= 36 && y < 48){
                x_offset = 40;
                y_offset = 36;
                to_print = line_1[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 144 && y < 156){
                if(wafer_type == 'N'){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 144;
                to_print = line_2[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else if(x >= 72 && x < 72 + 12*8 && y >= 180 && y < 192){
                if(wafer_type == 'P'){
                    selected = 0xFFFF;
                }

                x_offset = 72;
                y_offset = 180;
                to_print = line_3[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, selected);
            }else{
                *(pixels + x + y * lcd_width) = 0xFFFF;
            }
        }
    }

}

void display_perform_measurement(){
    int x_offset = 0;
    int y_offset = 0;
    char to_print = '0'; 
    char* line_1 = "Place Probe Head on Wafer"; // 25
    char* line_2 = "Press Enter When Ready"; // 22

    for(int y = 0; y < lcd_height; y++){
        for(int x = 0; x < lcd_width; x++){
            if(x >= 20 && x < 20 + 25*8 && y >= 144 && y < 156){
                x_offset = 20;
                y_offset = 144;
                to_print = line_1[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else if(x >= 32 && x < 32 + 22*8 && y >= 204 && y < 216){
                x_offset = 32;
                y_offset = 204;
                to_print = line_2[(x - x_offset) / 8];

                set_pixel(to_print, x, y, x_offset, y_offset, 0x0000);
            }else{
                *(pixels + x + y * lcd_width) = 0xFFFF;
            }
        }
    }
}

void end_of_menu(uint16_t *pixels){
    free(pixels);
}