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
#include "hal/spi_types.h"


#define BUTTON_PRESS        0
#define BUTTON_NOT_PRESS    !BUTTON_PRESS
#define GPIO_BUTTON_0       GPIO_NUM_16
#define GPIO_BUTTON_1       GPIO_NUM_15
#define GPIO_BUTTON_2       GPIO_NUM_14
#define GPIO_BUTTON_3       GPIO_NUM_13
#define GPIO_BUTTON_4       GPIO_NUM_37
#define GPIO_BUTTON_5       GPIO_NUM_38
#define GPIO_BUTTON_6       GPIO_NUM_39
#define GPIO_BUTTON_7       GPIO_NUM_40
#define GPIO_BUTTON_8       GPIO_NUM_41
// GPIO_BUTTON_9 REPLACED BY LCD_RESET


#define GPIO_LED            GPIO_NUM_2

#define LCD_LED             GPIO_NUM_7
#define LCD_SCK             GPIO_NUM_20
#define LCD_MOSI            GPIO_NUM_10
#define LCD_DC              GPIO_NUM_11
#define LCD_RESET           GPIO_NUM_42
#define LCD_CS              GPIO_NUM_12

#define ADC_SCK             GPIO_NUM_18
#define ADC_MCK             GPIO_NUM_19
#define ADC_MOSI            GPIO_NUM_7
#define ADC_MISO            GPIO_NUM_8
#define ADC_CS              GPIO_NUM_6
#define ADC_OUT             GPIO_NUM_9

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
    gpio_config_t buttons_config = {
        .pin_bit_mask = (1ULL << GPIO_BUTTON_0) | (1ULL << GPIO_BUTTON_1) | (1ULL << GPIO_BUTTON_2) | (1ULL << GPIO_BUTTON_3) | \
                        (1ULL << GPIO_BUTTON_4) | (1ULL << GPIO_BUTTON_5) | (1ULL << GPIO_BUTTON_6) | (1ULL << GPIO_BUTTON_7) | \
                        (1ULL << GPIO_BUTTON_8),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
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

    ESP_ERROR_CHECK(gpio_config(&buttons_config));

    ESP_ERROR_CHECK(gpio_config(&lcd_led_config));

    
    spi_bus_config_t LCD_SPI_CONFIG = {
        .sclk_io_num = LCD_SCK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t),
    };

    spi_bus_config_t ADC_SPI_CONFIG = {
        .sclk_io_num = ADC_SCK,
        .mosi_io_num = ADC_MOSI,
        .miso_io_num = ADC_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &LCD_SPI_CONFIG, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &ADC_SPI_CONFIG, SPI_DMA_CH_AUTO));

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

    ESP_ERROR_CHECK(gpio_set_level(LCD_LED, LCD_ON));

    display_init(LCD_H_RES, LCD_V_RES);

    display_start_screen();
    update_menu(panel_handle);

    char first_message[12] =  "First Screen";
    display_message(first_message, 12);

    while((gpio_get_level(GPIO_BUTTON_0) & gpio_get_level(GPIO_BUTTON_1) & gpio_get_level(GPIO_BUTTON_2) & gpio_get_level(GPIO_BUTTON_3) & \
          gpio_get_level(GPIO_BUTTON_4) & gpio_get_level(GPIO_BUTTON_5) & gpio_get_level(GPIO_BUTTON_6) & gpio_get_level(GPIO_BUTTON_7) & \
          gpio_get_level(GPIO_BUTTON_8)) == BUTTON_NOT_PRESS){
        vTaskDelay(100);
    }
    update_menu(panel_handle);
    vTaskDelay(1000);

    char second_message[14] = "Input Detected";
    display_message(second_message, 14);
    
    while(gpio_get_level(GPIO_BUTTON_0) == 0){
        vTaskDelay(100);
    }

    ESP_ERROR_CHECK(gpio_set_level(GPIO_LED, 1));

    update_menu(panel_handle);

}