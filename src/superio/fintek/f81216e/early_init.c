/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <arch/io.h>
#include <device/pnp_ops.h>
#include <device/pnp.h>
#include <stdint.h>
#include "f81216e.h"

#define F81216E_EXIT_KEY		0xAA

#define F81216E_PINMUX_REG		0x28
#define F81216E_PINMUX_SEL_SHIFT(uart)	((uart) * 2)
#define F81216E_PINMUX_SEL_MASK(uart)	(3 << F81216E_PINMUX_SEL_SHIFT(uart))

#define F81216E_GPIO_BASE(bank)		(0xF0 - ((bank) * 0x10))
#define F81216E_GPIO_OE_REG		0x0
#define F81216E_GPIO_DATA_REG		0x1

/* Enable configuration: pass entry key into index port dev. */
static void pnp_enter_conf_state(pnp_devfn_t dev, enum f81216e_conf_entry_key k)
{
	uint16_t port = dev >> 8;
	outb(k, port);
	outb(k, port);
}

/* Disable configuration: pass exit key '0xAA' into index port dev. */
static void pnp_exit_conf_state(pnp_devfn_t dev)
{
	uint16_t port = dev >> 8;
	outb(F81216E_EXIT_KEY, port);
}

/* Bring up early serial debugging output before the RAM is initialized. */
void f81216e_enable_serial(pnp_devfn_t dev, uint16_t iobase,
			   enum f81216e_conf_entry_key k)
{
	pnp_enter_conf_state(dev, k);
	pnp_set_logical_device(dev);
	pnp_set_enable(dev, 0);
	pnp_set_iobase(dev, PNP_IDX_IO0, iobase);
	pnp_set_enable(dev, 1);
	pnp_exit_conf_state(dev);
}

/* Configure the UART pinmux. */
void f81216e_set_pinmux(pnp_devfn_t dev, enum f81216e_conf_entry_key k,
			enum f81216e_pinmux mux)
{
	uint8_t index = dev & 0x3;

	pnp_enter_conf_state(dev, k);
	pnp_set_logical_device(dev);
	pnp_unset_and_set_config(dev, F81216E_PINMUX_REG,
				 F81216E_PINMUX_SEL_MASK(index),
				 mux << F81216E_PINMUX_SEL_SHIFT(index));
	pnp_exit_conf_state(dev);
}

void f81216e_gpio_output(pnp_devfn_t dev, enum f81216e_conf_entry_key k,
			 uint8_t gpio, uint8_t value)
{
	uint8_t gpio_bank = gpio / 10;
	uint8_t gpio_num = gpio % 10;
	uint8_t base_reg = F81216E_GPIO_BASE(gpio_bank);

	pnp_enter_conf_state(dev, k);
	pnp_set_logical_device(dev);
	pnp_unset_and_set_config(dev, base_reg + F81216E_GPIO_OE_REG,
				 0, 1 << gpio_num);
	pnp_unset_and_set_config(dev, base_reg + F81216E_GPIO_DATA_REG,
				 1 << gpio_num, value << gpio_num);
	pnp_exit_conf_state(dev);
}
