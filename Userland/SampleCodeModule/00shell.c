#include "stdint.h"
#include "00shell.h"
#include "call80.h"
#include "lib.h"
#include "sounds.h"


Command commands[15];
char comm[20];
int number_of_commands = 15;
char aux;
char name[20] = {0};


extern char bss;
extern char endOfBinary;
extern uint8_t data;
extern uint8_t text;
extern uint8_t rodata;
extern double _sin(int angle);
extern double _cos(int angle);

//void* reader(void* algo);
//void* writer(void* algo);

int main(){
	memset(&bss, 0, &endOfBinary - &bss);
	shell_erase_screen();
	beep(0);
	print_message("#   #                                ######  #### \n", 0xFF);
	print_message("# # # #####  ###  ##### ##### #####  #    # #    #\n", 0xFF);
	print_message("# # # #     #       #   #     #    # #    # #     \n", 0xFF);
	print_message("# # # ####   ###    #   ####  #    # #    #  #### \n", 0xFF);
	print_message("# # # #         #   #   #     #####  #    #      #\n", 0xFF);
	print_message("# # # #     #   #   #   #     #   #  #    # #    #\n", 0xFF);
	print_message(" # #  #####  ###    #   ##### #    # ######  #### \n", 0xFF);

	print_message("\nType help and hit enter to see available commands\n\n", 0xFF);

	_call_int80(INT_GET_ALL_PROCESS);

	init_commands(0,"clear", clear_str ,&shell_erase_screen);
	init_commands(1,"time", time_str , &showRTC);
	init_commands(2,"help" , help_str, &shell_show_commands);
	init_commands(3,"change time", change_time_str , &changeTime);
	init_commands(4, "whoami", whoami_str , &whoami);
	init_commands(5, "keyboard" , keyboard_str, &show_keyboard);
	init_commands(6, "colors" ,modi_str, &modify_colors);
	init_commands(7, "screen time", saver_str, &set_screensaver_time);
	init_commands(8,"piano",piano_str,&piano);
	init_commands(9,"songs",songs_str,&songs);
	init_commands(10,"beep", beep_str,&beep);
	init_commands(11,"draw",draw_str,&draw);
	init_commands(12,"game",game_str,&game);
	init_commands(13,"ipcs",ipcs_str,&list_ipcs);
	init_commands(14,"ps",show_process_str,&show_process);
	//_call_int80(INT_SLEEP,100);

	//_call_int80(INT_NEW_PROCESS,"read",&reader);
	//_call_int80(INT_NEW_PROCESS,"write",&writer);

	while(1){
		shell_command();
	}
}

void init_commands(unsigned char index, char * name, char* description, void (*function)()){
	commands[index].name = name;
	commands[index].description = description;
	commands[index].function = function;
}

void shell_command(){
	char valid_command = 0;
	unsigned char c = 0,j = 0;
	while((c = get_char()) != '\n'){
		comm[j++] = c;
	}

	comm[j] = 0;

	for(int i = 0;i<number_of_commands;i++){
		if(strcmp(comm,commands[i].name) == 0){
			_call_int80(INT_NEW_PROCESS,commands[i].name,commands[i].function);
			valid_command = 1;
		}
	}
	if(valid_command == 0){
		printf("Invalid command \"%s\" \n",0xFF, comm);
	}

}
void* show_process(void* s){
	_call_int80(INT_GET_ALL_PROCESS);
	return 0;
}
void* shell_show_commands(void* sss){
	shell_erase_screen();

	print_message(" _          _       \n", 0xFF);
	print_message("| |        | |      \n", 0xFF);
	print_message("| |__   ___| |_ __  \n", 0xFF);
	print_message("| '_ \\ / _ \\ | '_ \\ \n", 0xFF);
	print_message("| | | |  __/ | |_) |\n", 0xFF);
	print_message("|_| |_|\\___|_| .__/ \n", 0xFF);
	print_message("             | |    \n", 0xFF);
	print_message("             |_|    \n\n", 0xFF);

	for(int i=0 ;i<number_of_commands;i++){
		print_message(commands[i].name, 0xFF);
		print_message(" - ", 0xFF);
		print_message(commands[i].description, 0xFF);
		print_message("\n",0xFF);
	}
	print_message("\n", 0xFF);
}

void shell_erase_screen(){
	_call_int80(INT_ERASE_SCR);
}

void* showRTC(void* sss){
	unsigned char seg = _call_int80(INT_RTC_READ,0);
	unsigned char min = _call_int80(INT_RTC_READ,1);
	unsigned char hora = _call_int80(INT_RTC_READ,2);
	unsigned char dia = _call_int80(INT_RTC_READ,3);
	unsigned char mes = _call_int80(INT_RTC_READ,4);
	unsigned char anio = _call_int80(INT_RTC_READ,5);
	printf("Day: %d Month: %d Year: %d\n", 0xFF, dia,mes,anio);
	printf("Time: %d:%d:%d \n", 0xFF,hora,min,seg);
}

