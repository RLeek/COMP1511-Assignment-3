
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
#define TRUE  1
#define FALSE 0
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
int best_sell(struct location *b, int *largest, int maximum_move, 
    struct bot *bot);
struct score *add_element(struct score *head, struct location *curr, 
    int distance, int maximum_move, int maximum_fruit, int cash);
int add_score(struct score *item, int maximum_move, int maximum_fruit, 
    int cash);
char *largest_element(struct score *head, int *largest);
int best_buy(struct location *b, int maximum_move, struct bot *bot);
int buy_fuel(struct bot *b);
int closet_charger(struct bot *b, int *direction, int maximum_move);
int shortest_distance(char *name, struct location *b, int *direction);
char *valid_charger(struct score *head, int *distance);
int number_bots(struct location *location);
int available_seller(struct bot *b);
int available_buyer(struct bot *b);
int available_charger(struct bot *b);
int unsafe_fuel_level(struct bot *b, int *direction);



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

//Prints our desired player name
void print_player_name(void) {
    printf("PLUS ULTRA");

}

//Prints out our desired move
void print_move(struct bot *b) {

    int direction = EAST;

    if (b->fruit != NULL) {

        int largest;
        int maximum_move = b->maximum_move;
        int move = best_sell(b->location, &largest, maximum_move, b);

        if (move != 0) {
            printf("move %d", move);
        } else if (available_seller(b)) {
            printf("sell %d", b->maximum_fruit_kg);
        }
    } else if ((b->fruit == NULL) && (b->turns_left > 2)) {

        int maximum_move = b->maximum_move;
        int move = best_buy(b->location, maximum_move, b);

        if (unsafe_fuel_level(b, &direction)) {
            if (available_charger(b)) {
                int buy = buy_fuel(b);
                printf("buy %d", buy);
            } else {
                int move = closet_charger(b, &direction, b->maximum_move);
                printf("move %d", direction * move);
            }
        } else if (move != 0) {
            printf("move %d", move);
        } else if (available_buyer(b)) {
            printf("buy %d", b->maximum_fruit_kg);
        }
    } else {
        printf("move 0");
    }
}



//The main strategy for testing if my code works
//as intended is by seeing the decisions my
//bot makes in simple worlds.

//Each world will test a different
//function in the bot and ensures
//that the bot is always choosing
//the optimal choice for
//my code

//The worlds used in these tests 
//can be seen in the txt file
//world_unit_tests.txt
//Which I should have sent by email 
//With the subject:COMP1511 assignment 3 Unit test worlds

//After these tests, the code was run
//in random large worlds to 
//ensure there were no errors


void run_unit_tests(void) {

    //These first tests will test the decision making of the bot 
    //When buying fruit



    //Test1: This ensures that the bot will buy fruit if
    //there is only one location that sells fruit
    //see world1

    //Test2: This ensures that our bot does not buy any fruit
    //when there are less than 2 turns left
    //see world2


    //The next few tests ensure that the bot chooses
    //the optimal location ot buy fruit
    //when given two choices


    //Test3: This ensures that the bot, when given two fruit-buying locations 
    //with the same price, quantity and distance, but with different
    //buyers, will choose the one that can be sold for more
    //see world3

    //Test4: This ensures that the bot, when given two fruit-buying locations 
    //with the same price and distance, but with a different quantity and
    //different buyers, will choose the one that can be sold for more
    //(essentially tests that the bot ignores quantity)
    //see world4


    //Test5:This ensures that the bot, when given two fruit-buying locations 
    //with the same price and quantity, but different distances, will choose
    //the closer one
    //see world5

    //Test6:This ensures that the bot, when given two fruit-buying locations 
    //with the same price, but different distances and quantity, will choose
    //the closer one
    //(essentially tests that the bot ignores quantity)
    //see world6


    //Test7:This ensures that the bot, when given two fruit-buying locations 
    //with the same distance and quantity, but different prices, will choose
    //the cheaper one
    //see world7

    //Test8:This ensures that the bot, when given two fruit-buying locations 
    //with the same distance and quantity, but different prices, will choose
    //the cheaper one
    //(essentially tests that the bot ignores quantity)
    //see world8




    //These next tests will test the decision making of the bot 
    //When selling fruit



    //Test1: This ensures that our bot sells fruit a valid location 
    //that isn't a compost heap.
    //see world9



    //The next few tests ensure that the bot chooses
    //the optimal location ot buy fruit
    //when given two choices


    //Test2: This ensures that the bot, wheng give two fruit-selling locations
    //with the same price and quantity but different distances, will choose 
    //the closer one
    //see world10

    //Test3: This ensures that the bot, wheng give two fruit-selling locations
    //with the same price but different distances and quantities, will choose 
    //the closer one
    //(essentially tests that the bot ignores quantity)
    //see world11


    //Test4: This ensures that the bot, wheng give two fruit-selling locations
    //with the same distance and quantity but different prices, will choose 
    //the cheaper one
    //see world12

    //Test5: This ensures that the bot, wheng give two fruit-selling locations
    //with the same distance and quantity but different prices, will choose 
    //the cheaper one
    //(essentially tests that the bot ignores quantity)
    //see world13




    //These next tests ensures that our bot buys electricity when our
    //electricity levels are low enough


    //Test1: This ensures that our bot will buy the correct amount of
    //electricity when there are only a few turns left
    //see world14

    //Test2: This ensures that our bot will buy the correct amount of 
    //electricity when there are a large number of turns left
    //see world15

}


