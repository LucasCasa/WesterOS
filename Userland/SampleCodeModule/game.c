#include "game.h"

char board[WIDTH*HEIGHT];

uint32_t last[] = {1,2,3,4,5,6};
uint32_t next[] = {7,8,9,11,12};

Color c[] = {{255,0,0},{0,0,255},{0,255,0},{255,0,255},{255,255,0},{255,255,255}};
char* controls[6][2] = {{"A","D"},{"J","L"},{"L Arrow","R Arrow"},{"Z","C"},{"I","P"},{"1","3"}};
int starting = 1;

void (*powerup_effects[NUM_POWERUPS][2])(Player*) = {{powerUp_cleanScreen,nothing},{powerUp_speed,powerUp_speed_end}};
Color powerup_color[NUM_POWERUPS] = {{0,191,255},{42,223,60}};
PowerUp powerups[MAX_POWERUPS];
int num_powerup, powerup_cont, powerup_next;

Player p[MAX_PLAYERS];

void game(){
  _call_int80(INT_ENTER_DRAW_MODE);
  _call_int80(INT_CLEAR);

  //AMOUNT OF PLAYERS
  int nplayers = lobby();

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
    while(pass < 6500000){
      pass++;
    }

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

    for(int i = 0; i<nplayers;i++){
      if(p[i].alive){
        checkEffects(&(p[i]));
        if(!starting){
           p[i].time_no_inv++;
        }
        if(p[i].time_no_inv == p[i].time_with_inv + HOLE_SIZE){
          p[i].time_no_inv = 0;
          p[i].time_with_inv = rand() % MAX_DRAW;
        }
        p[i].angle += 3*p[i].mod;
        p[i].acum.x += 3*_cos(p[i].angle)*p[i].speed;
        p[i].acum.y += 3*_sin(p[i].angle)*p[i].speed;
        p[i].pos.x = p[i].acum.x;
        p[i].pos.y = p[i].acum.y;
        if(p[i].time_no_inv < p[i].time_with_inv){
          _call_int80(INT_DRAW_CIRCLE,&(p[i].pos),p[i].radius,&c[i]);
          p[i].alive = draw_into_board(i+1,p[i].pos);
          if(p[i].erasable > 0){
            _call_int80(INT_UNDRAW_ERASABLE_CIRCLE,p[i].erasable);
            p[i].erasable = -1;
          }
        }else{
          if(p[i].erasable > 0){
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
    if(total_alive <= nplayers -1){
      for(int i = 0; i<nplayers;i++){
        if(p[i].alive){
          exit_game();
          print_message("Player ",0xFF);
          print_number(i+1);
          print_message(" has won!\n",0xFF);
          return;
        }
      }
      exit_game();
      print_message("Tie\n",0xFF);
      return;
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
    }while(cha < '2' || cha > '6');
      return cha-'0';
}
void exit_game(){
  _call_int80(INT_UNSET_EVENT_KEYUP);
  _call_int80(INT_UNSET_EVENT_KEYDOWN);
  _call_int80(INT_EXIT_DRAW_MODE);
  _call_int80(INT_ERASE_SCR);
}
void get_key_down(uint8_t key){
  switch(key){
    case 'a':
      p[0].mod =-1;
      break;
    case 'd':
      p[0].mod = 1;
      break;
    case 'j':
      p[1].mod = -1;
      break;
    case 'l':
      p[1].mod = 1;
      break;
    case '4':
      p[2].mod = -1;
      break;
    case '6':
      p[2].mod = 1;
      break;
    case 'z':
      p[3].mod = -1;
      break;
    case 'c':
      p[3].mod = 1;
      break;
    case 'i':
      p[4].mod = -1;
      break;
    case 'p':
      p[4].mod = 1;
      break;
    case '1':
      p[5].mod = -1;
      break;
    case '3':
      p[5].mod = 1;
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
char draw_into_board(uint32_t pn,Point point){
  if(point.x-RADIUS < 0 || point.x+RADIUS > WIDTH){
    return 0;
  }
  if(point.y-RADIUS < 0 || point.y+RADIUS > HEIGHT){
    return 0;
  }
  for(signed int y=-RADIUS ; y<=RADIUS; y++){
    for(signed int x=-RADIUS ; x<=RADIUS; x++){
      if(x*x+y*y <= RADIUS*RADIUS){
        if(board[point.x + x + (point.y+y)*WIDTH ] == last[pn-1] || board[point.x + x + (point.y +y)*WIDTH] == 0){
          board[point.x + x + (point.y +y)*WIDTH ] = next[pn-1];
        }else{
          print_message("Crash at",0xFF);
          print_number(point.x + x);
          print_message(",",0xFF);
          print_number(point.y +y);
          print_message("Extra",0xFF);
          print_number(board[point.x + x + (point.y +y)*WIDTH]);
          print_number(pn);
          print_number(next[pn-1]);
          print_number(last[pn-1]);

          return 0;
        }
      }
    }
  }
  uint32_t aux;
  aux = last[pn - 1];
  last[pn - 1] = next[pn - 1];
  next[pn - 1] = aux;
  return 1;
}

void init_players(int nplayers){
  for(int i = 0; i<nplayers;i++){
    p[i].id = i;
    p[i].pos.x = rand() % (WIDTH-200) + 100;
    p[i].pos.y = rand() % (HEIGHT-160) + 80;
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
}

void powerUp_speed(Player * trigger){
  trigger->speed *= 5;
}

void powerUp_speed_end(Player * player){
  player->speed /= 5;
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
  k = getRandIndex(MAX_POWERUPS);
  powerups[n].initial_effect = powerup_effects[k][0];
  powerups[n].final_effect = powerup_effects[k][1];

  powerups[n].id = _call_int80(INT_DRAW_ERASABLE_CIRCLE,&(powerups[n].pos),powerups[n].radius,&(powerup_color[n]));
}

void managePowerups(){
  if(powerup_cont == powerup_next){
    createNewPowerup();
    powerup_cont = 0;
    powerup_next = rand()%(MAX_POWERUP_TIME - MIN_POWERUP_TIME) + MIN_POWERUP_TIME;
  }

  powerup_cont++;
}

int square(int n){
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
  if((n=getAvailableEffectIndex(player))>0){
    pwup->initial_effect(player);
    player->effects[n].active = 1;
    player->effects[n].time_left = 1000; // MAGIC NUMBER CAMBIAR DESP
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