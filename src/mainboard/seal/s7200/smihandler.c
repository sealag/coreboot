/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <acpi/acpi.h>
#include <cpu/x86/smm.h>
#include <intelblocks/xhci.h>

#include "superio.h"

void mainboard_smi_sleep(uint8_t slp_typ)
{
	printk(BIOS_DEBUG, "SMI: sleep S%d\n", slp_typ);

	switch (slp_typ) {
	case ACPI_S0:
		mainboard_set_status_led(STATUS_LED_GREEN);
		break;
	case ACPI_S3:
		mainboard_set_status_led(STATUS_LED_YELLOW);
		break;
	case ACPI_S4:
	case ACPI_S5:
		mainboard_set_status_led(STATUS_LED_OFF);
		break;
	}

	/*
	 * Workaround: Reset the XHCI controller prior to S4/S5 to avoid
	 * PMC timeout error during power-on from S4/S5.
	 */
	if (slp_typ == ACPI_S4 || slp_typ == ACPI_S5)
		xhci_host_reset();
}
