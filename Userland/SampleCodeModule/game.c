#include "game.h"

char board[WIDTH*HEIGHT];
void* game_sound(void*);
int nplayers;
Color c[] = {{255,0,0},{0,0,255},{0,255,0},{255,0,255},{255,255,0},{255,255,255}};
char* controls[6][2] = {{"A","D"},{"J","L"},{"L Arrow","R Arrow"},{"Z","C"},{"I","P"},{"1","3"}};
int starting = 1;

void (*powerup_effects[NUM_POWERUPS][2])(Player*) = {{powerUp_cleanScreen,nothing},{powerUp_speed,powerUp_speed_end},{powerUp_speed_others,powerUp_speed_others_end},{powerUp_make_fat,powerUp_make_fat_end}};
Color powerup_color[NUM_POWERUPS] = {{0,191,255},{42,223,60},{240,101,101},{179,4,176}};
PowerUp powerups[MAX_POWERUPS];
int num_powerup, powerup_cont, powerup_next;

Player p[MAX_PLAYERS];
int fd;

void* game(void*saaasdsdsd){
  _call_int80(INT_ENTER_DRAW_MODE);
  _call_int80(INT_CLEAR);
  fd = _call_int80(INT_MKFIFO,"sound");
  _call_int80(INT_NEW_PROCESS,"Sound",game_sound);
  //AMOUNT OF PLAYERS
  nplayers = lobby();

  _call_int80(INT_CLEAR);

  starting = 1;

  // INIT powerups
  num_powerup = 0;
  for(int i=0; i<NUM_POWERUPS; i++){
    powerups[i].active = 0;
  }

  // INIT players
  init_players(nplayers);

  // INIT board
  for(int i = 0; i<WIDTH*HEIGHT;i++){
    board[i] = 0;
  }

  int pass = 0;

  _call_int80(INT_SET_EVENT_KEYUP,&get_key_up);
  _call_int80(INT_SET_EVENT_KEYDOWN,&get_key_down);


  while(1){
    _call_int80(INT_SLEEP,8);
    //print_message("Hola\n",0xFF);
    // Decides when to create new powerups
    if(!starting)
      managePowerups();

    // Check powerup collisions
    for(int i=0; i<MAX_POWERUPS; i++){
      if(powerups[i].active){
        for(int j=0; j<nplayers; j++){
          // check collision with player
          if(square((powerups[i].radius+p[j].radius))>square(p[j].pos.x - powerups[i].pos.x)+square(p[j].pos.y - powerups[i].pos.y)){
            collide_powerup(&(powerups[i]),&(p[j]));
          }
        }
      }
    }
    int iter = 0;
    for(int i = 0; i<nplayers;i++){
      if(p[i].alive){
        checkEffects(&(p[i]));
        if(!starting){
           p[i].time_no_inv += p[i].speed/INIT_SPEED;
          if(p[i].time_no_inv >= p[i].time_with_inv + HOLE_SIZE*(p[i].radius / RADIUS)){
            p[i].time_no_inv = 0;
            p[i].time_with_inv = (rand() % MAX_DRAW);
          }
        }
        p[i].angle += p[i].mod;
        p[i].acum.x += _cos(p[i].angle)*p[i].speed;
        p[i].acum.y += _sin(p[i].angle)*p[i].speed;

        // Save state
        p[i].pp_pos.x = p[i].prev_pos.x;
        p[i].pp_pos.y = p[i].prev_pos.y;
        p[i].pp_radius = p[i].prev_radius;

        p[i].prev_pos.x = p[i].pos.x;
        p[i].prev_pos.y = p[i].pos.y;
        p[i].prev_radius = p[i].radius;

       /* print_number(p[i].pos.x);
        print_message(",",0xFF);
        print_number(p[i].pos.y);
        print_message("\n",0xFF);*/


        p[i].pos.x = p[i].acum.x;
        p[i].pos.y = p[i].acum.y;
        if(0){
          print_number(i+1);
          if(p[i].pos.x == p[i].prev_pos.x){

            print_message(" Igual X ",0xFF);
          }else{
            print_message(" DiffX: ",0xFF);
            print_number(p[i].pos.x - p[i].prev_pos.x);
          }
          if(p[i].pos.y == p[i].prev_pos.y){
            print_message(" Igual Y ",0xFF);
          }else{
            print_message(" DiffY: ",0xFF);
            print_number(p[i].pos.y - p[i].prev_pos.y);
          }
          if(p[i].prev_radius == p[i].radius){
            print_message(" Igual Radio ",0xFF);
          }
          print_message("\n",0xFF);
        }

        if(p[i].time_no_inv < p[i].time_with_inv){
          _call_int80(INT_DRAW_CIRCLE,&(p[i].pos),p[i].radius,&c[i]);
          p[i].alive = draw_into_board(i+1,&(p[i]));
          if(!p[i].alive){
            _call_int80(INT_WRITEFIFO,fd,"A",1);
          }
          if(p[i].erasable >= 0){
            _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,p[i].erasable);
            p[i].erasable = -1;
          }
        }else{
          if(p[i].erasable >= 0){
            _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,p[i].erasable);
          }
          p[i].erasable = _call_int80(INT_DRAW_ERASABLE_CIRCLE,&(p[i].pos),p[i].radius,&c[i]);
        }
      }
    }
    int total_alive = 0;

    for(int i = 0; i<nplayers;i++){
      if(p[i].alive)
        total_alive++;
    }

    if((nplayers>1 && total_alive <= 1) || (nplayers==1 && total_alive==0)){
      for(int i = 0; i<nplayers;i++){
        if(p[i].alive){
          exit_game();
          print_message("Player ",0xFF);
          print_number(i+1);
          print_message(" has won!\n",0xFF);
          return 0;
        }
      }
      exit_game();
      print_message("Tie\n",0xFF);
      return 0;
    }
    pass = 0;
    if(starting == 40){
      starting = 0;
    }else if( starting > 0){
      starting++;
    }
  }
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  //MALLOC DE UN TABLERO??
  return 0;
}

