/*
 *  Easy-Link EL-MINI board support
 *
 *  Copyright (C) 2012 huangfc <huangfangcheng@163.com>
 *  Copyright (C) 2011 hys <550663898@qq.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>

#include <asm/mach-ath79/ath79.h>

#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define SXX_AR9331_GPIO_LED_SYSTEM	27
#define SXX_AR9331_GPIO_BTN_RESET	11

#define SXX_AR9331_GPIO_USB_POWER	8

#define SXX_AR9331_KEYS_POLL_INTERVAL	20	/* msecs */
#define SXX_AR9331_KEYS_DEBOUNCE_INTERVAL	(3 * SXX_AR9331_KEYS_POLL_INTERVAL)

static const char *sxx_ar9331_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data sxx_ar9331_flash_data = {
	.part_probes	= sxx_ar9331_part_probes,
};

static struct gpio_led sxx_ar9331_leds_gpio[] __initdata = {
	{
		.name		= "sxx-ar9331:green:system",
		.gpio		= SXX_AR9331_GPIO_LED_SYSTEM,
		.active_low	= 1,
	},
};

static struct gpio_keys_button sxx_ar9331_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = SXX_AR9331_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= SXX_AR9331_GPIO_BTN_RESET,
		.active_low	= 0,
	}
};

static void __init sxx_ar9331_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
	u8 *ee = (u8 *) KSEG1ADDR(0x1fff1000);

	ath79_register_m25p80(&sxx_ar9331_flash_data);
	ath79_register_leds_gpio(-1, ARRAY_SIZE(sxx_ar9331_leds_gpio),
				 sxx_ar9331_leds_gpio);
	ath79_register_gpio_keys_polled(-1, SXX_AR9331_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(sxx_ar9331_gpio_keys),
					sxx_ar9331_gpio_keys);

	gpio_request_one(SXX_AR9331_GPIO_USB_POWER,
			 GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			 "USB power");
	ath79_register_usb();

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, -1);

	ath79_register_mdio(0, 0x0);
	ath79_register_eth(0);

	ath79_register_wmac(ee, mac);
}

MIPS_MACHINE(ATH79_MACH_SXX_AR9331, "SXX-AR9331", "SXX AR9331",
	     sxx_ar9331_setup);
