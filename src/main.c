#include "clock.h"
#include "ldo.h"
#include "watchdog.h"
#include "delay.h"
#include "isp.h"
#include "uart.h"
#include "stdio.h"
#include "gpio.h"
#include "matrix.h"
#include "pwm.h"
#include "usb.h"
#include "debug.h"
#include "utils.h"
#include "keyboard.h"

void init()
{
    ldo_init();
    clock_init();
    gpio_init();
    uart_init();
    pwm_init();
    usb_init();
    matrix_init();
    keyboard_init();

    // usb interupt priority - 3
    IPH1 |= (1 << 6);
    IPL1 |= (1 << 6);

    // uart interupt priority - 2
    IPH1 |= (1 << 6);
    IPL1 |= (0 << 6);

    EA = 1; // enable interrupts
}

void main()
{
    init();

    dprintf("SMK v" TOSTRING(SMK_VERSION) "\r\n");
    dprintf("DEVICE vId:" TOSTRING(USB_VID) " pId:" TOSTRING(USB_PID) "\n\r");
    dprintf("OS: 0x%x, CONN: 0x%x\n\r", keyboard_state.os_mode, keyboard_state.conn_mode);

    // enable pwm and interrupt (driving matrix scan)
    pwm_enable();

    while (1) {
        CLR_WDT();

        matrix_task();
    }
}