int lobby(){
  char* div = "|";
  char* bar = "_________________________________";
  char play[2] = "1";
  Point point = {50,50};
    _call_int80(INT_DRAW_TEXT,&point,"Ingresar cantidad de jugadores(2 a 6):");
    point.y+=50;
    point.x = COLOR_POS;
    _call_int80(INT_DRAW_TEXT,&point,"COLOR");
    point.x = NUMBER_POS - 20;
    _call_int80(INT_DRAW_TEXT,&point,div);
    point.x = NUMBER_POS;
    _call_int80(INT_DRAW_TEXT,&point,"NUMERO");
    point.x = RIGHT_POS- 20;
    _call_int80(INT_DRAW_TEXT,&point,div);
    point.x = RIGHT_POS;
    _call_int80(INT_DRAW_TEXT,&point,"IZQUIERDA");
    point.x = LEFT_POS- 20;
    _call_int80(INT_DRAW_TEXT,&point,div);
    point.x = LEFT_POS;
    _call_int80(INT_DRAW_TEXT,&point,"DERECHA");
    point.y+=25;
    point.x = COLOR_POS;
    _call_int80(INT_DRAW_TEXT,&point,bar);
    point.y+=30;
    for(int i = 0; i<6;i++){
      point.y+= 10;
      point.x = COLOR_POS + ((NUMBER_POS - COLOR_POS) / 2 - RADIUS*2);
      _call_int80(INT_DRAW_CIRCLE,&point,RADIUS*2,&c[i]);
      point.y-=10;
      point.x = NUMBER_POS - 20;
      _call_int80(INT_DRAW_TEXT,&point,div);
      point.x = NUMBER_POS + ((RIGHT_POS - NUMBER_POS) / 2 - 11);
      play[0] = i + '1';
      _call_int80(INT_DRAW_TEXT,&point,play);
      point.x = RIGHT_POS- 20;
      _call_int80(INT_DRAW_TEXT,&point,div);
      point.x = RIGHT_POS;
      _call_int80(INT_DRAW_TEXT,&point,controls[i][0]);
      point.x = LEFT_POS - 20;
      _call_int80(INT_DRAW_TEXT,&point,div);
      point.x= LEFT_POS;
      _call_int80(INT_DRAW_TEXT,&point,controls[i][1]);
      point.y+=30;
    }

    char cha = 0;
    do{
      cha = _call_int80(INT_GCFB);
    }while(cha < '1' || cha > '6');
      return cha-'0';
}
void exit_game(){
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  _call_int80(INT_EXIT_DRAW_MODE);
  _call_int80(INT_ERASE_SCR);
  _call_int80(INT_CLOSEFIFO,fd);
}
void get_key_down(uint8_t key){
  switch(key){
    case 'a':
      p[0].mod =-p[0].speed;
      break;
    case 'd':
      p[0].mod = p[0].speed;
      break;
    case 'j':
      p[1].mod = -p[1].speed;
      break;
    case 'l':
      p[1].mod = p[1].speed;
      break;
    case '4':
      p[2].mod = -p[2].speed;
      break;
    case '6':
      p[2].mod = p[2].speed;
      break;
    case 'z':
      p[3].mod = -p[3].speed;
      break;
    case 'c':
      p[3].mod = p[3].speed;
      break;
    case 'i':
      p[4].mod = -p[4].speed;
      break;
    case 'p':
      p[4].mod = p[4].speed;
      break;
    case '1':
      p[5].mod = -p[5].speed;
      break;
    case '3':
      p[5].mod = p[5].speed;
      break;
  }
}
void get_key_up(uint8_t key){
  switch(key){
    case 'a':
      p[0].mod = 0;
      break;
    case 'd':
      p[0].mod = 0;
      break;
    case 'j':
      p[1].mod = 0;
      break;
    case 'l':
      p[1].mod = 0;
      break;
    case '4':
      p[2].mod = 0;
      break;
    case '6':
      p[2].mod = 0;
      break;
    case 'z':
      p[3].mod = 0;
      break;
    case 'c':
      p[3].mod = 0;
      break;
    case 'i':
      p[4].mod = 0;
      break;
    case 'p':
      p[4].mod = 0;
      break;
    case '1':
      p[5].mod = 0;
      break;
    case '3':
      p[5].mod = 0;
      break;
  }
}

