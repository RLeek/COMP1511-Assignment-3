
// fruit_bot.c
// Assignment 3, COMP1511 18s1: Fruit Bot
//
// This program by Ricky Liu (z5207969) on 31/3/2018
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: minor bug in main fixed

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "fruit_bot.h"

#define WEST    -1
#define EAST    1

struct score {
    char *name;
    char *fruit;
    int distance;
    int quantity; 
    int price;
    int num_bots;
    int num_score;
    struct score *next;
};


void print_player_name(void);
void print_move(struct bot *b);
void run_unit_tests(void);

// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
int best_sell(struct location *b, int *largest, int maximum_move, struct bot *bot);
struct score *add_element(struct score *head, struct location *curr, int distance, int maximum_move, int maximum_fruit);
int add_score(struct score *item, int maximum_move, int maximum_fruit);
char *largest_element(struct score *head, int *largest);
int best_buy(struct location *b, int maximum_move, struct bot *bot);
int adjusted(int move);
int buy_fuel(struct bot *b);
int closet_charger(struct bot *b, int *direction, int maximum_move);
int shortest_distance(char *name, struct location *b, int *direction);
char *valid_charger(struct score *head, int *distance);
int number_bots(struct location *location);

// YOU SHOULD NOT NEED TO CHANGE THIS MAIN FUNCTION

int main(int argc, char *argv[]) {

    if (argc > 1) {
        // supply any command-line argument to run unit tests
        run_unit_tests();
        return 0;
    }

    struct bot *me = fruit_bot_input(stdin);
    if (me == NULL) {
        print_player_name();
    } else {
        print_move(me);
    }

    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("mxyzptlk");

}
void print_move(struct bot *b) {
    
    int direction = EAST;
    if (b->fruit != NULL) {
        int largest;
        int maximum_move = b->maximum_move;
        int move = best_sell(b->location, &largest, maximum_move, b);
        if (move != 0) {
            printf("move %d", adjusted(move));
        } else if (b->location->price > 0 && b->location->quantity > 0 && (strcmp(b->location->fruit, b->fruit) ==0)) {
            printf("sell %d", b->maximum_fruit_kg);
        } else if (b->location->price > 0 && b->location->quantity > 0 && (strcmp(b->location->fruit, "Anything") ==0)) {
            printf("sell %d", b->maximum_fruit_kg);
        }

    } else if (b->fruit == NULL && b->turns_left>2) {
        int maximum_move = b->maximum_move;
        int move = best_buy(b->location, maximum_move, b);
        if (b->battery_level - 2*(b->maximum_move) <=  closet_charger(b, &direction,b->maximum_move) && closet_charger(b, &direction,b->maximum_move) != -1000000) {
            if (strcmp(b->location->fruit,"Electricity") == 0 && b->location->quantity > 0) {
                int buy = buy_fuel(b);
                printf("buy %d", buy);
            } else {
                int move = closet_charger(b, &direction, b->maximum_move);
                printf("move %d", direction*move);
            }
        } else if (move != 0){
            printf("move %d", adjusted(move));
        } else if (b->location->price < 0 && b->location->quantity > 0 && strcmp(b->location->fruit,"Electricity")!= 0) {
            printf("buy %d", b->maximum_fruit_kg);
        } 

    } else {
        printf("move 0");
    }
}

