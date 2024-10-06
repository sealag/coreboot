/* SPDX-License-Identifier: GPL-2.0-only */

#include <smbios.h>
#include <spi_flash.h>
#include <string.h>

#define OEM_AREA_SIZE	256
#define OEM_AREA_OFFSET	(4096 - OEM_AREA_SIZE)

/*
 * The OEM area in the flash descriptor contains an array
 * of records with the following structure.
 */
struct oem_mapping {
	char magic[4]; /* "OEM" */
	char serial_number[16];
	uint8_t uuid[16];
} __packed;

/* Local buffer to read the OEM area in the flash descriptor */
static char oem_area_data[OEM_AREA_SIZE];
static struct oem_mapping *oem_area = (void *)&oem_area_data;
static bool oem_area_read = false;
static bool oem_area_valid = false;

static void read_oem_area(void)
{
	const struct spi_flash *flash = boot_device_spi_flash();

	if (oem_area_read)
		return;

	if (spi_flash_read(flash, OEM_AREA_OFFSET, OEM_AREA_SIZE, oem_area_data))
		return;

	oem_area_read = true;
	oem_area_valid = !strncmp(oem_area->magic, "OEM", 3);
}

const char *smbios_mainboard_serial_number(void)
{
	read_oem_area();

	if (!oem_area_valid)
		return CONFIG_MAINBOARD_SERIAL_NUMBER;

	/* Make sure the string is null-terminated */
	oem_area->serial_number[ARRAY_SIZE(oem_area->serial_number) - 1] = '\0';

	return oem_area->serial_number;
}

void smbios_system_set_uuid(uint8_t *uuid)
{
	read_oem_area();

	if (!oem_area_valid)
		return;

	memcpy(uuid, oem_area->uuid, sizeof(oem_area->uuid));
}