void* whoami(void* sss){
	if(name[0] == 0){
		unsigned char c = 0,i = 0;
		print_message("No se... Quien sos?\n",0xFF);
		while((c = get_char()) != '\n' && i<19){
			name[i++] = c;
		}
		if(i==19)
			flush_buffer();
	}else{
		if(isMuffin(name))
			muffin(0);
		else
			printf("%s \n",0xFF,name);
	}
}

void* draw(void* sss){
	_call_int80(INT_ENTER_DRAW_MODE);
	_call_int80(INT_CLEAR);
	Point aux;
	Point p = {300,300};
	Color c = {255,255,255};
	char ch;
	int angle = 0;
	int pass = 0;
	double accumx = 300, accumy = 300;
	while(1){
		p.x = accumx;
		p.y = accumy;
		_call_int80(INT_DRAW_CIRCLE,&p,20,&c);

		if(p.x<20){
			aux.x = p.x+1024;
			aux.y = p.y;
			_call_int80(INT_DRAW_CIRCLE,&aux,20,&c);
		}else if(p.x>1024-20,p.y){
			aux.x = p.x-1024;
			aux.y = p.y;
			_call_int80(INT_DRAW_CIRCLE,&aux,20,&c);
		}

		if(p.y<20){
			aux.x = p.x;
			aux.y = p.y+768;
			_call_int80(INT_DRAW_CIRCLE,&aux,20,&c);
		}else if(p.y>768-20){
			aux.x = p.x;
			aux.y = p.y-768;
			_call_int80(INT_DRAW_CIRCLE,&aux,20,&c);
		}

		while((ch = _call_int80(INT_GCFB)) == 0 && pass < 30000){
			pass++;
		}

		if(ch == 'a'){
			angle-=10;
		}else if(ch == 'd'){
			angle+=10;
		}
		if(ch == 'x'){
			_call_int80(INT_EXIT_DRAW_MODE);
			_call_int80(INT_ERASE_SCR);
			return;
		}
		accumx+= 2*_cos(angle);
		accumy+= 2*_sin(angle);
		if(accumy < 0){
			accumy = 768 + accumy;
		}else if(accumy >= 768){
			accumy = accumy - 768;
		}
		if(accumx < 0){
			accumx = 1024+accumx;
		}else if(accumx >= 1024){
			accumx = accumx - 1024;
		}
		ch = 0;
		pass = 0;
	}
}

void list_ipcs(){
	_call_int80(INT_SHOWIPCS);
}

void* muffin(void* sss){
	print_message("   .-\"`\"`\"`\"-.   \n", 0xFF);
    print_message("  /.\'\'\'`.\'`.\'`\\  \n", 0xFF);
    print_message(" /`.\'`.`\'.`\'`.\'\\ \n", 0xFF);
    print_message("/.`.\'`.`\'.`\'`.\'\\ \n", 0xFF);
    print_message("(.`\'.`\'.\'`.\'`.`\')\n", 0xFF);
    print_message(" ~|||||||||||||~ \n", 0xFF);
    print_message("  |||||||||||||  \n", 0xFF);
    print_message("  |||||||||||||  \n", 0xFF);
    print_message("  |||||||||||||  \n", 0xFF);
    print_message("  `\"\"\"\"\"\"\"\"\"\"\"`  \n", 0xFF);
}

int isMuffin(char * name){
	int i;
	char * m = "muffin";
	for(i=0; name[i]!=0 && m[i]!=0; i++){
		if(name[i]!=m[i])
			return 0;
	}
	return m[i]==name[i];
}


//PARA TESTEAR READ BLOQUEANTE
/*void* reader(void* algo){
	print_message("En el process que lee\n",0xFF);
	char * buff = _call_int80(INT_MALLOC,1);
	for(int i=0; i<128; i++){
		buff[i] = 0;
	}
	int fd = _call_int80(INT_MKFIFO,"test");
	_call_int80(INT_READFIFOBLOQ,fd,buff,128);
	print_message("lei: ",0xFF);
	print_message(buff,0xFF);
	print_message("\n",0xFF);
}

void* writer(void* algo){
	print_message("En el process que escribe\n",0xFF);
	int fd;
	char * buf = "HOLA QUE TAL AMIGO";
	while((fd=_call_int80(INT_OPENFIFO,"test"))==0);
	_call_int80(INT_SLEEP,10000);
	_call_int80(INT_WRITEFIFO,fd,buf,19);
}*/

/*void testipc(){
	char buf[6];
	int n;
	int fd = _call_int80(MKFIFO,"muffin");
	printf("mk ret: %d\n",0xFF,fd);
	n = _call_int80(OPENFIFO,"muffin");
	printf("open ret: %d\n",0xFF,fd);
	n = _call_int80(WRITEFIFO,fd,"fucku\n",6);
	printf("write ret: %d\n",0xFF,n);
	n = _call_int80(READFIFO,fd,buf,6);
	printf("read ret: %d\n",0xFF,n);
	printf("leo: %s\n",0xFF,buf);
	fd = _call_int80(MKFIFO,"magda");
	n = _call_int80(WRITEFIFO,fd,"panqueque",9);
	printf("segundo mkfifo ret: %d\n",0xFF,fd);
}*/
