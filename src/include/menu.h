#ifndef MENU_H
#define MENU_H


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"

#define SUCCESS 1
#define FAIL 0

extern int exit_loop;
extern int level_up;

struct menu_template
{
	int cursor_index;
	int num_of_items;
	char* name;
	struct menu_item** items;

};

// need a new menu item structure
// it holds 3 pieces of information (for now)
// 	- parent menu ptr 			- to enable the user to go back a level
// 	- child menu ptr  			- to enable the user to go down a level
// 	- function ptr (void *?)    - when the item is supposed to do sth, not open another menu

struct menu_item
{
	struct menu_template* parent_menu;
	struct menu_template* child_menu;
	char *name;
	void (*callback)();
};

/*
@brief Each menu encapsulates a number of items, those items are created with this funcion.
*/

void createItem(char* name, 
				struct menu_template* parent, 
				struct menu_template* child, 
				void (*callback)(),
				struct menu_item*);

/*
@brief In order to create a new menu, a few key information are needed about it's elements (items)


@param [in/out] struct menu_template* menu ->	Pointer to the menu that needs to be created

@param [in]		struct menu_template* parent_menu -> This pointer is used for navigation, i.e. 
													   every item is an element in a tree (this tree is an analogy for the menu).
													   Each element (apart from the root node/element) contains a parent element
													   to which the cursor should be returned to when the user wants to jump
													   one level back...

@param [in] 	struct menu_template** child_menu 	-> This array of pointers is used for navigation, i.e.
													   if you want to go a level deeper, this pointer is going to contain the child
													   address...

@param [in]		void(*callback_functions[])()		-> Array of callback function pointers. When the item is a leaf element (no 
													   more submenues), this function pointer is used to bind some function behavior
													   to the item.


*/
void fillMenu(struct menu_template* menu,
			  struct menu_template* parent_menu,
			  struct menu_template** child_menus,
			  void (*callback_functions[])(), 
			  char item_names[][50], 
			  int num_of_items);


/*
@brief Function used for displaying the active menu to the screen

@param [in]	struct menu_template* 		-> Pointer to the menu to be displayed
@param [in] int 						-> Number of rows in the screen
@param [in] int 						-> Number of columns in the screen
*/
void printMenu(struct menu_template*, int, int);

#endif