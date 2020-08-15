#include "proc.h"
#include "fs.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
	int fd = fs_open(filename, 0, 0);
	Elf_Ehdr ehdr;
	fs_read(fd, (void *)&ehdr, sizeof(Elf_Ehdr));
	
	// check whether the file format is ELF
	if (!( ehdr.e_ident[EI_MAG0] == ELFMAG0 &&
		ehdr.e_ident[EI_MAG1] == ELFMAG1 &&
		ehdr.e_ident[EI_MAG2] == ELFMAG2 &&
		ehdr.e_ident[EI_MAG3] == ELFMAG3) ) {
		
		assert(0);
	}
  
	Elf_Phdr phdr; 
	int num = ehdr.e_shnum;
	for(int i = 0; i < num; ++ i){
		fs_lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		fs_read(fd, &phdr, sizeof(Elf_Phdr));
		if(phdr.p_type != PT_LOAD) continue; 
		fs_lseek(fd, phdr.p_offset, SEEK_SET);
		fs_read(fd, (void *)phdr.p_vaddr, phdr.p_filesz);
		memset((void *)phdr.p_vaddr + phdr.p_filesz, 0, phdr.p_memsz - phdr.p_filesz);
	}
	
	fs_close(fd);
	
	return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
