#ifndef SHELL_H_

#define SHELL_H_

typedef struct {
  char* name;
  char* description;
  void*(* function)(void*);

} Command;



void * memset(void * destiny, int32_t c, uint64_t length);
void init_commands(unsigned char index, char * name, char* description, void (*function)());
int isMuffin(char * name);
void* showRTC(void*);
void shell_command();
void shell_erase_screen();
void _shutdown_asm();
void changeTime();
void* shell_show_commands(void*);
void* shell_exit(void*);
void* whoami(void*);
void* show_keyboard(void*);
void* modify_colors(void*);
void* set_screensaver_time(void*);
void* beep(void*);
void* songs(void*);
void* piano(void*);
void* draw(void*);
void* game(void*);
void* muffin(void*);
void* show_process(void*);

char* time_str = "Shows the time and the date";
char* help_str = "Shows all available commands";
char* change_time_str = "Change time and date";
char* whoami_str = "Who am i?";
char* keyboard_str = "Shows the keyboard distribution graphically";
char* modi_str = "Change the console set of colours";
char* clear_str = "Clear the screen";
char* saver_str = "Change the time until the screensaver appears";
char* piano_str= "Start typing and listen to the sounds of the piano";
char* songs_str= "Choose a song and listen to it";
char* beep_str= "Listen to the beep";
char* draw_str ="Paint un poroto al lado de esto";
char* ipcs_str = "List all existing IPC's";
char* game_str = "Best game in da world";
char* show_process_str = "show all processes";

#endif
