// Copyright (C) 2025 - Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#include <stdint.h>
#include <tkey/assert.h>
#include <tkey/lib.h>
#include <tkey/tk1_mem.h>

#include "reset.h"

// clang-format off
static volatile uint32_t *system_reset  = (volatile uint32_t *)TK1_MMIO_TK1_SYSTEM_RESET;
static volatile struct reset *resetinfo = (volatile struct reset *)TK1_MMIO_RESETINFO_BASE;
// clang-format on

int reset(struct reset *userreset, size_t nextlen)
{
	if ((uint32_t)userreset < TK1_RAM_BASE ||
	    (uint32_t)userreset >= TK1_RAM_BASE + TK1_RAM_SIZE) {
		return -1;
	}

	if (nextlen > sizeof(resetinfo->next_app_data)) {
		return -1;
	}

	(void)memset((void *)resetinfo, 0, sizeof(*resetinfo));
	resetinfo->type = userreset->type;
	memcpy((void *)resetinfo->app_digest, userreset->app_digest, 32);
	memcpy((void *)resetinfo->next_app_data, userreset->next_app_data,
	       nextlen);

	// Do the actual reset.
	*system_reset = 1;

	// Should not be reached.
	assert(1 == 2);

	__builtin_unreachable();
}
