#include <stdint.h>
//#include <string.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "scheduler.h"
#include "process.h"
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void _change_to_graphics();
extern void _inactive_process();
void* start_shell(void*);
extern void set_interrupts();
extern void _start_userland();
void initialize_task();
void* inactive_p(void*);
void* soy2(void* s);
void* soy3(void *s);
static const uint64_t PageSize = 0x1000;
void* kernel_stack;
static void * const sampleCodeModuleAddress = (void*)0x600000;
static void * const sampleDataModuleAddress = (void*)0x700000;
static void * const TarImageModuleAddress = (void*)0x710000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	kernel_stack = (void*)( (uint64_t)&endOfKernel + PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
	return kernel_stack;
}


void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint((char *)cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		TarImageModuleAddress,
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();
	//while(1);
	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{

	init_scheduler();
	init_malloc();
	ncPrint("Scheduler");
	put_char('a',0xFF);
	put_char('b',0xFF);
	Process* test = create_process("Snorlax",initialize_task,0);
	Process* s1 = create_process("Inactive P",inactive_p,0);
	Process* shell = create_process("Shell",start_shell,0);
	add_inactive(s1);
	add_new_process(shell);
	//shell->has_foreground = 1;
	add_new_process(test);
	//test->has_foreground = 0;
	test->state = PROC_WAITING;

	//add_new_process(s3);
	kmain();
	_start_userland();
	return 0;
}
void* start_shell(void* a){
	((EntryPoint)sampleCodeModuleAddress)();
	while(1);
	return 0;
}
void* inactive_p(void* s){
	while(1){
	 __asm__( "hlt" );
 	}
}
void* soy2(void* s){
	int i = 0;
	while(1){
		i++;
		if(i> 10000000){
			print_message("  Soy 2!!!  \n",0xFF);
			i = 0;

		}
	}
}
void* soy3(void* ss){
	int i = 0;
	while(1){
		i++;
		if(i> 10000000){
			print_message("  Soy 3!!!  \n",0xFF);
			i = 0;

		}
	}
}
void initialize_task() {
	int i = 0;
	while(1){
		i++;
		if(i> 100000000){
			print_message("Corriendo...",0xFF);
			i = 0;
		}
	}
}
