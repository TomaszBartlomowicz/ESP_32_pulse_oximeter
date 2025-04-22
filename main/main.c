#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "max30102.h"
#include "sh1106_i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ 100000

#define TAG "MAIN"

void i2c_master_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void app_main(void) {

    sh1106_init();
    i2c_master_init();
    
    if (max30102_init(I2C_MASTER_NUM) == ESP_OK) {
        ESP_LOGI(TAG, "Sensor connected.");
        max30102_start_measurement(I2C_MASTER_NUM);
    } else {
        ESP_LOGE(TAG, "Sensor not detected!");
        return;
    }

    // Wyczyść bufor (czyli cały ekran)
    sh1106_clear();
    sh1106_draw_text(0, 2, "HEART RATE:");
    sh1106_draw_text(0, 6, "BLOOD OXYGEN:");
    sh1106_display();

    while (1) {
        max30102_data_t data;
        if (max30102_read_data(I2C_MASTER_NUM, &data) == ESP_OK) {
            ESP_LOGI(TAG, "SpO2: %d%%, BPM: %lu, Temp: %.2f C", data.spo2, data.bpm, data.temperature);
            char heart_rate[50];
            char spo2[50];
            sprintf(heart_rate, "%3lu", data.bpm);
            sprintf(spo2, "%d%%", data.spo2);
            sh1106_draw_empty_heart(93, 2);
            sh1106_draw_empty_bubbles(93 ,6);
            sh1106_display();
            sh1106_draw_heart(93, 2);
            sh1106_draw_bubbles(93 ,6);
            if (data.bpm < 300){
                sh1106_draw_text(108, 2, heart_rate);
                sh1106_draw_text(108, 6, spo2);
            }
            else{
                sh1106_draw_text(108, 2, "---");
                sh1106_draw_text(108, 6, "---");
            }
            sh1106_display();

        } else {
            ESP_LOGE(TAG, "Failed to read data.");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
