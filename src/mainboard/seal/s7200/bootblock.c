/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <bootblock_common.h>
#include <delay.h>
#include <intelblocks/lpc_lib.h>
#include <soc/gpio.h>
#include <soc/intel/common/block/lpc/lpc_def.h>
#include <superio/fintek/f81216e/f81216e.h>

#include "superio.h"

static const pnp_devfn_t serial_devs[] = {
	/* COM1 */
	PNP_DEV(0x2e, F81216E_SP1),
	/* COM2 */
	PNP_DEV(0x2e, F81216E_SP2),
	/* COM3 */
	PNP_DEV(0x2e, F81216E_SP3),
	/* COM4 */
	PNP_DEV(0x2e, F81216E_SP4),
};

static const struct pad_config early_gpio_table[] = {
	/* B3  : MEMORYID[0] */
	PAD_CFG_GPI(GPP_B3, NONE, DEEP),
	/* A11 : MEMORYID[1] */
	PAD_CFG_GPI(GPP_A11, NONE, DEEP),
	/* A16 : MEMORYID[2] */
	PAD_CFG_GPI(GPP_A16, NONE, DEEP),
};

static const struct pad_config early_wwan_gpio_table[] = {
	/* E22 : WWAN_PWREN */
	PAD_CFG_GPO(GPP_E22, 1, DEEP),

	/* F12 : WWAN_RST# (updated in ramstage) */
	PAD_CFG_GPO(GPP_F12, 0, DEEP),
	/* F16 : WWAN_PERST# (updated in ramstage) */
	PAD_CFG_GPO(GPP_F16, 0, DEEP),
	/* F15 : WWAN_FCPO# (updated in romstage) */
	PAD_CFG_GPO(GPP_F15, 0, DEEP),
};

static const void lpc_setup_io_ports(void)
{
	uint16_t com_ranges = LPC_IOD_COMA_RANGE | LPC_IOD_COMB_RANGE;
	uint16_t com_enable = LPC_IOE_COMA_EN | LPC_IOE_COMB_EN;
	uint16_t com_mask = LPC_IOD_COMA_RANGE_MASK | LPC_IOD_COMB_RANGE_MASK;

	/* COM1/2 */
	lpc_set_fixed_io_ranges(com_ranges, com_mask);
	lpc_enable_fixed_io_ranges(com_enable);

	lpc_enable_fixed_io_ranges(LPC_IOE_SUPERIO_2E_2F);

	/* COM3 */
	lpc_open_pmio_window(0x3e8, 8);

	/* COM4 */
	lpc_open_pmio_window(0x2e8, 8);
}

void bootblock_mainboard_early_init(void)
{
	gpio_configure_pads(early_gpio_table, ARRAY_SIZE(early_gpio_table));
	gpio_configure_pads(early_wwan_gpio_table, ARRAY_SIZE(early_wwan_gpio_table));

	/* Enable eSPI decoding for COM1-4 (3f8, 2f8, 3e8, 2e8) and Super I/O (2e) */
	lpc_setup_io_ports();

	/* Mux all Super I/O pins except for UARTx TX/RX as GPIOs */
	for (int i = 0; i < ARRAY_SIZE(serial_devs); i++)
		f81216e_set_pinmux(serial_devs[i], CONF_ENTRY_KEY_6767,
				   PINMUX_UART_SIN_SOUT);

	if (CONFIG(CONSOLE_SERIAL)) {
		mainboard_configure_serial_port(CONFIG_UART_FOR_CONSOLE,
						COM_MODE_SELECT_RS232,
						false, false);

		/* wait for the transceiver to turn on */
		mdelay(1);

		f81216e_enable_serial(serial_devs[CONFIG_UART_FOR_CONSOLE],
				      CONFIG_TTYS0_BASE, CONF_ENTRY_KEY_6767);
	}
}
