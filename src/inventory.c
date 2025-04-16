#include <stdbool.h>
#include <stdio.h>
#include "buckai.h"
#include "inventory.h"

bool inventory_has_item(int *inventory, int item)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (inventory[i] == item)
            return true;
    }
    return false;
}

void inventory_add_items(int *inventory, int *new_items)
{
    int i = 0;
    while (i < INVENTORY_SIZE)
    {
        int new_item = new_items[i];
        if (new_item != ITEM_NONE)
        {
            for (int j = 0; j <= INVENTORY_SIZE; j++)
            {
                if (j == INVENTORY_SIZE)
                {
                    printf("Inventory full. Skipping...\n");
                    return;
                }
                if (inventory[j] == ITEM_NONE)
                {
                    inventory[j] = new_item;
                    break;
                }
            }
        }
        i++;
    }
}

bool inventory_use_item(int *inventory, int item)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (inventory[i] == item)
        {
            inventory[i] = ITEM_NONE;
            return true;
        }
    }
    return false;
}

void print_item_list(void)
{
    printf("Item List:\n");
    for (int item = 0; item <= 5; item++)
        printf("%s: %d\n", translate_item_name(item), item);
    printf("\n");
}

void print_inventory(int *inventory)
{
    printf("Current Inventory:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        int item = inventory[i];
        const char *item_name = translate_item_name(item);
        printf("[%d]: %s\n", i + 1, item_name);
    }
    printf("\n");
}

const char *translate_item_name(int item)
{
    switch (item)
    {
    case ITEM_NONE:
        return "None";
    case ITEM_MEDICINE:
        return "Medicine";
    case ITEM_SODA:
        return "Soda";
    case ITEM_GLASS:
        return "Magnifier";
    case ITEM_SAW:
        return "Saw";
    case ITEM_CUFFS:
        return "Handcuffs";
    default:
        return NULL;
    }
}
