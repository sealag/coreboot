/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <acpi/acpi.h>
#include <delay.h>
#include <soc/ramstage.h>
#include <static.h>

#include "efi_config.h"

/* Pad configuration in ramstage */
static const struct pad_config gpio_table[] = {
	/* A0  : ESPI_IO0 */
	PAD_CFG_NF(GPP_A0, NONE, DEEP, NF1),
	/* A1  : ESPI_IO1 */
	PAD_CFG_NF(GPP_A1, NONE, DEEP, NF1),
	/* A2  : ESPI_IO2 */
	PAD_CFG_NF(GPP_A2, NONE, DEEP, NF1),
	/* A3  : ESPI_IO3 */
	PAD_CFG_NF(GPP_A3, NONE, DEEP, NF1),
	/* A4  : ESPI_CS0# */
	PAD_CFG_NF(GPP_A4, NONE, DEEP, NF1),
	/* A5  : ESPI_ALERT0# */
	PAD_CFG_NF(GPP_A5, NONE, DEEP, NF1),
	/* A6  : NC */
	PAD_NC(GPP_A6, NONE),
	/* A7  : LAN1_I226_RST_N */
	PAD_CFG_GPO(GPP_A7, 1, DEEP),
	/* A8  : NC */
	PAD_NC(GPP_A8, NONE),
	/* A9  : ESPI_CLK */
	PAD_CFG_NF(GPP_A9, NONE, DEEP, NF1),
	/* A10 : ESPI_RESET# */
	PAD_CFG_NF(GPP_A10, NONE, DEEP, NF1),
	/* A11 : MEMORYID[1] (configured in bootblock) */
	/* A12 : SATAXPCIE1 ==> M2_SSD_PEDET */
	PAD_CFG_NF(GPP_A12, NONE, DEEP, NF1),
	/* A13 : NC */
	PAD_NC(GPP_A13, NONE),
	/* A14 : USB_OC1# ==> TCPD_OC1_N */
	PAD_CFG_NF(GPP_A14, NONE, DEEP, NF1),
	/* A15 : USB_OC2# ==> TCPD_OC2_N */
	PAD_CFG_NF(GPP_A15, NONE, DEEP, NF1),
	/* A16 : MEMORYID[2] (configured in bootblock) */
	/* A17 : NC */
	PAD_NC(GPP_A17, NONE),
	/* A18 : NC */
	PAD_NC(GPP_A18, NONE),
	/* A19 : NC */
	PAD_NC(GPP_A19, NONE),
	/* A20 : NC */
	PAD_NC(GPP_A20, NONE),
	/* A21 : NC */
	PAD_NC(GPP_A21, NONE),
	/* A22 : NC */
	PAD_NC(GPP_A22, NONE),
	/* A23 : NC */
	PAD_NC(GPP_A23, NONE),

	/* B0  : CORE_VID0 ==> VCCIN_AUX_VID0 */
	PAD_CFG_NF(GPP_B0, NONE, DEEP, NF1),
	/* B1  : CORE_VID1 ==> VCCIN_AUX_VID1 */
	PAD_CFG_NF(GPP_B1, NONE, DEEP, NF1),
	/* B2  : VRALERT# */
	PAD_CFG_NF(GPP_B2, NONE, DEEP, NF1),
	/* B3  : MEMORYID[0] (configured in bootblock) */
	/* B4  : NC */
	PAD_NC(GPP_B4, NONE),
	/* B5  : MBUS_NRST - TODO: open drain output? */
	/* B6  : MBUS_SWCLK_BOOT0  - TODO: open drain output? */
	/* B7  : USB_C_GPP_B7 ==> TCP0_AUX_N_BIAS_GPIO */
	PAD_CFG_NF(GPP_B7, NONE, DEEP, NF6),
	/* B8  : NC */
	PAD_NC(GPP_B8, NONE),
	/* B9  : Not available */
	PAD_NC(GPP_B9, NONE),
	/* B10 : Not available */
	PAD_NC(GPP_B10, NONE),
	/* B11 : PMCALERT# ==> USBC_PD_SML1_IRQ_N */
	PAD_CFG_NF(GPP_B11, NONE, DEEP, NF1),
	/* B12 : SLP_S0# */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* B13 : PLTRST# */
	PAD_CFG_NF(GPP_B13, NONE, PLTRST, NF1),
	/* B14 : GPP_B14_STRAP */
	PAD_NC(GPP_B14, NONE),
	/* B15 : NC */
	PAD_NC(GPP_B15, NONE),
	/* B16 : NC */
	PAD_NC(GPP_B16, NONE),
	/* B17 : NC */
	PAD_NC(GPP_B17, NONE),
	/* B18 : GPP_B18_STRAP */
	PAD_NC(GPP_B18, NONE),
	/* B19 : Not available */
	PAD_NC(GPP_B19, NONE),
	/* B20 : Not available */
	PAD_NC(GPP_B20, NONE),
	/* B21 : Not available */
	PAD_NC(GPP_B21, NONE),
	/* B22 : Not available */
	PAD_NC(GPP_B22, NONE),
	/* B23 : USB_C_GPP_B23 ==> TCP0_AUX_P_BIAS_GPIO */
	PAD_CFG_NF(GPP_B23, NONE, DEEP, NF6),

	/* C0  : NC */
	PAD_NC(GPP_C0, NONE),
	/* C1  : NC */
	PAD_NC(GPP_C1, NONE),
	/* C2  : GPP_C2_STRAP */
	PAD_NC(GPP_C2, NONE),
	/* C3  : NC */
	PAD_NC(GPP_C3, NONE),
	/* C4  : NC */
	PAD_NC(GPP_C4, NONE),
	/* C5  : GPP_C5_STRAP */
	PAD_NC(GPP_C5, NONE),
	/* C6  : SML1CLK */
	PAD_CFG_NF(GPP_C6, NONE, DEEP, NF1),
	/* C7  : SML1DATA */
	PAD_CFG_NF(GPP_C7, NONE, DEEP, NF1),

	/* D0  : NC */
	PAD_NC(GPP_D0, NONE),
	/* D1  : NC */
	PAD_NC(GPP_D1, NONE),
	/* D2  : NC */
	PAD_NC(GPP_D2, NONE),
	/* D3  : NC */
	PAD_NC(GPP_D3, NONE),
	/* D4  : USB_C_GPP_D4 ==> TCP1_AUX_N_BIAS_GPIO */
	PAD_CFG_NF(GPP_D4, NONE, DEEP, NF6),
	/* D5  : SRCCLKREQ0# ==> CLKREQ0_M2_KEYM_SSD_N */
	PAD_CFG_NF(GPP_D5, NONE, DEEP, NF1),
	/* D6  : SRCCLKREQ1# ==> CLKREQ1_M2_KEYB_WWAN_N */
	PAD_CFG_NF(GPP_D6, NONE, DEEP, NF1),
	/* D7  : SRCCLKREQ2# ==> NC (I210 CLKREQ) */
	PAD_CFG_NF(GPP_D7, DN_20K, DEEP, NF1),
	/* D8  : SRCCLKREQ3# ==> CLKREQ3_LAN0_I226_N */
	PAD_CFG_NF(GPP_D8, NONE, DEEP, NF1),
	/* D9  : NC */
	PAD_NC(GPP_D9, NONE),
	/* D10 : NC */
	PAD_NC(GPP_D10, NONE),
	/* D11 : NC */
	PAD_NC(GPP_D11, NONE),
	/* D12 : NC */
	PAD_NC(GPP_D12, NONE),
	/* D13 : NC */
	PAD_NC(GPP_D13, NONE),
	/* D14 : NC */
	PAD_NC(GPP_D14, NONE),
	/* D15 : USB_C_GPP_D15 ==> TCP1_AUX_P_BIAS_GPIO */
	PAD_CFG_NF(GPP_D15, NONE, DEEP, NF6),
	/* D16 : LAN2_DISABLE_N */
	PAD_CFG_GPO(GPP_D16, 1, DEEP),
	/* D17 : NC */
	PAD_NC(GPP_D17, NONE),
	/* D18 : NC */
	PAD_NC(GPP_D18, NONE),
	/* D19 : NC */
	PAD_NC(GPP_D19, NONE),

	/* E0  : NC */
	PAD_NC(GPP_E0, NONE),
	/* E1  : NC */
	PAD_NC(GPP_E1, NONE),
	/* E2  : NC */
	PAD_NC(GPP_E2, NONE),
	/* E3  : NC */
	PAD_NC(GPP_E3, NONE),
	/* E4  : LAN0_DISABLE_N */
	PAD_CFG_GPO(GPP_E4, 1, DEEP),
	/* E5  : LAN1_DISABLE_N */
	PAD_CFG_GPO(GPP_E5, 1, DEEP),
	/* E6  : GPP_E6_STRAP */
	PAD_NC(GPP_E6, NONE),
	/* E7  : NC */
	PAD_NC(GPP_E7, NONE),
	/* E8  : NC */
	PAD_NC(GPP_E8, NONE),
	/* E9  : NC */
	PAD_NC(GPP_E9, NONE),
	/* E10 : NC */
	PAD_NC(GPP_E10, NONE),
	/* E11 : M2_PCH_SSD_PWREN */
	PAD_CFG_GPO(GPP_E11, 1, DEEP),
	/* E12 : NC */
	PAD_NC(GPP_E12, NONE),
	/* E13 : NC */
	PAD_NC(GPP_E13, NONE),
	/* E14 : BT_RF_KILL_N */
	PAD_CFG_GPO(GPP_E14, 1, DEEP),
	/* E15 : LAN2_I210_RST_N */
	PAD_CFG_GPO(GPP_E15, 1, DEEP),
	/* E16 : M2_SSD_RST_N */
	PAD_CFG_GPO(GPP_E16, 1, DEEP),
	/* E17 : NC */
	PAD_NC(GPP_E17, NONE),
	/* E18 : NC */
	PAD_NC(GPP_E18, NONE),
	/* E19 : NC */
	PAD_NC(GPP_E19, NONE),
	/* E20 : NC */
	PAD_NC(GPP_E20, NONE),
	/* E21 : NC */
	PAD_NC(GPP_E21, NONE),
	/* E22 : WWAN_PWREN (configured in bootblock) */
	/* E23 : WIFI_RF_KILL_N */
	PAD_CFG_GPO(GPP_E23, 1, DEEP),

	/* F0  : CNV_BRI_DT */
	PAD_CFG_NF(GPP_F0, NONE, DEEP, NF1),
	/* F1  : CNV_BRI_RSP */
	PAD_CFG_NF(GPP_F1, UP_20K, DEEP, NF1),
	/* F2  : CNV_RGI_DT */
	PAD_CFG_NF(GPP_F1, NONE, DEEP, NF1),
	/* F3  : CNV_RGI_RSP */
	PAD_CFG_NF(GPP_F1, UP_20K, DEEP, NF1),
	/* F4  : CNV_RF_RESET# */
	PAD_CFG_NF(GPP_F4, NONE, DEEP, NF1),
	/* F5  : MODEM_CLKREQ ==> CNV_CLKREQ */
	PAD_CFG_NF(GPP_F5, NONE, DEEP, NF2),
	/* F6  : NC */
	PAD_NC(GPP_F6, NONE),
	/* F7  : NC */
	PAD_NC(GPP_F7, NONE),
	/* F8  : Not available */
	PAD_NC(GPP_F8, NONE),
	/* F9  : Not available */
	PAD_NC(GPP_F9, NONE),
	/* F10 : NC */
	PAD_NC(GPP_F10, NONE),
	/* F11 : NC */
	PAD_NC(GPP_F11, NONE),
	/* F12 : M2_WWAN_RST_N */
	PAD_CFG_GPO(GPP_F12, 1, DEEP),
	/* F13 : USBC_PD_I2C_IRQ_N */
	PAD_NC(GPP_F13, NONE),
	/* F14 : M2_WWAN_DISABLE_N */
	PAD_CFG_GPO(GPP_F14, 1, DEEP),
	/* F15 : M2_WWAN_FCP_OFF_N */
	PAD_CFG_GPO(GPP_F16, 1, DEEP),
	/* F16 : M2_WWAN_PERST_N */
	PAD_CFG_GPO(GPP_F16, 1, DEEP),
	/* F17 : NC */
	PAD_NC(GPP_F17, NONE),
	/* F18 : NC */
	PAD_NC(GPP_F18, NONE),
	/* F19 : Not available */
	PAD_NC(GPP_F19, NONE),
	/* F20 : Not available */
	PAD_NC(GPP_F20, NONE),
	/* F21 : Not available */
	PAD_NC(GPP_F21, NONE),
	/* F22 : NC */
	PAD_NC(GPP_F22, NONE),
	/* F23 : NC */
	PAD_NC(GPP_F23, NONE),

	/* H0  : GPP_H0_STRAP */
	PAD_NC(GPP_H0, NONE),
	/* H1  : GPP_H1_STRAP */
	PAD_NC(GPP_H1, NONE),
	/* H2  : GPP_H2_STRAP */
	PAD_NC(GPP_H2, NONE),
	/* H3  : NC */
	PAD_NC(GPP_H3, NONE),
	/* H4  : I2C0_SDA ==> I2C0_SDA */
	PAD_CFG_NF(GPP_H4, NONE, DEEP, NF1),
	/* H5  : I2C0_SCL ==> I2C0_SCL */
	PAD_CFG_NF(GPP_H5, NONE, DEEP, NF1),
	/* H6  : NC */
	PAD_NC(GPP_H6, NONE),
	/* H7  : NC */
	PAD_NC(GPP_H7, NONE),
	/* H8  : NC */
	PAD_NC(GPP_H8, NONE),
	/* H9  : NC */
	PAD_NC(GPP_H9, NONE),
	/* H10 : NC ==> UART0_DBG_RX */
	PAD_NC(GPP_H10, NONE),
	/* H11 : NC ==> UART0_DBG_TX */
	PAD_NC(GPP_H11, NONE),
	/* H12 : NC */
	PAD_NC(GPP_H12, NONE),
	/* H13 : SATA_DEVSLP1B ==> M2_SSD_DEVSLP */
	PAD_CFG_NF(GPP_H13, NONE, DEEP, NF5),
	/* H14 : Not available */
	PAD_NC(GPP_H14, NONE),
	/* H15 : NC */
	PAD_NC(GPP_H15, NONE),
	/* H16 : Not available */
	PAD_NC(GPP_H16, NONE),
	/* H17 : NC */
	PAD_NC(GPP_H17, NONE),
	/* H18 : PROC_C10_GATE# ==> PROC_C10_GATE_N */
	PAD_CFG_NF(GPP_H18, NONE, DEEP, NF1),
	/* H19 : SRCCLKREQ4# ==> CLKREQ4_LAN1_I226_N */
	PAD_CFG_NF(GPP_H19, NONE, DEEP, NF1),
	/* H20 : NC */
	PAD_NC(GPP_H20, NONE),
	/* H21 : NC */
	PAD_NC(GPP_H21, NONE),
	/* H22 : NC */
	PAD_NC(GPP_H22, NONE),
	/* H23 : LAN0_I226_RST_N */
	PAD_CFG_GPO(GPP_H23, 1, DEEP),

	/* I5  : NC */
	PAD_NC(GPP_I5, NONE),
	/* I7  : NC */
	PAD_NC(GPP_I7, NONE),
	/* I8  : NC */
	PAD_NC(GPP_I8, NONE),
	/* I9  : NC */
	PAD_NC(GPP_I9, NONE),
	/* I10 : NC */
	PAD_NC(GPP_I10, NONE),
	/* I11 : NC */
	PAD_NC(GPP_I11, NONE),
	/* I12 : NC */
	PAD_NC(GPP_I12, NONE),
	/* I13 : NC */
	PAD_NC(GPP_I13, NONE),
	/* I14 : NC */
	PAD_NC(GPP_I14, NONE),
	/* I15 : NC */
	PAD_NC(GPP_I15, NONE),
	/* I16 : NC */
	PAD_NC(GPP_I16, NONE),
	/* I17 : NC */
	PAD_NC(GPP_I17, NONE),
	/* I18 : NC */
	PAD_NC(GPP_I18, NONE),

	/* R0  : NC */
	PAD_NC(GPP_R0, NONE),
	/* R1  : NC */
	PAD_NC(GPP_R1, NONE),
	/* R2  : GPP_R2_STRAP */
	PAD_NC(GPP_R2, NONE),
	/* R3  : NC */
	PAD_NC(GPP_R3, NONE),
	/* R4  : NC */
	PAD_NC(GPP_R4, NONE),
	/* R5  : NC */
	PAD_NC(GPP_R5, NONE),
	/* R6  : NC */
	PAD_NC(GPP_R6, NONE),
	/* R7  : NC */
	PAD_NC(GPP_R7, NONE),

	/* S0  : NC */
	PAD_NC(GPP_S0, NONE),
	/* S1  : NC */
	PAD_NC(GPP_S1, NONE),
	/* S2  : NC */
	PAD_NC(GPP_S2, NONE),
	/* S3  : NC */
	PAD_NC(GPP_S3, NONE),
	/* S4  : NC */
	PAD_NC(GPP_S4, NONE),
	/* S5  : NC */
	PAD_NC(GPP_S5, NONE),
	/* S6  : NC */
	PAD_NC(GPP_S6, NONE),
	/* S7  : NC */
	PAD_NC(GPP_S7, NONE),

	/* GPD0  : BATLOW# ==> BATLOW# */
	PAD_CFG_NF(GPD0, NONE, DEEP, NF1),
	/* GPD1  : ACPRESENT ==> ACPRESENT */
	PAD_CFG_NF(GPD1, NONE, DEEP, NF1),
	/* GPD2  : GPD2_STRAP */
	PAD_NC(GPD2, NONE),
	/* GPD3  : PWRBTN# ==> PM_PWRBTN_N */
	PAD_CFG_NF(GPD3, NONE, DEEP, NF1),
	/* GPD4  : SLP_S3# ==> PM_SLP_S3_N */
	PAD_CFG_NF(GPD4, NONE, DEEP, NF1),
	/* GPD5  : SLP_S4# ==> PM_SLP_S4_N */
	PAD_CFG_NF(GPD5, NONE, DEEP, NF1),
	/* GPD6  : SLP_A# ==> PM_SLP_A_N */
	PAD_CFG_NF(GPD6, NONE, DEEP, NF1),
	/* GPD7  : NC */
	PAD_NC(GPD7, NONE),
	/* GPD8  : SUSCLK ==> SUS_CLK */
	PAD_CFG_NF(GPD8, NONE, DEEP, NF1),
	/* GPD9  : SLP_WLAN# ==> PM_SLP_WLAN_N */
	PAD_CFG_NF(GPD9, NONE, DEEP, NF1),
	/* GPD10 : SLP_S5# ==> PM_SLP_S5_N */
	PAD_CFG_NF(GPD10, NONE, DEEP, NF1),
	/* GPD11 : NC */
	PAD_NC(GPD11, NONE),
};

static void configure_tpl(void)
{
	ADVANCED_CPU_CONFIGURATION efi_cfg;
	enum cb_err ret;

	config_t *cfg = config_of_soc();
	struct soc_power_limits_config *soc_conf =
		&cfg->power_limits_config[ADL_N_041_6W_CORE];

	ret = mainboard_get_advanced_cpu_config(&efi_cfg);
	if (ret != CB_SUCCESS)
		return;

	printk(BIOS_DEBUG, "%s: Configuring PL1: %d W, PL2: %d W\n",
	       __func__, efi_cfg.PL1, efi_cfg.PL2);

	soc_conf->tdp_pl1_override = efi_cfg.PL1;
	soc_conf->tdp_pl2_override = efi_cfg.PL2;
}

static void mainboard_init(void *chip_info)
{
	int slp_typ = acpi_get_sleep_type();

	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));

	configure_tpl();

	/*
	 * If we're coming from S4 or S5, allow additional
	 * time for any attached USB-C displays to be detected
	 */
	if (slp_typ == ACPI_S4 || slp_typ == ACPI_S5)
		mdelay(2500);
}

struct chip_operations mainboard_ops = {
	.init = mainboard_init,
};
