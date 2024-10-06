#ifndef MAINBOARD_SUPERIO_H
#define MAINBOARD_SUPERIO_H

#include <stdbool.h>
#include <stdint.h>

#define COM_MODE_SELECT_RS232		0x0
#define COM_MODE_SELECT_RS485		0x1
#define COM_MODE_SELECT_DISABLED	0x3

#define STATUS_LED_OFF			0x0
#define STATUS_LED_RED			0x1
#define STATUS_LED_GREEN		0x2
#define STATUS_LED_BLUE			0x4
#define STATUS_LED_YELLOW		(STATUS_LED_RED | STATUS_LED_GREEN)
#define STATUS_LED_PURPLE		(STATUS_LED_RED | STATUS_LED_BLUE)
#define STATUS_LED_CYAN			(STATUS_LED_GREEN | STATUS_LED_BLUE)
#define STATUS_LED_WHITE		(STATUS_LED_RED | STATUS_LED_GREEN | STATUS_LED_BLUE)

void mainboard_configure_serial_port(int port, uint8_t mode,
				     bool limit_slew_rate, bool bias_enable);
void mainboard_set_status_led(uint8_t state);

#endif /* MAINBOARD_SUPERIO_H */
