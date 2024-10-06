/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <gpio.h>
#include <fsp/api.h>
#include <soc/gpio.h>
#include <soc/meminit.h>
#include <soc/romstage.h>

#include "efi_config.h"
#include "superio.h"

static const struct pad_config romstage_gpio_table[] = {
	/* E22 : WWAN_PWREN */
	PAD_CFG_GPO(GPP_E22, 1, DEEP),
	/* F15 : WWAN_FCPO# */
	PAD_CFG_GPO(GPP_F15, 1, DEEP),
};

static size_t get_spd_index(void)
{
	/*
	 * Memory configuration board straps
	 * MEMORYID[0]	GPP_B3
	 * MEMORYID[1]	GPP_A11
	 * MEMORYID[2]	GPP_A16
	 */
	gpio_t spd_gpios[] = {
		GPP_B3,
		GPP_A11,
		GPP_A16,
	};

	return gpio_base2_value(spd_gpios, ARRAY_SIZE(spd_gpios));
}

static void configure_serial_ports(void)
{
	SERIAL_PORT_CONFIGURATION cfg;
	enum cb_err ret;

	ret = mainboard_get_serial_port_config(&cfg);
	if (ret != CB_SUCCESS)
		return;

	for (int i = 0; i < 4; i++) {
		if (CONFIG_CONSOLE_SERIAL && CONFIG_UART_FOR_CONSOLE == i) {
			printk(BIOS_DEBUG,
			       "%s: port %d configured as console, skipping\n",
			       __func__, i);
		} else {
			mainboard_configure_serial_port(i, cfg.Port[i].Mode,
							cfg.Port[i].LimitSlewRate,
							cfg.Port[i].EnableBiasTermResistors);
		}
	}
}

void mainboard_memory_init_params(FSPM_UPD *memupd)
{
	static const struct mb_cfg lp5_mem_config = {
		.type = MEM_TYPE_LP5X,

		/* DQ byte map */
		.lpx_dq_map = {
			.ddr0 = {
				.dq0 = { 12,  9, 10, 11, 14, 13,  8, 15 },
				.dq1 = {  3,  1,  2,  0,  4,  7,  5,  6 },
			},
			.ddr1 = {
				.dq0 = {  3,  1,  2,  0,  4,  7,  5,  6 },
				.dq1 = { 13,  9,  8, 11, 10, 14, 15, 12 },
			},
			.ddr2 = {
				.dq0 = {  2,  1,  3,  0,  4,  6,  5,  7 },
				.dq1 = {  8,  9, 10, 11, 13, 14, 12, 15 },
			},
			.ddr3 = {
				.dq0 = {  3,  0,  1,  2,  5,  6,  4,  7 },
				.dq1 = { 13,  9, 11,  8, 14, 15, 10, 12 },
			},
			.ddr4 = {
				.dq0 = { 12,  9, 10, 11, 14, 13,  8, 15 },
				.dq1 = {  3,  1,  2,  0,  4,  7,  5,  6 },
			},
			.ddr5 = {
				.dq0 = {  3,  1,  2,  0,  4,  7,  5,  6 },
				.dq1 = { 13,  9,  8, 11, 10, 14, 15, 12 },
			},
			.ddr6 = {
				.dq0 = {  2,  1,  3,  0,  4,  6,  5,  7 },
				.dq1 = {  8,  9, 10, 11, 13, 14, 12, 15 },
			},
			.ddr7 = {
				.dq0 = {  3,  0,  1,  2,  5,  6,  4,  7 },
				.dq1 = { 13,  9, 11,  8, 14, 15, 10, 12 },
			},
		},

		/* DQS CPU<>DRAM map */
		.lpx_dqs_map = {
			.ddr0 = { .dqs0 = 1, .dqs1 = 0 },
			.ddr1 = { .dqs0 = 0, .dqs1 = 1 },
			.ddr2 = { .dqs0 = 0, .dqs1 = 1 },
			.ddr3 = { .dqs0 = 0, .dqs1 = 1 },
			.ddr4 = { .dqs0 = 1, .dqs1 = 0 },
			.ddr5 = { .dqs0 = 0, .dqs1 = 1 },
			.ddr6 = { .dqs0 = 0, .dqs1 = 1 },
			.ddr7 = { .dqs0 = 0, .dqs1 = 1 }
		},

		.ect = true, /* Early Command Training */

		.UserBd = BOARD_TYPE_ULT_ULX,

		.LpDdrDqDqsReTraining = 1,

		.lp5x_config = {
			.ccc_config = 0xff,
		},
	};

	const bool half_populated = true;

	const struct mem_spd memory_down_spd_info = {
		.topo = MEM_TOPO_MEMORY_DOWN,
		.cbfs_index = get_spd_index(),
	};

	memcfg_init(memupd, &lp5_mem_config, &memory_down_spd_info, half_populated);

	configure_serial_ports();

	mainboard_set_status_led(STATUS_LED_GREEN);

	gpio_configure_pads(romstage_gpio_table, ARRAY_SIZE(romstage_gpio_table));
}
