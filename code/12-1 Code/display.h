#pragma once

void display(uint16_t *pixel_lines, int current_line, int lines_drawn);
void display_init(int line_width, int total_lines);
void display_start_screen();
void display_message(char message[], int message_length);
