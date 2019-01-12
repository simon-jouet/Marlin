#include <Arduino.h> // replace that with the proper imports
#include "i2s.h"
#include "../../core/macros.h"
#include "driver/periph_ctrl.h"
#include "rom/lldesc.h"
#include "soc/i2s_struct.h"

typedef enum {
    I2S_NUM_0 = 0x0,  /*!< I2S 0*/
    I2S_NUM_1 = 0x1,  /*!< I2S 1*/
    I2S_NUM_MAX,
} i2s_port_t;

static portMUX_TYPE i2s_spinlock[I2S_NUM_MAX] = {portMUX_INITIALIZER_UNLOCKED, portMUX_INITIALIZER_UNLOCKED};
static i2s_dev_t* I2S[I2S_NUM_MAX] = {&I2S0, &I2S1};

#define I2S_ENTER_CRITICAL()         portENTER_CRITICAL(&i2s_spinlock[i2s_num])
#define I2S_EXIT_CRITICAL()          portEXIT_CRITICAL(&i2s_spinlock[i2s_num])

inline static void gpio_matrix_out_check(uint32_t gpio, uint32_t signal_idx, bool out_inv, bool oen_inv)
{
    //if pin = -1, do not need to configure
    if (gpio != -1) {
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[gpio], PIN_FUNC_GPIO);
        gpio_set_direction((gpio_num_t)gpio, (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
        gpio_matrix_out(gpio, signal_idx, out_inv, oen_inv);
    }
}

static esp_err_t i2s_reset_fifo(i2s_port_t i2s_num)
{
    I2S_ENTER_CRITICAL();
    I2S[i2s_num]->conf.rx_fifo_reset = 1;
    I2S[i2s_num]->conf.rx_fifo_reset = 0;
    I2S[i2s_num]->conf.tx_fifo_reset = 1;
    I2S[i2s_num]->conf.tx_fifo_reset = 0;
    I2S_EXIT_CRITICAL();

    return ESP_OK;
}

esp_err_t i2s_start(i2s_port_t i2s_num)
{
    //start DMA link
    I2S_ENTER_CRITICAL();
    i2s_reset_fifo(i2s_num);

    //reset dma
    I2S[i2s_num]->lc_conf.in_rst = 1;
    I2S[i2s_num]->lc_conf.in_rst = 0;
    I2S[i2s_num]->lc_conf.out_rst = 1;
    I2S[i2s_num]->lc_conf.out_rst = 0;

    I2S[i2s_num]->conf.tx_reset = 1;
    I2S[i2s_num]->conf.tx_reset = 0;
    I2S[i2s_num]->conf.rx_reset = 1;
    I2S[i2s_num]->conf.rx_reset = 0;

    I2S[i2s_num]->int_clr.val = 0xFFFFFFFF;
    I2S[i2s_num]->out_link.start = 1;
    I2S[i2s_num]->conf.tx_start = 1;
    I2S_EXIT_CRITICAL();

    return ESP_OK;
}

esp_err_t i2s_stop(i2s_port_t i2s_num)
{
    I2S_ENTER_CRITICAL();
    I2S[i2s_num]->out_link.stop = 1;
    I2S[i2s_num]->conf.tx_start = 0;

    I2S[i2s_num]->int_clr.val = I2S[i2s_num]->int_st.val; //clear pending interrupt
    I2S_EXIT_CRITICAL();

    return ESP_OK;
}


void i2s_init()
{
    periph_module_enable(PERIPH_I2S0_MODULE);

    // stop i2s
    i2s_stop(I2S_NUM_0);

    // configure I2S data port interface.
    i2s_reset_fifo(I2S_NUM_0);

    //reset i2s
    I2S0.conf.tx_reset = 1;
    I2S0.conf.tx_reset = 0;
    I2S0.conf.rx_reset = 1;
    I2S0.conf.rx_reset = 0;

    //reset dma
    I2S0.lc_conf.in_rst = 1;
    I2S0.lc_conf.in_rst = 0;
    I2S0.lc_conf.out_rst = 1;
    I2S0.lc_conf.out_rst = 0;

    //Enable and configure DMA
    I2S0.lc_conf.check_owner = 0;
    I2S0.lc_conf.out_loop_test = 0;
    I2S0.lc_conf.out_auto_wrback = 0;
    I2S0.lc_conf.out_data_burst_en = 0;
    I2S0.lc_conf.outdscr_burst_en = 0;
    I2S0.lc_conf.out_no_restart_clr = 0;
    I2S0.lc_conf.indscr_burst_en = 0;
    I2S0.lc_conf.out_eof_mode = 1;

    I2S0.conf2.lcd_en = 0;
    I2S0.conf2.camera_en = 0;
    I2S0.pdm_conf.pcm2pdm_conv_en = 0;
    I2S0.pdm_conf.pdm2pcm_conv_en = 0;

    I2S0.fifo_conf.dscr_en = 0;

    I2S0.conf_chan.tx_chan_mod = 3;
    I2S0.fifo_conf.tx_fifo_mod = 3;
    I2S0.conf.tx_mono = 0;

    I2S0.conf_chan.rx_chan_mod = 0;
    I2S0.fifo_conf.rx_fifo_mod = 0;
    I2S0.conf.rx_mono = 0;

    I2S0.fifo_conf.dscr_en = 1; //connect dma to fifo

    I2S0.conf.tx_start = 0;
    I2S0.conf.rx_start = 0;

    I2S0.conf.tx_msb_right = 0;
    I2S0.conf.tx_right_first = 0;

    I2S0.conf.tx_slave_mod = 0; // Master
    I2S0.fifo_conf.tx_fifo_mod_force_en = 1;

    I2S0.pdm_conf.rx_pdm_en = 0;
    I2S0.pdm_conf.tx_pdm_en = 0;

    I2S0.conf.tx_short_sync = 0;
    I2S0.conf.rx_short_sync = 0;
    I2S0.conf.tx_msb_shift = 0;
    I2S0.conf.rx_msb_shift = 0; 

    // set clock, fbck = fpll/(N + b/a)
    I2S0.clkm_conf.clka_en = 0;      // Use PLL/2 as reference
    I2S0.clkm_conf.clkm_div_num = 4; // reset value of 4
    I2S0.clkm_conf.clkm_div_a = 1;   // 0 at reset, what about divide by 0?
    I2S0.clkm_conf.clkm_div_b = 0;   // 0 at reset

    // Initial the constant channel value to 0
    I2S0.conf_single_data = 0;

    // Disable TX interrupts
    I2S0.int_ena.out_eof = 0;
    I2S0.int_ena.out_dscr_err = 0;

    // i2s_set_pin
    gpio_matrix_out_check(22, I2S0O_DATA_OUT23_IDX, 0, 0);
    gpio_matrix_out_check(25, I2S0O_WS_OUT_IDX, 0, 0);
    gpio_matrix_out_check(26, I2S0O_BCK_OUT_IDX, 0, 0);

    // Start the I2S peripheral
    i2s_start(I2S_NUM_0);
}

void i2s_write(uint8_t pin, uint8_t val) {
    // in 16 bits mode the top 2 bytes are sent
    SET_BIT_TO(I2S0.conf_single_data, pin+16, val);
}