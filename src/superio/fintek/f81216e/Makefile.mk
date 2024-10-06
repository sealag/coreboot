# SPDX-License-Identifier: GPL-2.0-or-later

bootblock-$(CONFIG_SUPERIO_FINTEK_F81216E) += early_init.c
romstage-$(CONFIG_SUPERIO_FINTEK_F81216E) += early_init.c
ramstage-$(CONFIG_SUPERIO_FINTEK_F81216E) += superio.c

smm-$(CONFIG_SUPERIO_FINTEK_F81216E) += early_init.c