//Checks to see if we are at a location where we can sell our fruits 
int available_seller(struct bot *b) {

    int true_false = FALSE;

    if ((b->location->price > 0) && (b->location->quantity > 0)) {
        if (strcmp(b->location->fruit, b->fruit) == 0) {
            true_false = TRUE;
        } else if (strcmp(b->location->fruit, "Anything") == 0) {
            true_false = TRUE;
        }
    }
    return true_false;
}

//Checks to see if we are at a location where we can buy fruit
int available_buyer(struct bot *b) {

    int true_false = FALSE;

    if ((b->location->price < 0) && (b->location > 0)) {
        if (strcmp(b->location->fruit, "Electricity") != 0) {
            true_false = TRUE;
        }
    }
    return true_false;
}

//Checks to see if we are at a location where we can buy
//Electricity
int available_charger(struct bot *b) {

    int true_false = FALSE;

    if (strcmp(b->location->fruit, "Electricity") == 0) {
        if (b->location->quantity > 0) {
            true_false = TRUE;
        }
    }
    return true_false;
}

//Checks to see if we have a safe fuel level
int unsafe_fuel_level(struct bot *b, int *direction) {

    int true_false = FALSE;
    int safe_fuel = b->battery_level - 2 * (b->maximum_move);

    if (safe_fuel <= closet_charger(b, direction, b->maximum_move)) {
        if (closet_charger(b, direction, b->maximum_move) != -100) {
            true_false = TRUE;
        }
    }
    return true_false;
}



//Finds the optimal seller for a fruit and returns
//the direction,distance as well as the score of the
//optimal seller.
//The score of the optimal seller is used when this function
//is called by best_buy.
//The direction and distance are used by print_move.
int best_sell(struct location *place, int *largest, int maximum_move, 
    struct bot *b) {

    //Ensures that we choose the correct fruit to find
    //a seller for, depending on where this function
    //is called
    char *fruit;
    if (b->fruit == NULL) {
        fruit = place->fruit;
    } else {
        fruit = b->fruit;
    }

    //Initializing for the loop
    struct location *inital = place;
    struct location *curr = place;

    int distance = 0;
    int initializer = 0;
    int items = 0;
    int maximum_fruit = b->maximum_fruit_kg;
    int cash = b->cash;

    struct score *east_head = NULL;

    //Finds valid selling locations in the east;
    while ((strcmp(curr->name, inital->name)!= 0 ) || (initializer == 0)) {
        if ((curr->price > 0) && (strcmp("Anything", curr->fruit) == 0)) {
            east_head = add_element(east_head, curr, distance,  maximum_move, 
                maximum_fruit, cash);
            items++;
        } else if ((curr->price > 0) && (strcmp(fruit, curr->fruit) == 0)) {
            east_head = add_element(east_head, curr, distance,  maximum_move, 
                maximum_fruit, cash);
            items++;
        }
        initializer = 1;
        distance++;
        curr = curr->east; 
    }

    //Initializing for the loop
    inital = place;
    curr = place;

    distance = 0;
    initializer = 0;

    struct score *west_head = NULL;

    //Finds valid selling locations in the west
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if ((curr->price > 0) && (strcmp("Anything", curr->fruit) == 0)) {
            west_head = add_element(west_head, curr, distance, maximum_move, 
                maximum_fruit, cash);
            items++;
        } else if ((curr->price > 0) && (strcmp(fruit, curr->fruit) == 0)) {
            west_head = add_element(west_head, curr, distance, maximum_move, 
                maximum_fruit, cash);
            items++;
        }
        initializer = 1;
        distance++;
        curr = curr->west; 
    }
  
    //Lowest possible value for an integer
    //(that is allowed)
    //Esentially means that
    //there will always be a seller chosen
    //even if that seller has 0 quanitty
    int east_largest = -1874919424;
    int west_largest = -1874919424;

    char *east_name = largest_element(east_head, &east_largest);
    char *west_name = largest_element(west_head, &west_largest);

    if (east_largest >= west_largest) {

        struct score *curr2 = east_head;

        while((curr2 != NULL) && (curr2->num_score < east_largest)) {
            curr2 = curr2->next;
        }

        char *name = east_name;
        *largest = east_largest;
        int direction;

        return shortest_distance(name, place, &direction) * direction;
    } else {

        struct score *curr2 = west_head;

        while((curr2 != NULL) && (curr2->num_score < west_largest)) {
            curr2 = curr2->next;
        }

        char *name = west_name;
        *largest = west_largest;
        int direction;

        return shortest_distance(name, place, &direction) * direction;
    }
}



