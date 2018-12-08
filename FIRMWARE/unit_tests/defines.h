/*
 * defines.h
 *
 *  Created on: Dec 7, 2018
 *      Author: maxim
 */

#ifndef DEFINES_H_
#define DEFINES_H_
#define SPI_IF_BIT_RATE  30*100000 // TODO: figure out what the maximum SPI speed is and set this as fast as possible.
#define TR_BUFF_SIZE     100
#define UART_BAUD_RATE   115200

#define PIN_YD  PIN_04 // GPIO13, 10 on MOD
#define PORT_YD GPIOA1_BASE
#define MASK_YD 0x20

#define PIN_YU  PIN_59 // GPIO4, 49 on MOD
#define PORT_YU GPIOA0_BASE
#define MASK_YU 0x10
#define ADC_YU  ADC_CH_2

#define PIN_XR  PIN_15 //GPIO22, 11 on MOD
#define PORT_XR GPIOA2_BASE
#define MASK_XR 0x40

#define PIN_XL  PIN_58 // 48 on MOD
#define PORT_XL GPIOA0_BASE
#define MASK_XL 0x08
#define ADC_XL  ADC_CH_1

#define PIN_DC  PIN_03 // LCD D/C (RS), GPIO12
#define PORT_DC GPIOA1_BASE
#define MASK_DC 0x10

#define NO_OF_SAMPLES        8 // ADC
#define SERVO_RELOAD_VALUE   65536
#define BUZZER_RELOAD_VALUE  40000


// define some basic functions:
#define min(a,b) ((a)>(b)?(b):(a))
#define max(a,b) ((a)>(b)?(a):(b))
#define delay(i) MAP_UtilsDelay(14000*i)

#define print(a)    Message(a)
#define printf(a, ...) Report((a), ...) // TODO figure out this

// use arduino macro if you prefer:
#define OUTPUT GPIO_DIR_MODE_OUT
#define INPUT  GPIO_DIR_MODE_IN

#endif /* DEFINES_H_ */
