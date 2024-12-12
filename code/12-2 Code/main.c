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


#define BUTTON_PRESS        1
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
#define LCD_H_RES           240
#define LCD_V_RES           320
#define LCD_ON              1
#define LCD_OFF             !LCD_ON
#define LCD_PIXEL_CLK_HZ                (20 * 1000 * 1000)
#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8

static uint16_t *lines[2];

static void update_menu(esp_lcd_panel_handle_t panel_handle){

    int s_line = 0;
    int c_line = 0;

    for(int y = 0; y < LCD_V_RES; y += LCD_PARALLEL){
        display(lines[c_line], y, LCD_PARALLEL);
        s_line = c_line;
        c_line = !c_line;
        esp_lcd_panel_draw_bitmap(panel_handle, 0, y, 0 + LCD_H_RES, y + LCD_PARALLEL, lines[s_line]);
    }
}

esp_lcd_panel_handle_t lcd_init(){
    gpio_config_t lcd_led_config = {
        .pin_bit_mask = 1ULL << LCD_LED,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&lcd_led_config));
    ESP_ERROR_CHECK(gpio_set_level(LCD_LED, LCD_OFF));

    spi_bus_config_t LCD_SPI_CONFIG = {
        .sclk_io_num = LCD_SCK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t) * 2,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &LCD_SPI_CONFIG, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t lcd_handle = NULL;
    esp_lcd_panel_io_spi_config_t lcd_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = LCD_CS,
        .pclk_hz = LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &lcd_config, &lcd_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RESET,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(lcd_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));

    return panel_handle;
}

spi_device_handle_t adc_init(){
    gpio_config_t adc_cs_config = {
        .pin_bit_mask = 1ULL << ADC_CS,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t adc_avdd_config = {
        .pin_bit_mask = 1ULL << GPIO_NUM_35,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&adc_cs_config));
    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 1));

    ESP_ERROR_CHECK(gpio_config(&adc_avdd_config));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_35, 1));

    spi_bus_config_t ADC_SPI_CONFIG = {
        .sclk_io_num = ADC_SCK,
        .mosi_io_num = ADC_MOSI,
        .miso_io_num = ADC_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 50,
    };

    spi_device_handle_t adc_handle = NULL;
    spi_device_interface_config_t adc_config = {
        .command_bits = 8,
        .address_bits = 0,
        .clock_speed_hz = 1 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 1,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &ADC_SPI_CONFIG, 0));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &adc_config, &adc_handle));

    return adc_handle;

}

