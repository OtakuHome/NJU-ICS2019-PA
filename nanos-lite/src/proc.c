#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);
void register_pcb(PCB *pcb);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {

  Log("Initializing processes...");
  //register_pcb(&pcb[1]);
  //context_kload(&pcb[0], (void *)hello_fun);
  context_uload(&pcb[0], "/bin/pal");
  context_uload(&pcb[1], "/bin/hello");
  
  //context_uload(&pcb[0], "/bin/dummy");
  //naive_uload(NULL, "/bin/dummy");
  switch_boot_pcb();
  
  // load program here
  // naive_uload(NULL, "/bin/init");
  
}

_Context* schedule(_Context *prev) {
  current->cp = prev;
  static int time_piece = 0;	//时间片
  //current = &pcb[0];
  if(time_piece <= 0) {
  	current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  	if(current == &pcb[0]) time_piece = 100;
  	else time_piece = 1;
  }
  -- time_piece;
  //Log("schedule success. current PCB: 0x%08x", current);
  return current->cp;
}