char draw_into_board(uint32_t pn,Player * player){
  if(player->pos.x-player->radius < 0 || player->pos.x+player->radius > WIDTH){
    return 0;
  }
  if(player->pos.y-player->radius < 0 || player->pos.y+player->radius > HEIGHT){
    return 0;
  }
  for(signed int y=-player->radius ; y<=player->radius; y++){
    for(signed int x=-player->radius ; x<=player->radius; x++){
      //print_number(square(player->prev_pos.x - (player->pos.x + x))*square(player->prev_pos.y - (player->pos.y+y)));
      //print_message("\n",0xFF);
      if(x*x+y*y < player->radius*player->radius && checkPrev(x,y,player) && checkPP(x,y,player)){
        if(board[player->pos.x + x + (player->pos.y +y)*WIDTH] == 0){
          board[player->pos.x + x + (player->pos.y +y)*WIDTH ] = player->id;
        }else{
          /*print_message("Perdi",0xFF);
          print_message("Dist de viejo a punto: ",0xFF);
          print_number(square(player->prev_pos.x - (player->pos.x + x))+square(player->prev_pos.y-(player->pos.y+y)));
          print_message("Dist de nuevo a punto: ",0xFF);
          print_number(x*x+y*y);
          print_message("Juador",0xFF);
          print_number(player->id);
          print_message("\n",0xFF);*/

          return 0;
        }
      }
    }
  }
  return 1;
}
 int checkPrev(int x, int y,Player* player){
    return square(player->prev_pos.x - (player->pos.x + x))+square(player->prev_pos.y-(player->pos.y+y))>square(player->prev_radius);
 }
 int checkPP(int x, int y, Player* player){
    return square(player->pp_pos.x - (player->pos.x + x))+square(player->pp_pos.y-(player->pos.y+y))>square(player->pp_radius);
 }
void init_players(int nplayers){
  for(int i = 0; i<nplayers;i++){
    p[i].id = i+1;
    p[i].pos.x = rand() % (WIDTH-400) + 200;
    p[i].pos.y = rand() % (HEIGHT-360) + 180;
    p[i].acum.x = p[i].pos.x;
    p[i].acum.y = p[i].pos.y;
    p[i].angle = rand() % 360;
    p[i].mod = 0;
    p[i].alive = 1;
    p[i].radius = RADIUS;
    p[i].time_with_inv = 0;
    p[i].time_no_inv = 0;
    p[i].erasable = -1;
    p[i].speed = INIT_SPEED;
    p[i].prev_radius = 0;
    p[i].prev_pos.x = 0;
    p[i].prev_pos.y = 0;
    for(int j = 0; j<MAX_ACTIVE_EFFECTS; j++){
      p[i].effects[j].active = 0;
      p[i].effects[j].time_left = 0;
    }
  }
}

void powerUp_cleanScreen(Player * trigger){
  for(int i = 0; i<WIDTH*HEIGHT;i++){
    board[i] = 0;
  }
  _call_int80(INT_ERASE_SCR);
  for(int i = 0; i<MAX_POWERUPS; i++){
    if(powerups[i].active){
      _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,powerups[i].id);
      powerups[i].id = _call_int80(INT_DRAW_ERASABLE_CIRCLE,&(powerups[i].pos),powerups[i].radius,&(powerups[i].color));
    }
  }
}

