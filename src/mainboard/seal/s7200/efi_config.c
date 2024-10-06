#include <option.h>
#include <smmstore.h>
#include <string.h>
#include <console/console.h>
#include <drivers/efi/efivars.h>

#include <Uefi/UefiBaseType.h>

#include "efi_config.h"

static const EFI_GUID AdvancedCpuConfigNvDataGuid = {
	0x09b39a65, 0x7a37, 0x470d, {0xa0, 0xf7, 0x16, 0x7c, 0xd8, 0x2f, 0xf0, 0x6a}};
static const char *AdvancedCpuConfigVariableName = "AdvancedCpuConfig";

static const EFI_GUID SerialPortConfigNvDataGuid = {
	0xe055c87f, 0xe3aa, 0x4fb8, {0xba, 0xdb, 0x32, 0x2c, 0x32, 0xcb, 0xce, 0xc5}};
static const char *SerialPortConfigVariableName = "SerialPortConfig";

enum cb_err mainboard_get_advanced_cpu_config(ADVANCED_CPU_CONFIGURATION *cfg)
{
	struct region_device rdev;
	uint32_t size;
	enum cb_err ret;

	if (smmstore_lookup_region(&rdev)) {
		printk(BIOS_ERR,
		       "%s: failed to get SMMSTORE region\n", __func__);
		return CB_ERR;
	}

	size = sizeof(*cfg);
	ret = efi_fv_get_option(&rdev, &AdvancedCpuConfigNvDataGuid,
				AdvancedCpuConfigVariableName,
				cfg, &size);
	if (ret != CB_SUCCESS) {
		printk(BIOS_NOTICE,
		       "%s: failed to get AdvancedCpuConfig EFI variable, "
		       "using defaults\n", __func__);
		memset(cfg, 0, sizeof(*cfg));
		return ret;
	}

	return CB_SUCCESS;
}

enum cb_err mainboard_get_serial_port_config(SERIAL_PORT_CONFIGURATION *cfg)
{
	struct region_device rdev;
	uint32_t size;
	enum cb_err ret;

	if (smmstore_lookup_region(&rdev)) {
		printk(BIOS_ERR,
		       "%s: failed to get SMMSTORE region\n", __func__);
		return CB_ERR;
	}

	size = sizeof(*cfg);
	ret = efi_fv_get_option(&rdev, &SerialPortConfigNvDataGuid,
				SerialPortConfigVariableName,
				cfg, &size);
	if (ret != CB_SUCCESS) {
		printk(BIOS_NOTICE,
		       "%s: failed to get SerialPortConfig EFI variable, "
		       "using defaults\n", __func__);
		memset(cfg, 0, sizeof(*cfg));
	}

	return CB_SUCCESS;
}
