/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * Include this file into a mainboard's DSDT _SB device tree and it will
 * expose the F81216E LPC/eSPI to 4 UART and some of its functionality.
 *
 * It allows the change of IO ports, IRQs and DMA settings on logical
 * devices, disabling and reenabling logical devices and controlling power
 * saving mode on logical devices or the whole chip.
 *
 * LDN				State
 * 0x0 UARTA			Implemented
 * 0x1 UARTB			Implemented
 * 0x2 UARTC			Implemented
 * 0x3 UARTD			Implemented
 * 0x8 WDT			Not implemented
 * 0x9 GPIO			Not implemented
 *
 * Controllable through preprocessor defines:
 * SUPERIO_DEV		Device identifier for this SIO (e.g. SIO0)
 * SUPERIO_PNP_BASE	I/O address of the first PnP configuration register
 * F81216E_ENTRY_KEY	Key used to enter PNP configuration mode.
 * F81216E_SHOW_UARTA	If defined, UARTA will be exposed.
 * F81216E_SHOW_UARTB	If defined, UARTB will be exposed.
 * F81216E_SHOW_UARTC	If defined, UARTC will be exposed.
 * F81216E_SHOW_UARTD	If defined, UARTD will be exposed.
 */

#undef SUPERIO_CHIP_NAME
#define SUPERIO_CHIP_NAME F81216E
#include <superio/acpi/pnp.asl>

#undef PNP_DEFAULT_PSC
#define PNP_DEFAULT_PSC Return (0) /* no power management */

Device(SUPERIO_DEV) {
	Name (_HID, EisaId("PNP0A05"))
	Name (_STR, Unicode("Fintek F81216E LPC/eSPI to 4 UART"))
	Name (_UID, SUPERIO_UID(SUPERIO_DEV,))

	/* Mutex for accesses to the configuration ports */
	Mutex(CRMX, 1)

	/* SuperIO configuration ports */
	OperationRegion (CREG, SystemIO, SUPERIO_PNP_BASE, 0x02)
	Field (CREG, ByteAcc, NoLock, Preserve)
	{
		PNP_ADDR_REG,	8,
		PNP_DATA_REG,	8
	}
	IndexField (PNP_ADDR_REG, PNP_DATA_REG, ByteAcc, NoLock, Preserve)
	{
		Offset (0x07),
		PNP_LOGICAL_DEVICE,	8, /* Logical device selector */

		Offset (0x30),
		PNP_DEVICE_ACTIVE,	1, /* Logical device activation */

		Offset (0x60),
		PNP_IO0_HIGH_BYTE,	8, /* First I/O port base - high byte */
		PNP_IO0_LOW_BYTE,	8, /* First I/O port base - low byte */

		Offset (0x70),
		PNP_IRQ0,		8, /* First IRQ */
	}

	Method(_CRS)
	{
		/* Announce the used i/o ports to the OS */
		Return (ResourceTemplate () {
			IO (Decode16, SUPERIO_PNP_BASE, SUPERIO_PNP_BASE, 0x01, 0x02)
		})
	}

	#undef PNP_ENTER_MAGIC_1ST
	#undef PNP_ENTER_MAGIC_2ND
	#undef PNP_ENTER_MAGIC_3RD
	#undef PNP_ENTER_MAGIC_4TH
	#undef PNP_EXIT_MAGIC_1ST
	#ifndef F81216E_ENTRY_KEY
	#define F81216E_ENTRY_KEY	0x67
	#endif
	#define PNP_ENTER_MAGIC_1ST	F81216E_ENTRY_KEY
	#define PNP_ENTER_MAGIC_2ND	F81216E_ENTRY_KEY
	#define PNP_EXIT_MAGIC_1ST	0xaa
	#include <superio/acpi/pnp_config.asl>

#ifdef F81216E_SHOW_UARTA
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 0
	#include <superio/acpi/pnp_uart.asl>
#endif

#ifdef F81216E_SHOW_UARTB
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 1
	#include <superio/acpi/pnp_uart.asl>
#endif

#ifdef F81216E_SHOW_UARTC
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 2
	#include <superio/acpi/pnp_uart.asl>
#endif

#ifdef F81216E_SHOW_UARTD
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 3
	#include <superio/acpi/pnp_uart.asl>
#endif
}
