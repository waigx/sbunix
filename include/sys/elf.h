#ifndef _ELF_H
#define _ELF_H

#include <sys/defs.h>
#include <stdlib.h>
#include <sys/sched.h>
//int open_tarfs(const char *pathname, int flags);
/*

// reference to downloads.openwatcom.org/ftp/.../elf-64-gen.pdf \
// and http://en.wikipedia.org/wiki/Executable_and_Linkable_Format

*/
typedef struct {
	unsigned char e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
}Elf64_Ehdr;

typedef struct {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_va;
	uint64_t p_pa;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
}Elf64_Phdr;




uint64_t load_elf(task_t *task, const char *task_name);



#endif
