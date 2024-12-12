#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "display.h"

#define GPIO_BUTTON         GPIO_NUM_1
#define GPIO_LED            GPIO_NUM_2

#define LCD_LED             GPIO_NUM_7
#define LCD_SCK             GPIO_NUM_20
#define LCD_MOSI            GPIO_NUM_10
#define LCD_DC              GPIO_NUM_11
#define LCD_RESET           GPIO_NUM_42
#define LCD_CS              GPIO_NUM_12

#define LCD_PARALLEL        8
#define LCD_H_RES           320
#define LCD_V_RES           240
#define LCD_ON              1
#define LCD_OFF             !LCD_ON
#define LCD_PIXEL_CLK_HZ                (20 * 1000 * 1000)
#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8

static uint16_t *lines[2];

static void update_menu(esp_lcd_panel_handle_t panel_handle){

    lines[0] = malloc(LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t));
    lines[1] = malloc(LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t));

    int s_line = 0;
    int c_line = 0;

    for(int y = 0; y < LCD_V_RES; y += LCD_PARALLEL){
        display(lines[c_line], y, LCD_PARALLEL);
        s_line = c_line;
        c_line = !c_line;
        esp_lcd_panel_draw_bitmap(panel_handle, 0, y, 0 + LCD_H_RES, y + LCD_PARALLEL, lines[s_line]);
    }
}

void app_main(void)
{


    gpio_config_t led_1_config = {
        .pin_bit_mask = 1ULL << GPIO_LED,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t lcd_led_config = {
        .pin_bit_mask = 1ULL << LCD_LED,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    // GPIO 1 Setup -- Button Input
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLDOWN_ONLY));

    // GPIO 2 Setup -- LED Output
    ESP_ERROR_CHECK(gpio_config(&led_1_config));

    // GPIO 42 Setup -- Backlight Output
    ESP_ERROR_CHECK(gpio_config(&lcd_led_config));

    
    spi_bus_config_t LCD_SPI_CONFIG = {
        .sclk_io_num = LCD_SCK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t),
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &LCD_SPI_CONFIG, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = LCD_CS  ,
        .pclk_hz = LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RESET,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(gpio_set_level(LCD_LED, LCD_OFF));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, true));

    ESP_ERROR_CHECK(gpio_set_level(LCD_LED, LCD_ON));\

    display_init(LCD_H_RES, LCD_V_RES);

    char first_message[12] =  "First Screen";

    display_message(first_message, 12);

    update_menu(panel_handle);

    char second_message[14] = "Input Detected";

    display_message(second_message, 14);
    
    while(gpio_get_level(GPIO_BUTTON) == 0){
        vTaskDelay(100);
    }

    ESP_ERROR_CHECK(gpio_set_level(GPIO_LED, 1));
    update_menu(panel_handle);

}