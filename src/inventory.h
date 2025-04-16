#ifndef _INVENTORY_H
#define _INVENTORY_H

#define INVENTORY_SIZE 8
#define ITEM_NONE 0
#define ITEM_MEDICINE 1
#define ITEM_SODA 2
#define ITEM_GLASS 3
#define ITEM_SAW 4
#define ITEM_CUFFS 5

bool inventory_has_item(int *inventory, int item);
void inventory_add_items(int *inventory, int *new_items);
bool inventory_use_item(int *inventory, int item);
void print_item_list(void);
void print_inventory(int *inventory);
const char *translate_item_name(int item);

#endif