void app_main(void)
{
    lines[0] = malloc(LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t));
    lines[1] = malloc(LCD_H_RES * LCD_PARALLEL * sizeof(uint16_t));

    gpio_config_t buttons_config = {
        .pin_bit_mask = (1ULL << GPIO_BUTTON_0) | (1ULL << GPIO_BUTTON_1) | (1ULL << GPIO_BUTTON_2) | (1ULL << GPIO_BUTTON_3) | \
                        (1ULL << GPIO_BUTTON_4) | (1ULL << GPIO_BUTTON_5) | (1ULL << GPIO_BUTTON_6) | (1ULL << GPIO_BUTTON_7) | \
                        (1ULL << GPIO_BUTTON_8),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    ESP_ERROR_CHECK(gpio_config(&buttons_config));
    
    esp_lcd_panel_handle_t panel_handle = lcd_init();
    spi_device_handle_t adc_handle = adc_init();

    display_init(LCD_H_RES, LCD_V_RES);
    display_start_screen();
    
    spi_transaction_t write_irq = {
        .cmd = 0b01010110,
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 8,    
        .rxlength = 0,
        .tx_data = {0b01110111},
    };
    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 0));
    ESP_ERROR_CHECK(spi_device_polling_transmit(adc_handle, &write_irq));
    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 1));

    spi_transaction_t write_config_0 = {
        .cmd = 0b01000110,
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 8,
        .rxlength = 0,
        .tx_data = {0b11110011},
    };

    spi_transaction_t read_data = {
        .cmd = 0b01000001,
        .flags = SPI_TRANS_USE_RXDATA,
        .length = 24,
        .rxlength = 24,
    };

    ESP_ERROR_CHECK(gpio_set_level(LCD_LED, LCD_ON));
    update_menu(panel_handle);
    vTaskDelay(100);

    int selected_line = 3;
    int wafer_size = 2;
    char wafer_type = 'P';
    
    display_main_menu(selected_line, wafer_size, wafer_type);

    while((gpio_get_level(GPIO_BUTTON_0) | gpio_get_level(GPIO_BUTTON_1) | gpio_get_level(GPIO_BUTTON_2) | gpio_get_level(GPIO_BUTTON_3) | \
          gpio_get_level(GPIO_BUTTON_4) | gpio_get_level(GPIO_BUTTON_5) | gpio_get_level(GPIO_BUTTON_6) | gpio_get_level(GPIO_BUTTON_7) | \
          gpio_get_level(GPIO_BUTTON_8)) == BUTTON_NOT_PRESS){
        vTaskDelay(10);
    }
    update_menu(panel_handle);
    vTaskDelay(100);



    //GPIO_BUTTON_0 = DOWN; PIN 16
    //GPIO_BUTTON_1 = UP; PIN 15
    //GPIO_BUTTON_2 = ENTER; PIN 14
    //GPIO_BUTTON_3 = MEASURE; PIN
    

    while(true){
        vTaskDelay(10);
        if(gpio_get_level(GPIO_BUTTON_0) == BUTTON_PRESS){
            if(selected_line == 3){
                selected_line = 5;
            }else if(selected_line == 5){
                selected_line = 6;
            }else if(selected_line == 6){
                selected_line = 7;
            }else{
                selected_line = 3;
            }
            display_main_menu(selected_line, wafer_size, wafer_type);
            update_menu(panel_handle);
            vTaskDelay(100);
        }

        if(gpio_get_level(GPIO_BUTTON_1) == BUTTON_PRESS){
            if(selected_line == 3){
                selected_line = 7;
            }else if(selected_line == 5){
                selected_line = 3;
            }else if(selected_line == 6){
                selected_line = 5;
            }else{
                selected_line = 6;
            }
            display_main_menu(selected_line, wafer_size, wafer_type);
            update_menu(panel_handle);
            vTaskDelay(100);
        }

        if(gpio_get_level(GPIO_BUTTON_2) == BUTTON_PRESS){
            if(selected_line == 3){
                display_wafer_size_selection(wafer_size);
                update_menu(panel_handle);
                vTaskDelay(100);

                while(gpio_get_level(GPIO_BUTTON_2) == BUTTON_NOT_PRESS){
                    vTaskDelay(10);
                    if(gpio_get_level(GPIO_BUTTON_0) == BUTTON_PRESS){
                        if(wafer_size == 2){
                            wafer_size = 3;
                        }else if(wafer_size == 3){
                            wafer_size = 4;
                        }else if(wafer_size == 4){
                            wafer_size = 8;
                        }else if(wafer_size == 8){
                            wafer_size = 10;
                        }else if(wafer_size == 10){
                            wafer_size = 12;
                        }else{
                            wafer_size = 2;
                        }
                        display_wafer_size_selection(wafer_size);
                        update_menu(panel_handle);
                        vTaskDelay(100);
                    }

                    if(gpio_get_level(GPIO_BUTTON_1) == BUTTON_PRESS){
                        if(wafer_size == 2){
                            wafer_size = 12;
                        }else if(wafer_size == 3){
                            wafer_size = 2;
                        }else if(wafer_size == 4){
                            wafer_size = 3;
                        }else if(wafer_size == 8){
                            wafer_size = 4;
                        }else if(wafer_size == 10){
                            wafer_size = 8;
                        }else{
                            wafer_size = 10;
                        }
                        display_wafer_size_selection(wafer_size);
                        update_menu(panel_handle);
                        vTaskDelay(100);
                    }
                }
            }else if(selected_line == 5){
                display_wafer_type_selection(wafer_type);
                update_menu(panel_handle);
                vTaskDelay(100);

                while(gpio_get_level(GPIO_BUTTON_2) == BUTTON_NOT_PRESS){
                    vTaskDelay(10);
                    if(gpio_get_level(GPIO_BUTTON_0) == BUTTON_PRESS || gpio_get_level(GPIO_BUTTON_1) == BUTTON_PRESS){
                        if(wafer_type == 'N'){
                            wafer_type = 'P';
                        }else{
                            wafer_type = 'N'; 
                        }

                        display_wafer_type_selection(wafer_type);
                        update_menu(panel_handle);
                        vTaskDelay(100);
                    }
                }
            }else if(selected_line == 6){

            }else if(selected_line == 7){
                display_perform_measurement();
                update_menu(panel_handle);
                vTaskDelay(100);

                while(gpio_get_level(GPIO_BUTTON_2) == BUTTON_NOT_PRESS){
                    vTaskDelay(10);
                }

                int byte_0 = 0;
                int byte_1 = 0;
                int byte_2 = 0;
                int successful = 0;

                for(int i = 0; i < 10; i++){
                    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 0));
                    ESP_ERROR_CHECK(spi_device_polling_transmit(adc_handle, &write_config_0));
                    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 1));
                    
                    vTaskDelay(10);
                    
                    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 0));
                    ESP_ERROR_CHECK(spi_device_polling_transmit(adc_handle, &read_data));
                    if((read_data.rx_data[0] & 0x80) == 0x80){
                        byte_0 = byte_0 + read_data.rx_data[0];
                        byte_1 = byte_1 + read_data.rx_data[1];
                        byte_2 = byte_2 + read_data.rx_data[2];
                        ESP_LOGI("Byte 0:", "%d", byte_0);
                        ESP_LOGI("Byte 1:", "%d", byte_1);
                        ESP_LOGI("Byte 2:", "%d", byte_2);
                        successful++;
                    }
                    ESP_ERROR_CHECK(gpio_set_level(ADC_CS, 1));
                }
                if(successful == 0){
                    ESP_LOGE("None Successful", "");
                }

                ESP_LOGI("Succesful:", "%d", successful);

                byte_0 = byte_0 / successful;
                byte_1 = byte_1 / successful;
                byte_2 = byte_2 / successful;

                ESP_LOGI("Byte 0:", "%d", byte_0);
                ESP_LOGI("Byte 1:", "%d", byte_1);
                ESP_LOGI("Byte 2:", "%d", byte_2);

                int result = (0xFF << 24) | (byte_0 << 16) | (byte_1 << 8) | byte_2;
                result = result * -1;
                result = result - 9500;
                if((0x80 & byte_0) == 0){
                    result = 0;
                }
                double calculated_voltage = (double) result * 3.3 / 8388608;

                double diameter = 0.1016;
                double thickness = 0.0005;
                double spacing = 0.002;
                
                double correction_a = 4.53;
                double correction_f = 1.0 / (1.0 + 9.0 / diameter / diameter * spacing * spacing);
                double correction_k = 2.0;

                double current = 0.2;

                double resistance = correction_k * correction_a * correction_f * calculated_voltage / current;
                double resistivity = resistance * thickness;

                ESP_LOGI("Calculated Voltage", "%f", calculated_voltage);
                
                ESP_LOGI("Calculated f", "%f", correction_f);
                ESP_LOGI("Calculated Resistance", "%f", resistance);
                ESP_LOGI("Calculated    ", "%f", resistivity);


            }

            display_main_menu(selected_line, wafer_size, wafer_type);
            update_menu(panel_handle);
            vTaskDelay(100);
        }
    }
}