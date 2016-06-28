#include <stdint.h>
//#include <string.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "scheduler.h"
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void _change_to_graphics();
void* start_shell(void*);
extern void set_interrupts();
extern void _start_userland();
void initialize_task();
static const uint64_t PageSize = 0x1000;
void* kernel_stack;
static void * const sampleCodeModuleAddress = (void*)0x600000;
static void * const sampleDataModuleAddress = (void*)0x700000;
static void * const ImageDataModuleAddress = (void*)0x800000;
static void * const TarImageModuleAddress = (void*)0x850000;

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
		ImageDataModuleAddress,
		TarImageModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

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
	Process* IDT_init = create_process(initialize_task);
	put_char('c',0xFF);
	add_new_process(initialize_task);

	Process* shell = create_process(start_shell);
	add_new_process(shell);
	kmain();
	_start_userland();
	ncPrint("Start");
	return 0;
}
void* start_shell(void* a){
	((EntryPoint)sampleCodeModuleAddress)();
	return 0;
}
void initialize_task() {
	set_interrupts();
    while (1);
}
