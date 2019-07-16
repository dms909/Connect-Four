#include <iostream>
#include <ncurses.h>
#include <unistd.h>

class player{
public:
  int color;
};


class board{
public:
  player *game_map[6][7];
  int game_win;
  int game_turn;
};

void init_terminal(){
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void display_board(board *b){
  int i, j;
  clear();
  for(i=0; i<6; i++){
    for(j=0; j<7; j++){
      if(b->game_map[i][j] == NULL){
	mvaddch(i + 3, j + 4, '*');
      }
      else{
	attron(COLOR_PAIR(b->game_map[i][j]->color));
	mvaddch(i + 3, j + 4, '@');
	attroff(COLOR_PAIR(b->game_map[i][j]->color));
      }
    }
  }  
}

int check_win(board *b, int color){
  int i, j;
  int m, n;
  int count = 0;
  //check diagonal down
  for(i = 0; i < 6; i++){
    for(j = 0; j < 7; j++){
      if(b->game_map[i][j] != NULL && b->game_map[i][j]->color == color){
	for(m = 1; m < 4; m++){
	  if(i + m < 6 && j + m < 7 && b->game_map[i + m][j + m] != NULL && b->game_map[i+m][j+m]->color == color){
	    count++;
	  }
	  else{
	    break;
	  }
	}
	if(count == 3){
	  b->game_win = 1;
	  return 1;
	}
	count = 0;
      }
    }
  }

  //check upward diagonal
  for(i = 0; i < 6; i++){
    for(j = 0; j < 7; j++){
      if(b->game_map[i][j] != NULL && b->game_map[i][j]->color == color){
	for(m = 1; m < 4; m--){
	  if(i - m >= 0 && j + m < 6 && b->game_map[i - m][j + m] != NULL && b->game_map[i-m][j+m]->color == color){
	    count++;
	  }
	  else{
	    break;
	  }
	}
	if(count == 4){
	  b->game_win = 1;
	  return 1;
	}
	count = 0;
      }
    }
  }

  //check vertical
  for(i = 0; i < 6; i++){
    for(j = 0; j < 7; j++){
      if(b->game_map[i][j] != NULL && b->game_map[i][j]->color == color){
	for(m = 1; m < 4; m++){
	  if(i + m < 6 && b->game_map[i + m][j] != NULL && b->game_map[i+m][j]->color == color){
	    count++;
	  }
	  else{
	    break;
	  }
	}
	if(count == 3){
	  b->game_win = 1;
	  return 1;
	}
	count = 0;
      }
    }
  }


  for(i = 0; i < 6; i++){
    for(j = 0; j < 7; j++){
      if(b->game_map[i][j] != NULL && b->game_map[i][j]->color == color){
	for(m = 1; m < 4; m++){
	  if(j + m < 7 && b->game_map[i][j+m] != NULL && b->game_map[i][j+m]->color == color){
	    count++;
	  }
	  else{
	    break;
	  }
	}
	if(count == 3){
	  b->game_win = 1;
	  return 1;
	}
	count = 0;
      }
    }
  }
  return 0;
}

int check_move(board *b, int x){
  if(b->game_map[0][x] != NULL){
    return 0;
  }
  return 1;
}


void gen_board(board *b){
  int i, j;
  b->game_win = 0;
  b->game_turn = 0;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 7; j++){
      b->game_map[i][j] = NULL;
    }
  }
}

void game_turn_red(board *b){
  int key, esc, slot, i;
  mvprintw(25, 15, "Choose a slot");
  slot = 0;
  attron(COLOR_PAIR(COLOR_RED));
  mvaddch(2, 4 + slot, '@');
  attroff(COLOR_PAIR(COLOR_RED));
  while(esc != 1){
    key = getch();
    if(key == KEY_UP){
      attron(COLOR_PAIR(COLOR_RED));
      mvaddch(2, 4 + slot, ' ');
      slot++;
      if(slot != 7){
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_RED));
      }
      else{
	slot = 0;
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_RED));
      }
    }
    else if(key == KEY_DOWN){
      attron(COLOR_PAIR(COLOR_RED));
      mvaddch(2, 4 + slot, ' ');
      slot--;
      if(slot != -1){
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_RED));
      }
      else{
	slot = 6;
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_RED));
      }
    }
    else if(key == 's'){
      if(check_move(b, slot) == 1){
	for(i = 0; i < 6; i++){
	  if(b->game_map[i][slot] != NULL){
	    b->game_map[i-1][slot] = new player;
	    b->game_map[i-1][slot]->color = COLOR_RED;
	    check_win(b, COLOR_RED);
	    break;
	  }
	  if(i == 5){
	    b->game_map[i][slot] = new player;
	    b->game_map[i][slot]->color = COLOR_RED;
	    check_win(b, COLOR_RED);
	  }
	}
	esc = 1;
      }
      else{
	mvprintw(10, 5, "Invalid move");
      }
    }
    else if(key == 27){
      esc = 1;
      b->game_win = 1;
    }
  }
  mvaddch(2, 4 + slot, ' ');
  b->game_turn = 1;
}

void game_turn_magenta(board *b){
  int key, esc, slot, i;
  mvprintw(25, 15, "Choose a slot");
  slot = 0;
  attron(COLOR_PAIR(COLOR_MAGENTA));
  mvaddch(2, 4 + slot, '@');
  attroff(COLOR_PAIR(COLOR_MAGENTA));
  while(esc != 1){
    key = getch();
    if(key == KEY_UP){
      attron(COLOR_PAIR(COLOR_MAGENTA));
      mvaddch(2, 4 + slot, ' ');
      slot++;
      if(slot != 7){
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
      }
      else{
	slot = 0;
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
      }
    }
    else if(key == KEY_DOWN){
      attron(COLOR_PAIR(COLOR_MAGENTA));
      mvaddch(2, 4 + slot, ' ');
      slot--;
      if(slot != -1){
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
      }
      else{
	slot = 6;
	mvaddch(2, 4 + slot, '@');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
      }
    }
    else if(key == 's'){
      if(check_move(b, slot) == 1){
	for(i = 0; i < 6; i++){
	  if(b->game_map[i][slot] != NULL){
	    b->game_map[i-1][slot] = new player;
	    b->game_map[i-1][slot]->color = COLOR_MAGENTA;
	    check_win(b,COLOR_MAGENTA);
	    break;
	  }
	  if(i == 5){
	    b->game_map[i][slot] = new player;
	    b->game_map[i][slot]->color = COLOR_MAGENTA;
	    check_win(b, COLOR_MAGENTA);
	  }
	}
	
	esc = 1;
      }
      else{
	mvprintw(10, 5, "Invalid move");
      }
    }
    else if(key == 27){
      esc = 1;
      b->game_win = 1;
    }
  }
  b->game_turn = 0;
}





int main(int argc, char *argv[]){
  board b;
  init_terminal();
  
  gen_board(&b);
  display_board(&b);

  while(b.game_win != 1){
    if(b.game_turn == 0){
      game_turn_red(&b);
      display_board(&b);
    }
    else{
      game_turn_magenta(&b);
      display_board(&b);
    }
  }

  
  endwin();

  if(b.game_turn == 1){
    std::cout << "Red wins!!" << std::endl;
  }
  else{
    std::cout << "Magenta wins!!" << std::endl;
  }
}