//Finds the best location to buy
//This is done by first calculating a score for each location that sells fruit
//then calling best_sell for these locations and adding 
//the score of the best buyer found by best_sell
int best_buy(struct location *place, int maximum_move, struct bot *b) {

    //Initializing for the loop
    struct location *inital = place;
    struct location *curr = place;

    int initializer = 0;
    int distance = 0;
    int buyable = 0;
    int maximum_fruit = b->maximum_fruit_kg;
    int cash = b->cash;

    struct score *beast_head = NULL;

    //Finds valid buying locations in the east
    //Note: beast_head as in buying east_head;
    while ((strcmp(curr->name, inital->name) != 0) || (initializer == 0)) {
        if ((curr->price < 0) && (strcmp("Electricity", curr->fruit) != 0)) {
            int largest;
            best_sell(curr, &largest, maximum_move, b);

            beast_head = add_element(beast_head, curr, distance, maximum_move,
                maximum_fruit,cash);
            beast_head->num_score = beast_head->num_score + largest;
            buyable++;
        }
        curr = curr->east; 
        distance++;
        initializer = 1;
    }

    //Initalizing for the loop
    inital = place;
    curr = place;

    distance = 0;
    initializer = 0;

    struct score *bwest_head = NULL;

    //Finds valid buying location in the west
    //Note: bwest_head as in buying west head;
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if (curr->price < 0 && (strcmp("Electricity", curr->fruit) != 0)) {
            int largest;
            best_sell(curr,&largest,maximum_move, b);

            bwest_head = add_element(bwest_head, curr, distance, maximum_move,
                maximum_fruit,cash);
            bwest_head->num_score = bwest_head->num_score + largest;
            buyable++;

        }
        initializer = 1;
        distance++;
        curr = curr->west; 
    }

    //Lowest possible value for an integer
    //(that is allowed)
    //Esentially means that
    //there will always be a seller chosen
    //even if that seller has 0 quanitty
    int beast_largest =  -1874919424;
    int bwest_largest =  -1874919424;

    char *beast_name = largest_element(beast_head, &beast_largest);
    char *bwest_name = largest_element(bwest_head, &bwest_largest);

    if (buyable == 0) {
        return 0;
    }

    if (beast_largest >= bwest_largest) {
        struct score *curr2 = beast_head;

        while ((curr2 != NULL) && (curr2->num_score != beast_largest)) {
            curr2 = curr2->next;
        }

        char *name = beast_name;
        int direction;

        return shortest_distance(name, place, &direction) * direction;

    } else {
        struct score *curr2 = bwest_head;
        while ((curr2 != NULL) && (curr2->num_score != bwest_largest)) {
            curr2 = curr2->next;
        }

        char *name = bwest_name;
        int direction;

        return shortest_distance(name, place, &direction) * direction;
    }
    
}

