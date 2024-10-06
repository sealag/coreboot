/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/device.h>
#include <device/pnp.h>
#include <superio/conf_mode.h>

#include "chip.h"
#include "f81216e.h"

static void pnp_enter_ext_func_mode(struct device *dev)
{
	const struct superio_fintek_f81216e_config *conf = dev->chip_info;

	outb(conf->conf_entry_key, dev->path.pnp.port);
	outb(conf->conf_entry_key, dev->path.pnp.port);
}

static void pnp_exit_ext_func_mode(struct device *dev)
{
	outb(0xaa, dev->path.pnp.port);
}

static const struct pnp_mode_ops pnp_conf_mode_ops = {
	.enter_conf_mode = pnp_enter_ext_func_mode,
	.exit_conf_mode = pnp_exit_ext_func_mode,
};

static void f81216e_init(struct device *dev)
{
	if (!dev->enabled)
		return;

	switch (dev->path.pnp.device) {
	case F81216E_SP1:
	case F81216E_SP2:
	case F81216E_SP3:
	case F81216E_SP4:
	case F81216E_WDT:
	case F81216E_GPIO:
		break;
	}
}

static struct device_operations ops = {
	.read_resources   = pnp_read_resources,
	.set_resources    = pnp_set_resources,
	.enable_resources = pnp_enable_resources,
	.enable           = pnp_alt_enable,
	.init             = f81216e_init,
	.ops_pnp_mode     = &pnp_conf_mode_ops,
};

static struct pnp_info pnp_dev_info[] = {
	{ NULL, F81216E_SP1,  PNP_IO0 | PNP_IRQ0, 0xfff8, },
	{ NULL, F81216E_SP2,  PNP_IO0 | PNP_IRQ0, 0xfff8, },
	{ NULL, F81216E_SP3,  PNP_IO0 | PNP_IRQ0, 0xfff8, },
	{ NULL, F81216E_SP4,  PNP_IO0 | PNP_IRQ0, 0xfff8, },
	{ NULL, F81216E_WDT, },
	{ NULL, F81216E_GPIO, },
};

static void enable_dev(struct device *dev)
{
	pnp_enable_devices(dev, &ops, ARRAY_SIZE(pnp_dev_info), pnp_dev_info);
}

struct chip_operations superio_fintek_f81216e_ops = {
	.name = "Fintek F81216E Super I/O",
	.enable_dev = enable_dev
};
