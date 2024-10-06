#ifndef MAINBOARD_EFI_CONFIG_H
#define MAINBOARD_EFI_CONFIG_H

typedef struct {
	UINT8 PL1;
	UINT8 PL2;
} ADVANCED_CPU_CONFIGURATION;

typedef struct {
	UINT8   Mode;
	BOOLEAN LimitSlewRate;
	BOOLEAN EnableBiasTermResistors;
} PORT_CONFIG;

typedef struct {
	PORT_CONFIG Port[4];
} SERIAL_PORT_CONFIGURATION;

enum cb_err mainboard_get_advanced_cpu_config(ADVANCED_CPU_CONFIGURATION *cfg);
enum cb_err mainboard_get_serial_port_config(SERIAL_PORT_CONFIGURATION *cfg);

#endif /* MAINBOARD_EFI_CONFIG_H */