//Finds the shortest distance from our bots
//location to the desired place
int shortest_distance(char *name, struct location *place, int *direction) {

    struct location *curr = place;

    int distance_west = 0;
    while (strcmp(name,curr->name) != 0) {
        curr = curr->west;
        distance_west++;
    }
    curr = place;
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


//Adds an element to our struct called 'score'
struct score *add_element(struct score *head, struct location *curr,
    int distance, int maximum_move, int maximum_fruit, int cash) {

    struct score *item = malloc(sizeof(struct score));

    item->name = curr->name;
    item->fruit = curr->fruit;
    item->distance = distance;
    item->quantity = curr->quantity;
    item->price = curr->price;
    item->num_bots = number_bots(curr);
    item->num_score = add_score(item, maximum_move, maximum_fruit,cash);
    item->next = head;

    head = item;

    return head;
}

//Finds the number of bots at a location
int number_bots(struct location *location) {

    if (location->bots == NULL) {
        return 0;
    }
    struct bot_list *curr = location->bots;
    int num = 0;
    while (curr->next != NULL) {
        num++;
        curr = curr->next;
    }
    return num;
}

//Calculates a score that is used to 
//find whether the optimal
//buying/selling place
int add_score(struct score *item, int maximum_move, int maximum_fruit, 
    int cash) {

    int score = 4 * item->price - 2 * item->distance;

    //If a location has too many bots
    //It's score is severaly reduced
    if (item->quantity - (item->num_bots * maximum_fruit) < 0) {
        score = -100000000;
    }

    //If we can't buy an item then
    //don't go to the location
    if (cash+item->price < 0 && item->price < 0) {
        score = -100000000;
    }

    //If the place does not buy or sell
    //then ignore it
    if (item->quantity == 0) {
        score =  -100000000;
    }

    //If the item is beyond our 
    //maximum moves
    //then reduce the value of the score
    if (item->distance > maximum_move) {
        if (score > 0) {
            score = score/10;
        }
        if (score < 0) {
            score = score * 10;
        }
    }

    //If the place sells anything
    //Go there only if there is no other
    //valid locatiom
    if (strcmp("Anything", item->fruit) == 0) {
        score = score -10000000;
    }
    return score;
}

//Locates the largest element from our score struct
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

//Locates the closet charger
//in an easterly direction
int closet_charger(struct bot *b, int *direction, int maximum_move) {

    //Initialization
    struct location *inital = b->location;
    struct location *curr = b->location;

    int distance = 0;
    int initializer = 0;

    char *e_name = NULL;

    //Goes through east and finds a valid charger
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if ((strcmp("Electricity", curr->fruit) == 0) && curr->quantity > 0) {
            e_name = curr->name;
            break;
            //We use a break statement here
            //because distance is the only parameter
            //that really needs to be considered
            //Hence the first charger we find
            //is likely the best charger
        }
        initializer = 1;
        distance++;
        curr = curr->east; 
    }

    //Initalization
    inital = b->location;
    curr = b->location;

    distance = 0;
    initializer = 0;

    char *w_name = NULL;

    //Goes through west and finds a valid charger
    while (strcmp(curr->name, inital->name)!= 0 || (initializer == 0)) {
        if ((strcmp("Electricity", curr->fruit) == 0) && curr->quantity > 0) {
            w_name = curr->name;
            break;
            //We use a break statement here
            //because distance is the only parameter
            //that really needs to be considered
            //Hence the first charger we find
            //is likely the best charger
        }
        initializer = 1;
        distance++;
        curr = curr->west; 
    }

    //Returns a value which tells
    //our bot that there
    //are no valid chargers
    if ((w_name == NULL) && (e_name == NULL)) {
        return -100;
    }

    int e_distance = shortest_distance(e_name, b->location, direction);
    int w_distance = shortest_distance(w_name, b->location, direction);

    if (e_distance >= w_distance) {
        *direction = WEST;
        return w_distance;
    } else {
        *direction = EAST;
        return w_distance;
    }
}

//Decides the amount of fuel we need to buy
int buy_fuel(struct bot *b) {

    int fuel1 = (b->maximum_move * b->turns_left)/3;
    int fuel2 = -(b->cash/b->location->price)/2;

    if (fuel2 < fuel1){
        return fuel2;
    } else {
        return fuel1;
    }
}

