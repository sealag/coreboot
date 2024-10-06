#include <console/console.h>
#include <superio/fintek/f81216e/f81216e.h>

#include "superio.h"

#define GPIO_DEV	PNP_DEV(0x2e, F81216E_GPIO)
#define ENTRY_KEY	CONF_ENTRY_KEY_6767

void mainboard_configure_serial_port(int port, uint8_t mode,
				     bool limit_slew_rate, bool bias_enable)
{
	int gpio_mode1, gpio_mode0, gpio_slew, gpio_r_dis;

	printk(BIOS_DEBUG, "%s: Configuring COM%d: Mode %d, LimitSlewRate %d, "
	       "EnableBiasTermResistors %d\n", __func__, port + 1, mode,
	       limit_slew_rate, bias_enable);

	switch (port) {
	case 0:
		gpio_mode1 = 30;
		gpio_mode0 = 31;
		gpio_slew = 26;
		gpio_r_dis = 27;
		break;
	case 1:
		gpio_mode1 = 21;
		gpio_mode0 = 23;
		gpio_slew = 17;
		gpio_r_dis = 20;
		break;
	case 2:
		gpio_mode1 = 32;
		gpio_mode0 = 16;
		gpio_slew = 13;
		gpio_r_dis = 33;
		break;
	case 3:
		gpio_mode1 = 10;
		gpio_mode0 = 11;
		gpio_slew = 6;
		gpio_r_dis = 7;
		break;
	default:
		return;
	}

	if (mode == COM_MODE_SELECT_RS232) {
		bias_enable = false;
	} else if (mode == COM_MODE_SELECT_DISABLED) {
		limit_slew_rate = false;
		bias_enable = false;
	}

	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, gpio_mode1, !!(mode & 0x2));
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, gpio_mode0, !!(mode & 0x1));
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, gpio_slew, !limit_slew_rate);
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, gpio_r_dis, !bias_enable);
}

void mainboard_set_status_led(uint8_t state)
{
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, 0, !(state & STATUS_LED_RED));
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, 1, !(state & STATUS_LED_GREEN));
	f81216e_gpio_output(GPIO_DEV, ENTRY_KEY, 2, !(state & STATUS_LED_BLUE));
}
