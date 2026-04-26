/* SPDX-License-Identifier: GPL-2.0-or-later */

/***************************************************************************
 *   Copyright (C) 2015 by David Ung                                       *
 ***************************************************************************/

#ifndef OPENOCD_TARGET_AARCH64_H
#define OPENOCD_TARGET_AARCH64_H

#include "armv8.h"

#define AARCH64_COMMON_MAGIC 0x41413634U

#define CPUDBG_CPUID	0xD00
#define CPUDBG_CTYPR	0xD04
#define CPUDBG_TTYPR	0xD0C
#define ID_AA64PFR0_EL1	0xD20
#define ID_AA64DFR0_EL1	0xD28

#define BRP_NORMAL 0
#define BRP_CONTEXT 1

#define AARCH64_PADDRDBG_CPU_SHIFT 13

enum aarch64_isrmasking_mode {
	AARCH64_ISRMASK_OFF,
	AARCH64_ISRMASK_ON,
};

struct aarch64_brp {
	int used;
	int type;
	target_addr_t value;
	uint32_t control;
	uint8_t brpn;
};

struct aarch64_common {
	unsigned int common_magic;

	struct armv8_common armv8_common;

	/* Context information */
	uint64_t system_control_reg;
	uint64_t system_control_reg_curr;

	/* Breakpoint register pairs */
	int brp_num_context;
	int brp_num;
	int brp_num_available;
	struct aarch64_brp *brp_list;

	/* Watchpoint register pairs */
	int wp_num;
	int wp_num_available;
	struct aarch64_brp *wp_list;

	enum aarch64_isrmasking_mode isrmasking_mode;

	/*
	 * ID_AA64DFR0_EL1.DoubleLock (bits [39:36]) probed at examine
	 * time. ARM ARM (ARMv8-A) D17.2.45: 0b0000 = OS Double-Lock
	 * implemented (OSDLR_EL1.DLK is RW), 0b1111 = not implemented
	 * (writes to OSDLR_EL1 are CONSTRAINED UNPREDICTABLE on those
	 * cores). Used to gate the resume-time DLK clear so we don't
	 * spam warnings on cores without the feature.
	 */
	bool osdlr_implemented;
};

static inline struct aarch64_common *
target_to_aarch64(struct target *target)
{
	return container_of(target->arch_info, struct aarch64_common, armv8_common.arm);
}

#endif /* OPENOCD_TARGET_AARCH64_H */
