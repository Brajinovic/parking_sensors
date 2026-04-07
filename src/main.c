#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "hello.h"
#include "menu.h"
#include "const.h"
#include "callback.h"


struct menu_template main_menu;
struct menu_template config_menu;


// global variables
int exit_loop = 0;
int level_up = 0;

int main(int argc, char** argv)
{
	// screen dimension variables 
	int row;
	int col;

	int active_menu_index = 0;
	struct menu_template* active_menu = &main_menu;
  
  // key input variable
  int input = 0;
  // menu cursor positioning variables
  // given the cursor index for the "parent" menu, 
  // I know which sub menu should be open...
  int cursor_index = 0;
  // 0 - main menu
  // 1 - sub menu
  // 2 - sub sub menu...
  int menu_level = 0;
  
  int num_of_items = 0;

  initscr();				/* Start curses mode */
  
  getmaxyx(stdscr,row,col);
  noecho();
  curs_set(0); // hide cursor
  
  fillMenu(&main_menu, 
			 NULL,

			 (struct menu_template*[]) {	// child menu
										NULL, 
										&config_menu,
										NULL},
			 (void (*[])(void)){ 					// callback function pointers
							&callbackStartUpload,
							&callbackConfigSettings,
							&callbackExit},

			 (char[][50]) {
							 "Start upload\0",
							 "Configure settings\0",
							 "Exit\0"}, 
							 NUM_OF_ITEMS_MAIN_MENU);

  
  fillMenu(&config_menu, 
				 &main_menu,
				 (struct menu_template*[]){	// child
																 NULL, 
																 NULL,
																 NULL},
				 (void (*[])(void)){ 				// callback function pointers
																 NULL,
																 NULL,
																 &callbackLevelUp},
					(char[][50]) {
																	 "COM port\0",
																	 "UART bandwidth\0",
																	 "Back\0"}, 
																	 NUM_OF_ITEMS_CONFIG_MENU);




  num_of_items = main_menu.num_of_items;
  printMenu(&main_menu, row, col);

  while(1)
  {
	refresh();		
	input = getch();
	  
	if (input == 66) // arrow key down
	{	
		cursor_index = ++cursor_index < num_of_items ? cursor_index++ : 0;
		active_menu->cursor_index = cursor_index;
	}
	else if (input == 65) // arrow key up
	{
		cursor_index = --cursor_index >= 0 ? cursor_index-- : num_of_items - 1;
		active_menu->cursor_index = cursor_index;
	}
	else if (input == 10) // enter
	{
		if (active_menu->items[cursor_index]->child_menu != NULL)
		{
			active_menu = active_menu->items[cursor_index]->child_menu;
			num_of_items = active_menu->num_of_items;
			cursor_index = 0;
		} else
		{
			mvprintw(0, 1, " No submenu! Checking for a callback function! \n\r");
			if (active_menu->items[cursor_index]->callback != NULL)
			{
				mvprintw(1, 1, " There is a callback!\n\r ");
				active_menu->items[cursor_index]->callback();
			} else
			{
				mvprintw(1, 1, " Well fuck it, no submenu or callback! \n\r");
			}
		}
			}
		
		if (level_up == 1)
		{
			active_menu = active_menu->items[cursor_index]->parent_menu;
			num_of_items = active_menu->num_of_items;
		cursor_index = active_menu->cursor_index;
			level_up = 0;
		}

	if (input == 'q')
		exit_loop = 1;

	if (exit_loop == 1)
		break;
	
	printMenu(active_menu, row, col);
  }

  endwin();                       	/* End curses mode */
  
  return 0;
}