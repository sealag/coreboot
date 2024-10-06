/* SPDX-License-Identifier: GPL-2.0-only */

#include <soc/gpio.h>

Method (MPTS, 1)
{
	/* Assert PERST# */
	\_SB.PCI0.CTXS(GPP_F16);
	Sleep(20);

	/* Assert RESET# */
	\_SB.PCI0.CTXS(GPP_F12);
	Sleep(20);

	/* Assert FULL_CARD_POWER_OFF# */
	\_SB.PCI0.CTXS(GPP_F15);
}
