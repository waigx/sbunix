/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/timer.h>


#define MAX_IDT               256

#define IDT_SEGMENT         (0x08)

#define IDT_DPL0            (0x00)  /*** descriptor privilege level 0 ***/
#define IDT_DPL1            (0x20)  /*** descriptor privilege level 1 ***/
#define IDT_DPL2            (0x40)  /*** descriptor privilege level 2 ***/
#define IDT_DPL3            (0x60)  /*** descriptor privilege level 3 ***/
#define IDT_P               (0x80)  /*** present ***/
#define IDT_INTERRUPT_TYPE  (0x0e)


struct idt_t {
	uint16_t low_offset;
	uint16_t segment_sel;
	/* It include IST, if you want to use IST, you should change this member. */
	uint8_t  reserved1; 
	uint8_t  flags;
	uint16_t middle_offset;
	uint32_t high_offset;
	uint32_t reserved2;
}__attribute__((packed));

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));


static struct idt_t idt_entry[MAX_IDT];


extern struct idtr_t idt[];

static struct idtr_t idtr = {
	sizeof(idt_entry),
	(uint64_t)idt_entry
};

//extern struct idtr_t idt[];


static struct idt_t idt_entry[MAX_IDT];

void _x86_64_asm_lidt(struct idtr_t* idtr);
void _set_isr_table(void);
void _set_idt_entry(struct idt_t *entry, void *handler, uint64_t selector, uint8_t flags);


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();



void reload_idt()
{
	_set_isr_table();
	_x86_64_asm_lidt(&idtr);
}

void _set_isr_table(void)
{
	struct idt_t *entry;
	entry = idt_entry;
	
	_set_idt_entry(&entry[0], isr0, IDT_SEGMENT, IDT_DPL0|IDT_P |IDT_INTERRUPT_TYPE );
	_set_idt_entry(&entry[1], isr1, IDT_SEGMENT  ,IDT_DPL0);
	_set_idt_entry(&entry[2], isr2, IDT_SEGMENT  ,IDT_DPL0);
	_set_idt_entry(&entry[3], isr3, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[4], isr4, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[5], isr5, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[6], isr6, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[7], isr7, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[8], isr8, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[9], isr9, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[10], isr10, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[11], isr11, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[12], isr12, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[13], isr13, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[14], isr14, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[15], isr15, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[16], isr16, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[17], isr17, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[18], isr18, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[19], isr19, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[20], isr20, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[21], isr21, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[22], isr22, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[23], isr23, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[24], isr24, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[25], isr25, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[26], isr26, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[27], isr27, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[28], isr28, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[29], isr29, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[30], isr30, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[31], isr31, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[32], isr32, IDT_SEGMENT ,IDT_DPL0|IDT_P|IDT_INTERRUPT_TYPE);
	_set_idt_entry(&entry[33], isr33, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[34], isr34, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[35], isr35, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[36], isr36, IDT_SEGMENT ,IDT_DPL0);
	_set_idt_entry(&entry[37], isr37, IDT_SEGMENT ,IDT_DPL0);
	
}

void _set_idt_entry(struct idt_t *entry, void *handler, uint64_t selector, uint8_t flags)
{
	entry->low_offset = (uint64_t)handler & 0xFFFF;
	entry->segment_sel = selector;
	entry->flags = flags;
	entry->middle_offset = ((uint64_t)handler >> 16) & 0xFFFF;
	entry->high_offset = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
	entry->reserved1 = 0;
	entry->reserved2 = 0;

}
