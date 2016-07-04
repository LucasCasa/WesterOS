#include "sounds.h"
#include "lib.h"

void* beep(void* s){
	_call_int80(INT_BEEP);
	return 0;
}

void* songs(void* s){
	int flag = 1;

	shell_erase_screen();
	print_message("\n ___|)________________  _____ _____ _____ _____ _____  __________\n",0xFF);
	print_message("|___/_________________ |   __|     |   | |   __|   __| __________\n",0xFF);
	print_message("|__/|_________________ |__   |  |  | | | |  |  |__   | __________\n",0xFF);
	print_message("|_/(|,\\_______________ |_____|_____|_|___|_____|_____| __________\n",0xFF);
	print_message("|_\\_|_/__________________________________________________________\n",0xFF);
	print_message("|   |  \n",0xFF);
	print_message("| (_|  \n\n",0xFF);

	print_message("Choose the number of the masterpiece you wish to hear: \n",0xFF);
	print_message("  1. Because - The Beatles\n", 0xFF);
	print_message("  2. Piano Concerto No. 5 - Beethoven\n", 0xFF);
	print_message("  3. exit play songs :( \n\n", 0xFF);
	char aux[10];

	while(flag){
		readKeyboard(aux);
		uint64_t value = atoi(aux);
		if(value >= 0 && value <=4){
			flag = 0;
			_call_int80(INT_SONGS, value);
		}else if(value == 3){
			flag = 0;
		}else{
			print_message("Please enter a valid number\n", 0xFF);
		}
	}
	return 0;
}


void* piano(void* s){

	shell_erase_screen();

	print_message(" _______________________________________ \n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  |2| |3|  |  |5| |6| |7|  |  |9| | |  |\n",0xFF);
	print_message("|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |\n",0xFF);
	print_message("|   |   |   |   |   |   |   |   |   |   |\n",0xFF);
	print_message("| Q | W | E | R | T | Y | U | I |   |   |\n",0xFF);
	print_message("|___|___|___|___|___|___|___|___|___|___|\n\n ",0xFF);


	print_message(" _______________________________________ \n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  | | | |  |  | | | | | |  |  | | | |  |\n",0xFF);
	print_message("|  | | |S|  |  |F| |G| |H|  |  | | | |  |\n",0xFF);
	print_message("|  |_| |_|  |  |_| |_| |_|  |  |_| |_|  |\n",0xFF);
	print_message("|   |   |   |   |   |   |   |   |   |   |\n",0xFF);
	print_message("|   | Z | X | C | V | B | N | M |   |   |\n",0xFF);
	print_message("|___|___|___|___|___|___|___|___|___|___|\n",0xFF);

	print_message("\nPress ESC to leave the piano\n\n",0xFF);

	_call_int80(INT_PIANO);
	return 0;
}