//We don't want to give it a location
//We only give a location if it contains the same fruit as the bot
//If we want to sell this should always be the case, but not necessarly!!
int best_sell(struct location *b, int *largest, int maximum_move, struct bot *bot) {
    struct location *inital = b;
    struct location *curr = b;
    int distance = 0;
    int initializer = 0;
    int items = 0;
    //Goes through east
    struct score *east_head = NULL;
    char *fruit;

    if (bot->fruit == NULL) {
        fruit = b->fruit;
    } else {
        fruit = bot->fruit;
    }

    int maximum_fruit = bot->maximum_fruit_kg;

    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if ((curr->price > 0) && ((strcmp(fruit, curr->fruit) == 0) || (strcmp("Anything", curr->fruit) == 0))) {
            east_head = add_element(east_head, curr, distance,  maximum_move, maximum_fruit);
            items++;

        }
        initializer = 1;
        distance++;
        curr = curr->east; 
    }

    inital = b;
    curr = b;
    distance = 0;
    initializer = 0;
    //Goes through east
    struct score *west_head = NULL;
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if (curr->price > 0 && ((strcmp(fruit, curr->fruit) == 0)|| (strcmp("Anything", curr->fruit) == 0))) {
            west_head = add_element(west_head, curr, distance, maximum_move, maximum_fruit);
            items++;

        }
        initializer = 1;
        distance++;
        curr = curr->west; 
    }
  
    int east_largest = -1874919424;
    int west_largest = -1874919424;
    char *east_name = largest_element(east_head, &east_largest);//Goes through west
    char *west_name = largest_element(west_head, &west_largest);//Goes through east

    struct score *curr2 = east_head;
    while (curr2 != NULL) {
        curr2 = curr2->next;
    }
    
    curr2 = west_head;
    while (curr2 != NULL) {
        curr2 = curr2->next;
    }

    if (items == 0) {
        *largest = -1000;
        fprintf(stderr, "THERE IS NO SITUATION WHERE THIS SHOULD RUN");
        return 0;
    }

    if (east_largest >= west_largest) {
        struct score *curr2 = east_head;
        while(curr2!=NULL && curr2->num_score < east_largest) {
            curr2 = curr2->next;
        }
        char *name = east_name;
        *largest = east_largest;
        int direction;
        return shortest_distance(name, b, &direction) * direction;
    } else {
        struct score *curr2 = west_head;
        while(curr2!=NULL && curr2->num_score < west_largest) {
            curr2 = curr2->next;
        }
        char *name = west_name;
        *largest = west_largest;
        int direction;
        return shortest_distance(name, b, &direction) * direction;
    }
}



int best_buy(struct location *b, int maximum_move, struct bot *bot) {

    int buyable = 0;
    struct location *inital = b;
    struct location *curr = b;
    int initializer = 0;
    int distance = 0;
    //Goes through east
    int maximum_fruit = bot->maximum_fruit_kg;

    struct score *beast_head = NULL;
    while ((initializer == 0) ||strcmp(curr->name, inital->name)!= 0) {
        if (curr->price < 0 && (strcmp("Electricity", curr->fruit) != 0)) {
            beast_head = add_element(beast_head, curr, distance, maximum_move,maximum_fruit);
            int largest;
            best_sell(curr, &largest, maximum_move, bot);
            beast_head->num_score = beast_head->num_score + largest;
            buyable++;
        }
        curr = curr->east; 
        distance++;
        initializer = 1;
    }
    inital = b;
    curr = b;
    distance = 0;
    initializer = 0;
    //Goes through east
    struct score *bwest_head = NULL;
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if (curr->price < 0 && (strcmp("Electricity", curr->fruit) != 0)) {
            bwest_head = add_element(bwest_head, curr, distance, maximum_move,maximum_fruit);
            int largest;
            best_sell(curr,&largest,maximum_move, bot);
            bwest_head->num_score = bwest_head->num_score + largest;
            buyable++;

        }
        initializer = 1;
        distance++;
        curr = curr->west; 
    }

    int beast_largest =  -1874919424;
    int bwest_largest =  -1874919424;

    char *beast_name = largest_element(beast_head, &beast_largest);//Goes through west
    char *bwest_name = largest_element(bwest_head, &bwest_largest);//Goes through east

    struct score *curr2 = beast_head;

    if (buyable == 0) {
        return 0;
    }

    if (beast_largest >= bwest_largest) {
        curr2 = beast_head;
        while (curr2 != NULL && curr2->num_score != beast_largest) {
            curr2 = curr2->next;
        }
        char *name = beast_name;
        int direction;
        return shortest_distance(name, b, &direction) * direction;

    } else {
        curr2 = bwest_head;
        while(curr2!=NULL && curr2->num_score != bwest_largest) {
            curr2 = curr2->next;
        }
        char *name = bwest_name;
        int direction;
        return shortest_distance(name, b, &direction) * direction;
    }
    
}

