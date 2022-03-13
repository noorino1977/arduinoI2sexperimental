#include <Arduino.h>
#include "driver/i2s.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#define fsampling 44100    //44100 //96000 //48000 /
#define length_buffer 128

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = fsampling,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S, 
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = length_buffer,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = fsampling*384
 };

i2s_pin_config_t pin_config = {.bck_io_num = 26, .ws_io_num = 27, .data_out_num = 14, .data_in_num = 12};

void parameter_I2S(){
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_sample_rates(I2S_NUM_0, fsampling);
  i2s_set_clk(I2S_NUM_0, fsampling, I2S_BITS_PER_SAMPLE_24BIT, I2S_CHANNEL_STEREO);
  i2s_start(I2S_NUM_0);
};

void mclk_pin_select(const uint8_t pin) {
    if(pin != 0 && pin != 1 && pin != 3) Serial.printf("Only support GPIO0/GPIO1/GPIO3, gpio_num:%d",pin);      
    switch(pin){
        case 0:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
            WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
            break;
        case 1:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
            WRITE_PERI_REG(PIN_CTRL, 0xF0F0);
            break;
        case 3:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
            WRITE_PERI_REG(PIN_CTRL, 0xFF00);
            Serial.println("ok"); 
            break;
        default:
            break;
    }
}
