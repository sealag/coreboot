/* SPDX-License-Identifier: GPL-2.0-or-later */

bootblock-y += bootblock.c
bootblock-y += superio.c

romstage-y += romstage.c
romstage-y += efi_config.c
romstage-y += superio.c

ramstage-y += mainboard.c
ramstage-y += efi_config.c
ramstage-y += smbios.c

smm-y += smihandler.c
smm-y += superio.c

#subdirs-y += memory
#subdirs-y += spd

subdirs-y += spd