int shortest_distance(char *name, struct location *b, int *direction) {
    struct location *curr = b;

    int distance_west = 0;
    while (strcmp(name,curr->name) != 0) {
        curr = curr->west;
        distance_west++;
    }
    curr = b;
    int distance_east = 0;
    while (strcmp(name,curr->name) != 0) {
        curr = curr->east;
        distance_east++;
    }
    if (distance_west >= distance_east) {
        *direction = EAST;
        return distance_east;
    } else  {
        *direction = WEST;
        return distance_west;
    }
}



struct score *add_element(struct score *head, struct location *curr, int distance, int maximum_move, int maximum_fruit) {
    struct score *item = malloc(sizeof(struct score));
    item->name = curr->name;
    item->fruit = curr->fruit;
    item->distance = distance;
    item->quantity = curr->quantity;
    item->price = curr->price;
    item->num_bots = number_bots(curr);
    item->num_score = add_score(item, maximum_move, maximum_fruit);
    item->next = head;
    head = item;
    return head;
}

int number_bots(struct location *location) {
    if (location->bots == NULL) {
        return 0;
    }
    struct bot_list *curr = location->bots;
    int num = 0;
    while (curr->next != NULL) {
        num++;
        curr = curr ->next;
    }
    return num;

}


int add_score(struct score *item, int maximum_move, int maximum_fruit) {
    int score = 4 * item->price - 2 * item->distance;
    if (item->quantity - (item->num_bots * maximum_fruit) < 0) {
        score = -100000000;
    }

    if (item->quantity == 0) {
        score =  -100000000;
    }
    if (item->distance > maximum_move) {
        if (score > 0) {
            score = score/10;
        }
        if (score < 0) {
            score = score *10;
        }
    }

    if (strcmp("Anything", item->fruit) == 0) {
        score = score -10000000;
    }

    return score;
}

char *largest_element(struct score *head, int *largest) {
    char *name = "NONE";

    struct score *curr = head;
    while (curr != NULL) {
        if (curr->num_score >= *largest) {
            *largest = curr->num_score;
            name = curr->name;
        }
        curr = curr->next;
    }
    return name;
}

int adjusted(int move) {
    return move;
}

int closet_charger(struct bot *b, int *direction, int maximum_move) {

    struct location *inital = b->location;
    struct location *curr = b->location;
    int distance = 0;
    int initializer = 0;
    char *name = NULL;
    //Goes through east
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if ((strcmp("Electricity", curr->fruit) == 0) && curr->quantity > 0) {
            name = curr->name;
            break;
            //We need to explain why this exists
        }
        initializer = 1;
        distance++;
        curr = curr->east; 
    }

    if (name == NULL) {
        return -1000000;
    }

    int final_distance = shortest_distance(name, b->location, direction);

    return final_distance;
}



char *valid_charger(struct score *head, int *distance) {
    char *name = "NONE";

    struct score *curr = head;
    while (curr != NULL) {
        if (curr->quantity > 0) {
            name  = curr->name;
            *distance = curr->distance;
            curr = NULL;
        } else {
            curr = curr->next;
        }
    }
    return name;
}


int buy_fuel(struct bot *b) {
    int fuel1 = (b->maximum_move * b->turns_left)/3;
    int fuel2 = -(b->cash/b->location->price)/2;

    if (fuel2<fuel1){
        return fuel2;
    } else {
        return fuel1;
    }

}

void run_unit_tests(void) {

    //We want to test
    //Buy_fuel
    //add_score
    
    
    
    //Write several worlds to test each function:
        //For best_buy:
            //Test what it buys when there is only one purchase place
        
            //Write a world that sells two goods with the same price and quantity and distance but one has a buyer whilst the other has an anything

            //Write a world that sells two goods with the same price and quantity but different distances
            //Write a world that sells two goods with the same price, but different quantities and different distances
            
            //Write a world that sells two goods with different prices and same quantity and distances
            //Write a world that sells two goods with different prices and different quantity and same distances
            
            //Test a world where there are less than two turns
            
            
           
        //For best_sell:
            //same thing but for best_buy
            //also include one where there is only a sell anything place
            //
            
            
         //include unit test for the other functions, hyup;
        
            
            
            //Write a world that sells only one good 
























    // PUT YOUR UNIT TESTS HERE
    // This is a difficult assignment to write unit tests for,
    // but make sure you describe your testing strategy above.
}




//Test buy_fuel
//Test largest largest_element
//add_score;
//number bots

