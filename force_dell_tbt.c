/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2016 Mario Limonciello <mario.limonciello@dell.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <smbios_c/smi.h>
#include <smbios_c/obj/smi.h>
#include <efivar.h>
#include <stdio.h>

#define DACI_FLASH_INTERFACE_CLASS	7
#define DACI_FLASH_INTERFACE_SELECT	3
#define DACI_FLASH_ARG_FLASH_MODE	3

#define DEVICE_FLASHING_MODE	1
#define DEVICE_USER_MODE	0

#define SMI_SUCCESS	0
#define TBT_GPIO_GUID		EFI_GUID (0x2efd333f, 0x65ec, 0x41d3, 0x86d3, 0x08, 0xf0, 0x9f, 0x4f, 0xb1, 0x14)

typedef union _ADDR_UNION{
	uint8_t *buf;
	efi_guid_t *guid;
} ADDR_UNION;
#pragma pack()

int force_device_mode(const efi_guid_t guid, int mode)
{
	int ret;
	struct dell_smi_obj *smi;
	ADDR_UNION buf;

	smi = dell_smi_factory (DELL_SMI_DEFAULTS);
	dell_smi_obj_set_class (smi, DACI_FLASH_INTERFACE_CLASS);
	dell_smi_obj_set_select (smi, DACI_FLASH_INTERFACE_SELECT);
	dell_smi_obj_set_arg (smi, cbARG1, DACI_FLASH_ARG_FLASH_MODE);
	//needs to be padded with an empty GUID
	buf.buf = dell_smi_obj_make_buffer_frombios_withoutheader(smi, cbARG2,
								  sizeof(efi_guid_t) *2);
        if (!buf.buf) {
		printf("failed to build buffer\n");
		return -1;
	}
	*buf.guid = guid;

	dell_smi_obj_set_arg (smi, cbARG4, mode);

	ret = dell_smi_obj_execute(smi);
	if (ret != SMI_SUCCESS){
		return ret;
	}

	ret = dell_smi_obj_get_res(smi, cbRES1);
	if (ret != SMI_SUCCESS)
		return ret;
	return 0;

}

void usage()
{
	printf("This application forces TBT controller to flashing mode on Dell systems\n\n");
	printf("Call with an argument of '1' to force flashing mode\n");
	printf("Call with an argument of '0' to return to normal mode\n");
}

int main(int argc, char *argv[])
{
	int ret;
	int mode;
	if (argc == 2) {
		if (strcmp(argv[1], "1") == 0)
			mode = DEVICE_FLASHING_MODE;
		else if (strcmp(argv[1], "0") == 0)
			mode = DEVICE_USER_MODE;
		else {
			usage();
			return -1;
		}
		ret = force_device_mode(TBT_GPIO_GUID, mode);
		if (ret != 0) {
			printf("Failed to force device to %d (ret %d)\n", mode, ret);
			return -1;
		}
		printf("Turned device to %d\n", mode);
	}
	else
		usage();
	return 0;
}
