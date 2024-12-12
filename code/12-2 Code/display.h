#pragma once

void display(uint16_t *pixel_lines, int current_line, int lines_drawn);
void display_init(int line_width, int total_lines);
void display_start_screen();
void display_main_menu(int selected_line, int wafer_size, char wafer_type);
void display_wafer_size_selection(int wafer_size);
void display_wafer_type_selection(char wafer_type);
void display_perform_measurement();