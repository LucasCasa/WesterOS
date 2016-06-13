#ifndef SHELL_H_

#define SHELL_H_

typedef struct {
  char* name;
  char* description;
  void(* function)();

} Command;



void * memset(void * destiny, int32_t c, uint64_t length);
void init_commands(unsigned char index, char * name, char* description, void (*function)());
void showRTC();
void shell_command();
void shell_erase_screen();
void _shutdown_asm();
void changeTime();
void shell_show_commands();
void shell_exit();
void whoami();
void show_keyboard();
void modify_colors();
void set_screensaver_time();
void beep();
void songs();
void piano();
void draw();
void game();

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

#endif