void powerUp_speed(Player * trigger){
  trigger->speed += INIT_SPEED;
  if(trigger->mod > 0){
    trigger->mod = trigger->speed;
  }else if(trigger->mod<0){
    trigger->mod = -trigger->speed;
  }
}

void powerUp_speed_end(Player * player){
  player->speed -= INIT_SPEED;
  if(player->mod > 0){
    player->mod = player->speed;
  }else if(player->mod<0){
    player->mod = -player->speed;
  }
}

void powerUp_speed_others(Player * player){
  for(int i=0; i<nplayers; i++){
    if(p[i].id != player->id){
      powerUp_speed(&(p[i]));
    }
  }
}

void powerUp_speed_others_end(Player * player){
  for(int i=0; i<nplayers; i++){
    if(p[i].id != player->id){
      powerUp_speed_end(&(p[i]));
    }
  }
}

void powerUp_make_fat(Player * player){
  for(int i=0; i<nplayers; i++){
    if(p[i].id != player->id){
      p[i].radius += RADIUS;
    }
  }
}

void powerUp_make_fat_end(Player * player){
  for(int i=0; i<nplayers; i++){
    if(p[i].id != player->id){
      p[i].radius -= RADIUS;
    }
  }
}

int getRandIndex(int max){
  return rand() % max;
}

int getAvailablePowerupIndex(){
  for(int i=0; i<MAX_POWERUPS; i++){
    if(!powerups[i].active)
      return i;
  }
  return -1;
}

void createNewPowerup(){
  int n, k;
  if((n=getAvailablePowerupIndex())<0)
    return;
  powerups[n].active = 1;
  powerups[n].radius = POWERUP_RADIUS;
  powerups[n].pos.x = rand() % (WIDTH - 2*POWERUP_RADIUS) + POWERUP_RADIUS;
  powerups[n].pos.y = rand() % (HEIGHT - 2*POWERUP_RADIUS) + POWERUP_RADIUS;
  k = getRandIndex(NUM_POWERUPS-1); // SACAR -1
  powerups[n].initial_effect = powerup_effects[k][0];
  powerups[n].final_effect = powerup_effects[k][1];
  powerups[n].color = powerup_color[k];

  powerups[n].id = _call_int80(INT_DRAW_ERASABLE_CIRCLE,&(powerups[n].pos),powerups[n].radius,&(powerup_color[k]));
}

void managePowerups(){
  if(powerup_cont == powerup_next){
    createNewPowerup();
    powerup_cont = 0;
    powerup_next = rand()%(MAX_POWERUP_TIME - MIN_POWERUP_TIME) + MIN_POWERUP_TIME;
  }

  powerup_cont++;
}

double square(int n){
  return  n*n;
}

void nothing(Player * p){}

int getAvailableEffectIndex(Player * player){
  for(int i=0; i<MAX_ACTIVE_EFFECTS; i++){
    if(!player->effects[i].active)
      return i;
  }
  return -1;
}

void collide_powerup(PowerUp * pwup, Player * player){
  int n;
  if((n=getAvailableEffectIndex(player))>=0){
    print_number(n);
    pwup->initial_effect(player);
    player->effects[n].active = 1;
    player->effects[n].time_left = PWUP_DURATION;
    player->effects[n].final_effect = pwup->final_effect;
  }
  pwup->active = 0;
  _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,pwup->id);
}

void checkEffects(Player * player){
  for(int i=0; i<MAX_ACTIVE_EFFECTS; i++){
    if(player->effects[i].active){
      if(player->effects[i].time_left == 0){
        player->effects[i].final_effect(player);
        player->effects[i].active = 0;
      }
      player->effects[i].time_left--;
    }
  }
}

void* game_sound(void* ss){
  int fd = 0;
  int finished = 0;
  char* buff = malloc();
  while((fd = _call_int80(INT_OPENFIFO,"sound")) == 0);
  while(!finished){
    int size = _call_int80(INT_READBLOQFIFO,fd,buff,4096);
    for(int i = 0; i<size;i++){
      buff[i] = 0;
    }
    if(!size){
      finished = 1;
    }
    beep(0);
  }
}

void* game_keyboard(void* ss){
  int fd = 0;
  int finished = 0;
  char* buff;
  while((fd = _call_int80(INT_OPENFIFO,"keyboard")) == 0);
  while(!finished){
    _call_int80(INT_GET_STR,buff); // ESTE SI
    _call_int80(INT_WRITEFIFO,fd,buff,strlen(buff));
  }
}
