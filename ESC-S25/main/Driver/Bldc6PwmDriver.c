#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#include "Bldc6PwmDriver.h"


typedef struct {
    mcpwm_dev_t* mcpwm_num; // MCPWM unit number
    mcpwm_unit_t unit;
    mcpwm_timer_t timer;
    mcpwm_operator_t mcpwm_operator1;
    mcpwm_operator_t mcpwm_operator2;
    mcpwm_io_signals_t mcpwm_ah;
    mcpwm_io_signals_t mcpwm_bh;
    mcpwm_io_signals_t mcpwm_ch;
    mcpwm_io_signals_t mcpwm_al;
    mcpwm_io_signals_t mcpwm_bl;
    mcpwm_io_signals_t mcpwm_cl;
} bldc_config_t;

void pwm_timer_config(long pwm_freq, float dead_time, mcpwm_unit_t mcpwm_unit) {

    mcpwm_config_t pwm_config; // Initialize MCPWM configuration

    pwm_config.frequency = pwm_freq; // Set frequency of PWM signal
    pwm_config.counter_mode = MCPWM_UP_COUNTER; // Set counter mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // Set duty mode

    mcpwm_init(mcpwm_unit, MCPWM_TIMER_0, &pwm_config);
    mcpwm_init(mcpwm_unit, MCPWM_TIMER_1, &pwm_config);
    mcpwm_init(mcpwm_unit, MCPWM_TIMER_2, &pwm_config);

    // sync timers

    // // ------ READ THIS
    // // mcpwm_sync_signal_t is defined only to SYNC0/1/2 that are for GPIO sync
    // // In order to sync Timer1 and Timer2 with Timer0 
    // // Pass 1 to mcpwm_sync_signal_t sync_sig in mcpwm_sync_enable
    // // 1 is equal to "PWM timer0 sync_out" in PWM_TIMER_SYNCI_CFG_REG.PWM_TIMER1/2_SYNCISEL
    // // Phase (last parameter) is zero.
    // //https://gist.github.com/fabiovila/77c544286e9b0260a5a09823b47ba39f
    // mcpwm_sync_enable(mcpwm_unit, MCPWM_TIMER_0, 1, 0);
    // mcpwm_sync_enable(mcpwm_unit, MCPWM_TIMER_1, 1, 0);
    // mcpwm_sync_enable(mcpwm_unit, MCPWM_TIMER_2, 1, 0);

    // // since timer 1 and 2 are synced to timer 0, we must set up timer 0 first
    // mcpwm_num->timer[0].sync.out_sel = 1;


    // configure sync signals for timers
    mcpwm_sync_config_t sync_config = {
        .sync_sig = MCPWM_SELECT_TIMER0_SYNC, // Sync out signal with Timer 0
        .timer_val = 0, 
        .count_direction = MCPWM_TIMER_DIRECTION_UP, 
    };

    mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_0, &sync_config);
    mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_1, &sync_config);
    mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_2, &sync_config);

    // Set all timers to be the TEZ (timer equals 0) of timer 0
    mcpwm_set_timer_sync_output(mcpwm_unit, MCPWM_TIMER_0, MCPWM_SWSYNC_SOURCE_TEZ); // Set Timer 0 as sync output
}

void pwm_config(long pwm_freq, float dead_time, int GPIO_A_H, int GPIO_A_L, int GPIO_B_H, int GPIO_B_L, int GPIO_C_H, int GPIO_C_L){
    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = GPIO_A_H,
        .mcpwm0b_out_num = GPIO_A_L,
        .mcpwm1a_out_num = GPIO_B_H,
        .mcpwm1b_out_num = GPIO_B_L,
        .mcpwm2a_out_num = GPIO_C_H,
        .mcpwm2b_out_num = GPIO_C_L,
    };
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config); // need to correct the hardcoded MCPWM_UNIT_0

    pwm_timer_config(pwm_freq, dead_time, MCPWM_UNIT_0);
    // MCPWM_Unit_1 -> Second MCPWM unit is used for capture
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_0, GPIO_MCPWM_CAP0); // Set GPIO as CAP0
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_1, GPIO_MCPWM_CAP1); // Set GPIO as CAP1
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_2, GPIO_MCPWM_CAP2); // Set GPIO as CAP2


    // NOT USED YET
    // mcpwm_pin_config_t pin_config_capture = {
    //     .mcpwm_cap0_in_num = GPIO_CAP0,
    //     .mcpwm_cap1_in_num = GPIO_CAP1,
    //     .mcpwm_cap2_in_num = GPIO_CAP2,
    // };
}


void pwm_write(float duty_A, float duty_B, float duty_C, int GPIO_A_H, int GPIO_A_L, int GPIO_B_H, int GPIO_B_L, int GPIO_C_H, int GPIO_C_L){
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, duty_A*100.0); 
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, duty_B*100.0); 
    
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0A, duty_B*100.0); 
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0B, duty_B*100.0); 

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0A, duty_C*100.0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0B, duty_C*100.0);
}
