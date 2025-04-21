#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "buckai.h"
#include "inventory.h"
#include "io.h"
#include "move.h"

#define RISK_TOLERANCE 0.2

int main(int argc, char **argv)
{
    printf("BuckAI\nBy Mohammad Niaz\n");

    int inventory[INVENTORY_SIZE] = {ITEM_NONE};

    int player_health = 5;
    int opponent_health = 5;

    int live_count = 0;
    int blank_count = 0;

    int round_count = 0;

    bool is_opponent_turn = false;

    bool is_gun_sawed = false;
    bool is_opponent_cuffed = false;

    int computed_move = MOVE_NULL;
    int move_outcome = OUTCOME_NULL;
    int glass_outcome = OUTCOME_NULL;

    while (player_health > 0 && opponent_health > 0)
    {
        round_count++;
        live_count = 0;
        blank_count = 0;
        is_opponent_turn = false;
        is_gun_sawed = false;
        is_opponent_cuffed = false;
        computed_move = MOVE_NULL;
        move_outcome = OUTCOME_NULL;
        glass_outcome = OUTCOME_NULL;

        printf("\n\nStarting round %d...\n\n", round_count);

        // Prompt operator for newly received items
        int new_items[INVENTORY_SIZE] = {ITEM_NONE};
        print_item_list();
        prompt_vi(
            "Please enter the items you received this round:\n>",
            new_items,
            INVENTORY_SIZE,
            NULL);
        printf("\n");
        inventory_add_items(inventory, new_items);
        print_inventory(inventory);

        // Prompt operator for new shell counts
        live_count = prompt_i("Enter the new live shell count: >");
        blank_count = prompt_i("Enter the new blank shell count: >");

        // Prompt operator for whose turn is it
        is_opponent_turn = prompt_b("Is the opponent going first: (y/n)>");

        while (live_count > 0 || blank_count > 0)
        {
            if (player_health <= 0 || opponent_health <= 0)
                break;

            printf("\nPlayer Health: %d || Opponent Health: %d\n", player_health, opponent_health);
            printf("Live Shells: %d || Blank Shells: %d\n\n", live_count, blank_count);

            if (is_opponent_turn)
            {
                // Prompt operator for outcome of opponent move
                int opp_live_used;
                int opp_blank_used;
                int player_new_health;
                int opp_new_health;
                printf("Waiting on opponent move...\n");
                opp_live_used = prompt_i("How many live shells did the opponent expend: >");
                opp_blank_used = prompt_i("How many blank shells did the opponent expend: >");
                player_new_health = prompt_i("What is your new health: >");
                opp_new_health = prompt_i("What is the opponent's new health: >");

                // Set new state based on opponent move outcome
                live_count -= opp_live_used;
                blank_count -= opp_blank_used;
                player_health = player_new_health;
                opponent_health = opp_new_health;

                // Continue to next move
                is_opponent_turn = false;
                continue;
            }

            // Compute move (Bulk of AI logic)
            printf("Computed move:\n");

            // Heal if medicine is available
            while (player_health > 1 && player_health < 5 && inventory_has_item(inventory, ITEM_MEDICINE))
            {
                inventory_use_item(inventory, ITEM_MEDICINE);
                player_health++;
                printf("Used 1x medicine to heal to %d.\n", player_health);
            }

            // Exhaust all available sodas, or until there is no risk
            while (inventory_has_item(inventory, ITEM_SODA) && live_count > 0 && blank_count > 0)
            {
                inventory_use_item(inventory, ITEM_SODA);
                printf("Used 1x soda.\n");
                bool soda_was_live = prompt_b("Was the racked shell live: (y/n)>");
                if (soda_was_live)
                {
                    printf("Live shell expended.\n");
                    live_count--;
                }
                else
                {
                    printf("Blank shell expended.\n");
                    blank_count--;
                }
            }

            // Use a glass if there is still risk
            glass_outcome = OUTCOME_NULL;
            if (inventory_has_item(inventory, ITEM_GLASS) && live_count > 0 && blank_count > 0)
            {
                inventory_use_item(inventory, ITEM_GLASS);
                printf("Used 1x magnifier.\n");
                bool glass_was_live = prompt_b("Was the exposed shell live: (y/n)>");
                if (glass_was_live)
                    glass_outcome = OUTCOME_LIVE;
                else
                    glass_outcome = OUTCOME_BLANK;
            }

            // Decide which player to shoot
            if ((blank_count > 0 && live_count < 1) || glass_outcome == OUTCOME_BLANK) // Scenario where all or current shell are blank
            {
                // Shoot player
                computed_move = MOVE_SHOOT_PLAYER;
            }
            else if ((live_count > 0 && blank_count < 1) || glass_outcome == OUTCOME_LIVE) // Scenario where all or current shell are live
            {
                // Saw the gun for maximum damage yield
                if (inventory_has_item(inventory, ITEM_SAW))
                {
                    inventory_use_item(inventory, ITEM_SAW);
                    is_gun_sawed = true;
                    printf("Used 1x saw.\n");
                }

                // Handcuff to prevent handoff if not the last shell
                if ((live_count + blank_count) > 1 && inventory_has_item(inventory, ITEM_CUFFS))
                {
                    inventory_use_item(inventory, ITEM_CUFFS);
                    is_opponent_cuffed = true;
                    printf("Used 1x handcuffs.\n");
                }

                // Shoot opponent
                computed_move = MOVE_SHOOT_OPPONENT;
            }
            else // Scenario where shells are mixed
            {
                // Shoot player if risk is tolerable
                float riskf = (float)live_count / (float)(live_count + blank_count);
                printf("Current risk factor: %.2f\n", riskf);
                if (riskf < RISK_TOLERANCE)
                    computed_move = MOVE_SHOOT_PLAYER;
                else
                    computed_move = MOVE_SHOOT_OPPONENT;
            }

            // Fire gun and prompt operator for shot outcome
            switch (computed_move)
            {
            case MOVE_SHOOT_PLAYER:
                printf("Shoot player.\n");
                break;
            case MOVE_SHOOT_OPPONENT:
                printf("Shoot opponent.\n");
                break;
            default:
                printf("Logical error - Default to shoot player.\n");
                break;
            }

            bool outcome_was_live = prompt_b("\nWas the shot shell live: (y/n)>");
            if (outcome_was_live)
                move_outcome = OUTCOME_LIVE;
            else
                move_outcome = OUTCOME_BLANK;

            printf("\n");

            // Handle shot outcome
            switch (move_outcome)
            {
            case OUTCOME_LIVE:
                live_count--;
                printf("Live shell expended.\n");
                if (computed_move == MOVE_SHOOT_PLAYER)
                {
                    player_health--;
                    printf("Player health decremented.\n");
                    if (is_gun_sawed)
                    {
                        player_health--;
                        is_gun_sawed = false;
                        printf("Additional player health decremented due to sawed-off gun.\n");
                    }
                }
                else if (computed_move == MOVE_SHOOT_OPPONENT)
                {
                    opponent_health--;
                    printf("Opponent health decremented.\n");
                    if (is_gun_sawed)
                    {
                        opponent_health--;
                        is_gun_sawed = false;
                        printf("Additional opponent health decremented due to sawed-off gun.\n");
                    }
                }
                if (is_opponent_cuffed)
                {
                    printf("Opponent moved blocked with handcuffs.\n");
                    is_opponent_cuffed = false;
                }
                else
                {
                    printf("Proceeding to opponent move.\n");
                    is_opponent_turn = true;
                }
                break;
            case OUTCOME_BLANK:
                blank_count--;
                printf("Blank shell expended.\n");
                if (is_gun_sawed)
                {
                    printf("Sawed-off gun exhausted.\n");
                    is_gun_sawed = false;
                }
                if (computed_move == MOVE_SHOOT_PLAYER)
                {
                    printf("Player awarded extra move.\n");
                    break;
                }
                if (is_opponent_cuffed)
                {
                    printf("Opponent move blocked with handcuffs.\n");
                    is_opponent_cuffed = false;
                }
                else
                {
                    printf("Proceeding to opponent move.\n");
                    is_opponent_turn = true;
                }
                break;
            default:
                printf("Logical error - Default move outcome.\n");
                if (is_opponent_cuffed)
                    is_opponent_cuffed = false;
                else
                    is_opponent_turn = true;
                break;
            }
        }
    }

    printf("\n\n");

    if (opponent_health <= 0)
    {
        printf("Player victory.\n");
    }
    else
    {
        printf("Player defeat.\n");
    }

    printf("Program complete.\n\n");

    return 0;
}
