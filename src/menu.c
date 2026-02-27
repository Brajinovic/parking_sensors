#include "menu.h"


void createItem(char* name, 
				struct menu_template* parent, 
				struct menu_template* child, 
				void (*callback)(),
				struct menu_item* item)
{
	item->name = (char*)calloc(sizeof(char), MAX_MENU_ITEM_SIZE);
	// copy the name value from stack to heap
	// *((*item).name) = *name;
	memcpy(item->name, name, MAX_MENU_ITEM_SIZE);
	// store the pointers
	item->parent_menu = parent;
	item->child_menu = child;
	item->callback = callback;
}


void fillMenu(struct menu_template* menu,
			  struct menu_template* parent_menu,
			  struct menu_template** child_menus,
			  void (*callback_functions[])(), 
			  char item_names[][50], 
			  int num_of_items)
{
	struct menu_item* item;
	
	menu->cursor_index = 0;
	menu->num_of_items = num_of_items;
	menu->items = (struct menu_item**)calloc(sizeof(struct menu_item*), (*menu).num_of_items);

	for (int i = 0; i < menu->num_of_items; i++)
	{
		// allocate the memory for an item and fill it out
		item = (struct menu_item*)calloc(sizeof(struct menu_item), 1);
		createItem(item_names[i],
				   parent_menu,
				   child_menus[i],
				   callback_functions[i],
				   item);
		// the menu.items is an array of pointers to the menu_item 
		// I need to add the address of each item to that array
		menu->items[i] = item;
		if (i == 0)	mvprintw(2, 1, " %s hihi\n\r", menu->items[i]->name);
	}	
}



void printMenu(struct menu_template* menu, int row, int col)
{	
	clear();

	for( int i = 0; i < menu->num_of_items; i++)
	{

		if (menu->cursor_index == i) attron(A_STANDOUT);
  		
  		mvprintw(((row - menu->num_of_items) / 2) + i, 
  				 (col - MAX_MENU_ITEM_SIZE) / 2, 
  				 " %s ", 
  				 menu->items[i]->name);
  				// "Majmun\0");
  		if ((*menu).cursor_index == i) attroff(A_STANDOUT);
  	
	}
}
	

