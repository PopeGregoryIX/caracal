/*
 * tss.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_ARCH_CPU_X86_64_TABLES_TSS_H_
#define KERNEL_INCLUDE_ARCH_CPU_X86_64_TABLES_TSS_H_

#include <stdint.h>

namespace arch
{
	struct tssTable
	{
		uint32_t reserved0;
		uint32_t rsp0_low;
		uint32_t rsp0_high;
		uint32_t rsp1_low;
		uint32_t rsp1_high;
		uint32_t rsp2_low;
		uint32_t rsp2_high;
		uint32_t reserved1;
		uint32_t reserved2;
		uint32_t ist1_low;
		uint32_t ist1_high;
		uint32_t ist2_low;
		uint32_t ist2_high;
		uint32_t ist3_low;
		uint32_t ist3_high;
		uint32_t ist4_low;
		uint32_t ist4_high;
		uint32_t ist5_low;
		uint32_t ist5_high;
		uint32_t ist6_low;
		uint32_t ist6_high;
		uint32_t ist7_low;
		uint32_t ist7_high;
		uint32_t reserved3;
		uint32_t reserved4;
		uint16_t reserved5;
		uint16_t iopb;
	} __attribute__((packed));
}



#endif /* KERNEL_INCLUDE_ARCH_CPU_X86_64_TABLES_TSS_H_ */
