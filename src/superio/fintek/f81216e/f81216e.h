/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef SUPERIO_FINTEK_F81216E_H
#define SUPERIO_FINTEK_F81216E_H

#include <device/pnp_type.h>

/* Logical Device Numbers (LDN). */
#define F81216E_SP1	0x00	/* UART1 */
#define F81216E_SP2	0x01	/* UART2 */
#define F81216E_SP3	0x02	/* UART3 */
#define F81216E_SP4	0x03	/* UART4 */
#define F81216E_WDT	0x08	/* Watchdog */
#define F81216E_GPIO	0x09	/* GPIO */

/*
 * The PNP config entry key is set with two strapping
 * resistors with 0x67 as the default key.
 * See page 29 of the datasheet for details.
 */
enum f81216e_conf_entry_key {
	CONF_ENTRY_KEY_7777 = 0x77,
	CONF_ENTRY_KEY_A0A0 = 0xA0,
	CONF_ENTRY_KEY_8787 = 0x87,
	CONF_ENTRY_KEY_6767 = 0x67,
};

enum f81216e_pinmux {
	PINMUX_GPIO_ALL = 0,
	PINMUX_UART_SIN_SOUT = 1,
	PINMUX_UART_SIN_SOUT_RTS = 2,
	PINMUX_UART_ALL = 3,
};

void f81216e_enable_serial(pnp_devfn_t dev, uint16_t iobase,
			   enum f81216e_conf_entry_key k);
void f81216e_set_pinmux(pnp_devfn_t dev, enum f81216e_conf_entry_key k,
			enum f81216e_pinmux mux);
void f81216e_gpio_output(pnp_devfn_t dev, enum f81216e_conf_entry_key k,
			 uint8_t gpio, uint8_t value);

#endif /* SUPERIO_FINTEK_F81216E_H */
