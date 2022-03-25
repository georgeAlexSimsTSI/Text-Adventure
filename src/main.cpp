/**
 *
 * minimum:
 *  traverse around a map - done
 *  have a goal - done
 *  have a golden compass that shows your distance to the goal - done
 *
 * ideas:
 *  some more hazards,
 *      - some kind of enemy that if you run into chases you to a different part of the map, maybe a bear in a cave
 *      - traps that you disarm if you inspect them (maybe 50 / 50 chance you take damage)
 *
 *  some more blockers, I like the idea of there being a house that you can try to interact with
 *
 *  some more items, thinking tools or weapons
 *      - clippers to clear away brambles or a flint and steel to set a fire
 *
 *  a portal could be cool...but would be a pain to have 2 way ones
 *
 *
 *  Things I wanted to do:
 *      wanted items and cells to be objects and work via polymorphism, check itemTest.cpp for details
 *
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::printf;
using std::string;
using std::vector;

class player;
class item;
class compass;
// for a 2d array coordinate position I am going with [y][x]

/**
 * @brief namespace to store const variables
 */
namespace constants
{
    const int GOAL_POS[] = {1, 1};
    const int MAX_Y = 15;
    const int MAX_X = 15;
};

/**
 * @brief Utility method that determines if a given position is in the specified bounds of the created map
 *
 * @param coord the position to be determined
 * @return true if the coord falls within the bounds
 * @return false if the coord falls outside of the bounds
 */
bool isInBounds(int coord[2])
{
    // cout << "Y: " << coord[0] << " X: " << coord[1] << endl;
    return ((coord[0] >= 0 && coord[0] < constants::MAX_Y) || (coord[1] >= 0 && coord[1] < constants::MAX_X));
}

/**
 * @brief Utility method that gets the compass bearing for an angle from 0 - 365 e.g. North or North by North West
 *
 * @param angle this is the angle in degrees that the needle would point
 * @return string that represents one of the ordinal or cardinal directions
 */
string getBearing(float angle)
{
    // 16 points with a distance of 22.5 each
    // could be cool to return an arrow pointing in that direction
    // something like this would be cool but not doing it any time soon https://ascii.co.uk/art/compass

    if (angle > 348.75 || angle <= 11.25)
        return "North";
    if (angle > 11.25 && angle <= 33.75)
        return "North by North East";
    if (angle > 33.75 && angle <= 56.25)
        return "North East";
    if (angle > 56.25 && angle <= 78.25)
        return "East by North East";
    if (angle > 78.25 && angle <= 101.25)
        return "East";
    if (angle > 101.25 && angle <= 123.75)
        return "East by South East";
    if (angle > 123.75 && angle <= 146.25)
        return "South East";
    if (angle > 146.25 && angle <= 168.75)
        return "South by South East";
    if (angle > 168.75 && angle <= 191.25)
        return "South";
    if (angle > 191.25 && angle <= 213.75)
        return "South by South West";
    if (angle > 213.75 && angle <= 236.25)
        return "South West";
    if (angle > 236.25 && angle <= 258.75)
        return "West by South West";
    if (angle > 258.75 && angle <= 281.25)
        return "West";
    if (angle > 281.25 && angle <= 303.75)
        return "West by North West";
    if (angle > 303.75 && angle <= 326.25)
        return "North West";
    if (angle > 326.25 && angle <= 348.75)
        return "North by North West";
}

/**
 * @brief Class that represents the player, stores relevant data and provides functionality
 *
 */
class player
{
public:
    int pos[2] = {9, 9};
    float health = 11;
    int distanceTraveled = 0;
    const int MAX_HEALTH = 20;
    vector<char> inventory; // may change

    void displayStatus();
    void useItem();
    void compass();
    void itemDescription(char c);
    void use(char c, int index);
};

/**
 * @brief Simple method that displays the players current health, number of items and distance travelled
 *
 */
void player::displayStatus()
{

    cout << "Status" << endl;
    cout << string(50, '-') << endl;
    // List how many items you have, flavour text for bag is heavy if you have many items?
    string numberOfItems = (inventory.size() < 3) ? "Your bag is light" : "Your bag is heavy";
    string plural = (inventory.size() > 1) ? " items" : " item";
    cout << numberOfItems << " carrying : " << inventory.size() << plural << endl;

    // print out inventory items and health.
    // Display current health
    cout << string(8, ' ') << string(MAX_HEALTH + 2, '-') << endl;
    cout << "health :"
         << "|" << string(health, 176) << string((MAX_HEALTH - health), ' ') << "|" << endl;
    cout << string(8, ' ') << string(MAX_HEALTH + 2, '-') << endl;

    // List how far you have walked
    cout << "In pursuit of your goal you have walked: " << distanceTraveled << " miles." << endl;
    cout << string(50, '-') << endl;
    cout << endl
         << endl;
}

/**
 * @brief A method that runs when the compass is used. It determines the distance and the direction of the goal to the player
 *
 */
void player::compass()
{
    int yDis = constants::GOAL_POS[1] - this->pos[1];
    int xDis = constants::GOAL_POS[0] - this->pos[0];
    double distance = sqrt(pow(yDis, 2) + pow(xDis, 2));

    float angle = (atan2(xDis, yDis) * (180 / M_PI)) + 450;
    // angle = (angle < 0) ? angle + 450 : +90;
    angle = (angle >= 360) ? angle - 360 : angle;

    // cout << angle << endl;

    string bearing = getBearing(angle);

    printf("The goal is %05.2f paces ", distance); // may go back and change goal to be a user inputted string
    cout << bearing << endl;
}

/**
 * @brief A method that uses a given item. This is either done here or calls another method
 *
 * @param c a char representing the item to be used. If consumable it may be removed from the inventory.
 * @param index the index of the item in the inventory
 */
void player::use(char c, int index)
{
    switch (c)
    {
    case 'c':
    case 'C':
        compass();
        break;
    case 'h':
    case 'H':
        cout << "You drink the potion and feel your vigor return " << endl;
        health = MAX_HEALTH;
        inventory.erase(inventory.begin() + index);
        break;
    case 'p':
    case 'P':
        cout << "You drink the unkown potion, probably not the best idea..." << endl;
        health -= 3;
        cout << "You feel yourself weaken " << endl;
        inventory.erase(inventory.begin() + index);
        break;
    default:
        cout << "unexpected item code" << endl;
    }
}

/**
 * @brief Simple switch statement that provides a description of different items.
 *
 * @param c a character representing a specific item
 */
void player::itemDescription(char c)
{
    switch (c)
    {
    case 'c':
    case 'C':
        cout << "A golden compass that points to your goal." << endl;
        break;
    case 'h':
    case 'H':
        cout << "A health potion of known make, should restore you to peak health." << endl;
        break;
    case 'p':
    case 'P':
        cout << "A potion of unkown origin, its effect is unkown." << endl;
        break;
    default:
        cout << "unexpected item code." << endl;
    }
}

/**
 * @brief method that outputs the players inventory then takes in userinput to determine which item to use.
 *
 */
void player::useItem()
{
    if (inventory.size() == 0)
    {
        cout << "You have no Items, I would suggest finding an Item near where you started...";
        return;
    }

    // Display list of available items by index then take user input
    int userInput = -1;
    do
    {
        cout << "In your bag you find the following items: " << endl;
        unsigned short i = 1;
        for (auto it = inventory.begin(); it != inventory.end(); it++)
        {
            cout << " " << i++ << " : ";
            itemDescription(*it);
        }

        cout << "Enter the number of the item you wish to use, otherwise enter 0:" << endl
             << endl;
        cin >> userInput;

    } while (userInput != 0 && (userInput < 0 && userInput > inventory.size()));

    if (userInput == 0)
        return;

    int index = userInput - 1;
    use(inventory.at(index), index);
}

/**
 * Cells:
 *  ' ' empty plains with no functionality, this is what is out of the bounds of the created map
 *  'G' this is the goal, getting here wins the game
 *  'C' the golden compass
 *  'T' spike trap
 *  'L' leaves
 *  '#' forrest
 *  '^' brambles that do minor damage to push through
 *  'H' health potion
 *  'h' fake potion
 *  '~' a thatched roof house with wooden walls. cant move through it, WIP
 */

struct game // using a struct just for simplicity and everything will be in this file
{
    char gameState;

    char map[constants::MAX_Y][constants::MAX_X] =
        {
            {' ', ' ', ' ', ' ', ' ', '^', '^', ' ', ' ', 'T', '#', ' ', ' ', ' ', ' '},
            {' ', 'G', '#', 'L', 'T', ' ', '^', 'L', '#', '#', 'L', ' ', ' ', '#', ' '},
            {'^', ' ', 'L', '#', '#', 'T', ' ', ' ', 'L', '#', 'L', ' ', ' ', ' ', ' '},
            {'^', '#', '#', '#', 'L', ' ', ' ', ' ', 'L', '#', ' ', ' ', ' ', '^', '^'},
            {'^', '^', 'L', '#', 'L', ' ', '#', '#', ' ', 'T', ' ', ' ', ' ', '^', ' '},
            {' ', '^', '^', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '^', ' '},
            {' ', '^', '^', 'H', ' ', ' ', ' ', 'h', '#', ' ', ' ', ' ', ' ', '^', ' '},
            {' ', '^', '^', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', 'h', '^', ' ', '#', '#', ' ', ' ', ' ', 'P', ' ', ' ', '#', '#', '#'},
            {' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', 'T', '#', '~', '#'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'L', '#'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', ' ', ' ', '#', '#', '#'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' '}}; // this is a char array but may change to enums

    // current plan is that if the player moves outside of the preset bounds that its either open planes or ocean

    player p;

    /**
     * @brief Get the char at the position in the array, if not in bounds return ' '
     *
     * @param newLocation int array of 2 values for the position
     * @return char the returned char
     */
    char getCell(int newLocation[2]) const
    {
        return (isInBounds(newLocation)) ? map[newLocation[0]][newLocation[1]] : ' ';
    }

    /**
     * @brief after taking in the chosen direction determine and return the new position
     *
     * @param dir a char representing north, east, south, west as w a s d
     * @return int* a 2 int array created on the heap
     */
    int *getPosition(char dir) const
    {
        int playerPos[2] = {p.pos[0], p.pos[1]};
        int *newLocation = new int[2];
        newLocation[0] = playerPos[0];
        newLocation[1] = playerPos[1];

        switch (dir)
        {
        case ('w'):
        case ('W'):
            newLocation[0] -= 1;
            return newLocation;
        case ('s'):
        case ('S'):
            newLocation[0] += 1;
            return newLocation;
        case ('d'):
        case ('D'):
            newLocation[1] += 1;
            return newLocation;
        case ('a'):
        case ('A'):
            newLocation[1] -= 1;
            return newLocation;

        default:
            cout << "ERROR IN MOVEMENT";
            return newLocation;
        }
    }

    /**
     * @brief Method to take the userinput for the chosen direction, will loop until a valid input is provided
     *
     * @return a char representing north, east, south, west as w a s d
     */
    char getDirInput() const
    {
        char dir = ' ';
        bool valid = false;
        do
        {
            cout << "Enter a direction" << endl
                 << "w : North" << endl
                 << "a : West" << endl
                 << "d : East" << endl
                 << "s : South" << endl;
            cin >> dir;
            system("cls");
            switch (dir)
            {
            case ('w'):
            case ('W'):
            case ('s'):
            case ('S'):
            case ('d'):
            case ('D'):
            case ('a'):
            case ('A'):
                valid = true;
                break;
            default:
                cout << "Invalid direction";
            }

        } while (!valid);
        system("cls");
        return dir;
    }

    /**
     * @brief takes a userinput representing direction then returns a description of what is in that cell.
     * for certain cell types a different action will occur e.g. disarming a trap
     */
    void inspect()
    {

        bool valid = true;
        char userInput;
        const int(&playerPos)[2] = this->p.pos;

        cout << "Which area do you wish to inspect? " << endl;
        cout << string(50, '-') << endl;
        // going with +y is north and +x is east
        cout << "To the North: ";
        look(playerPos[0] - 1, playerPos[1]);
        cout << "To the East: ";
        look(playerPos[0], playerPos[1] + 1);
        cout << "To the South: ";
        look(playerPos[0] + 1, playerPos[1]);
        cout << "To the West: ";
        look(playerPos[0], playerPos[1] - 1);
        cout << string(50, '-') << endl;

        cout << endl
             << "Enter the direction you wish to inspect: " << endl;
        userInput = getDirInput();

        int *cellPosition = getPosition(userInput);

        char c = getCell(cellPosition);

        string inspectionMessage = "";
        switch (c)
        {
        case '^':
            inspectionMessage = "You inspect the mass of brambles and thorns... and you determine nothing useful. Botany isn't your strong point.";
            break;
        case '#':
            inspectionMessage = "You inspect the mysterious woods... and determine that the trees are made of wood."; // Change this to something useful
            break;
        case 'P':
            inspectionMessage = "You inspect the empty podium... and you find on base an inscription \" placeholder \" ... doesn't mean anything to you. ";
            break;
        case 'C':
            inspectionMessage = "You inspect the podium holding the compass... and you ask yourself why would someone leave a golden compass on a podium in the middle of a field? you push it out of your mind.";
            break;
        case 'H':
            inspectionMessage = "You inspect the box... and you find a shipping manifest on the side that lists the contents as potions from a well known seller";
            break;
        case 'h':
            inspectionMessage = "You inspect the box... and you find a shipping manifest on the side that lists the contents as potions from a unkown seller";
            break;
        case 'L':
            inspectionMessage = "You inspect the pile of leaves... after rumaging through the leaves you determine that there is nothing of interest here.";
            break;
        case 'T':
            inspectionMessage = "You inspect the mysterious pile of leaves... after moving them around a bit you discover that they were covering a pit with spikes in it... good thing you checked"
                                "\nAfter some effort you manage to disarm the trap and make it a bit less dangerous.";
            // disarm the trap
            map[cellPosition[0]][cellPosition[1]] = 'D';
            break;
        case 'D':
            inspectionMessage = "You inspect the disarmed trap... after carefully inspecting it you determine that this trap wasn't for a normal animal";
            break;
        case '~':
            inspectionMessage = "You inspect the strange house... all of the windows are boarded up and as far as you can tell no one is home but you aren't sure";
            break;
        case ' ':
        default:
            inspectionMessage = "You inspect the grassy expanse of land and quickly determine that yes it is indeed grass... you should probably only inspect things that arouse suspicion ";
        }

        cout << inspectionMessage << endl;

        delete cellPosition;
    }

    /**
     * @brief A method that takes in a x y position then outputs a description of what is in the cell. this is a more generic version of inspect
     *
     * @param x represents the x coordinate in the array [][x]
     * @param y represents the y coordinate in the array [y][]
     */
    void look(int x, int y) const
    {
        // check if in bounds
        int pos[] = {x, y};
        char cell = getCell(pos);
        // cout << "Cell identifier: " << cell << endl;
        switch (cell)
        {
        case 'H':
        case 'h':
            cout << "You see a wooden crate that is typically used to carry goods." << endl;
            break;
        case '^':
            cout << "You see a mass of brambles and thorns, It may be possible to push through it but you wont come out unscathed." << endl;
            break;
        case '#':
            cout << "A thick impenetrable wood stands in your way. you must go around it" << endl;
            break;
        case 'P':
            cout << "You see an empty podium. What was once stored here is long gone." << endl;
            break;
        case 'C':
            cout << "You see a podium holding a strange compass." << endl;
            break;
        case 'G':
            cout << "You see your goal." << endl;
            break;
        case 'L':
            cout << "You see a small pile of recently fallen leaves blocking your way." << endl;
            break;
        case 'T':
            cout << "You see a small pile of old leaves blocking your way." << endl;
            break;
        case 'D':
            cout << "You see a disarmed trap, should be safe to pass by." << endl;
            break;
        case '~':
            cout << "You see a house in the distance. The lights do not appear to be on and you cant see any signs of life." << endl;
            break;
        case ' ':
        default:
            cout << "Grasslands for as far as you can see." << endl;
            // cout << "ERROR, unrecognized char: \"" << cell << "\"" << endl; //going to have it default to open grass
            break;
        }
    }

    /**
     * @brief A method that calls the look(int, int) method for the 4 cells surronding the player
     *
     */
    void look() const // stating that this method does not change the state of any variables
    {
        const int(&playerPos)[2] = this->p.pos;

        cout << "You Look around: " << endl;
        cout << string(50, '-') << endl;
        // going with +y is north and +x is east
        cout << "To the North: ";
        look(playerPos[0] - 1, playerPos[1]);
        cout << "To the East: ";
        look(playerPos[0], playerPos[1] + 1);
        cout << "To the West: ";
        look(playerPos[0], playerPos[1] - 1);
        cout << "To the South: ";
        look(playerPos[0] + 1, playerPos[1]);
        cout << string(50, '-') << endl;
    }

    /**
     * @brief A method that takes in user direction input then tries to adjust he player position accordingly. different cell types will have different text and effects.
     * e.g. moving onto an item will pick it up, walking into a trap will set it off
     *
     */
    void movePlayer()
    {
        char dir = getDirInput();
        int *newPosition = getPosition(dir);
        char newCell = getCell(newPosition); // checks if its in bounds first

        switch (newCell)
        {
        case 'H':
            cout << "You move towards the box and empty the contents adding what you deem useful to your bag. Inside you find a health potion that should be useful." << endl;
            this->p.inventory.push_back('H');
            map[newPosition[0]][newPosition[1]] = ' ';
            break;
        case 'h':
            cout << "You move towards the box and empty the contents adding what you deem useful to your bag. Inside you find a potion that should be useful." << endl;
            this->p.inventory.push_back('P');
            map[newPosition[0]][newPosition[1]] = ' ';
            break;
        case '^':
            cout << "You push your way through the brambles but sustain some injuries in the process." << endl;
            p.health -= 2;
            break;
        case '#':
            cout << "You try to move through the woods but no matter what you try you find yourself back at the entrace to the woods." << endl;
            newPosition[0] = p.pos[0];
            newPosition[1] = p.pos[1];
            p.distanceTraveled--;
            break;
        case 'P':
            cout << "You move to the old podium. Nothing of interest is here." << endl;
            break;
        case 'C':
            cout << "You pick up the golden compass... you find a note:" << endl;
            cout << "Use me to get to your goal" << endl;
            this->p.inventory.push_back('C');
            map[newPosition[0]][newPosition[1]] = 'P';
            break;
        case 'G':
            // game should be over now
            cout << "You move towards your goal" << endl;
            gameState = 1;
            break;
        case 'L':
            cout << "You move through the leaves with no issue.";
            break;
        case 'T':
            cout << "As you move through the leaves you feel the ground open up beneath your leg." << endl
                 << "A spike goes through your foot.. you feel yourself weaken as what was on the spike enters your body" << endl;
            p.health -= 6;
            map[newPosition[0]][newPosition[1]] = 'D';
            break;
        case 'D':
            cout << "You pass by the disarmed trap without any issue.";
            break;
        case '~': // was originally going to have you get attacked and then wake up in a random part of the map injured and missing some items
            cout << "As you approach the house you fail to notice anything observing you... " << endl
                 << "You can't make out anything through the boarded windows but after a moment or 2 of trying the door you give up and walk away" << endl;
            newPosition[0] = p.pos[0];
            newPosition[1] = p.pos[1];
            p.distanceTraveled--;
            break;
        case ' ':
        default:
            cout << "You move through the grass lands" << endl;
            break;
        }

        p.distanceTraveled++;
        this->p.pos[0] = newPosition[0];
        this->p.pos[1] = newPosition[1];

        delete newPosition; // need to remove from heap after use
    }

    /**
     * @brief runs before the main game loop just to have the player pick up the compass.
     *
     * this was done because of a recomendation.
     */
    void setUp()
    {
        // place user and goal positions
        // have the player pick up the compass

        map[constants::GOAL_POS[0]][constants::GOAL_POS[1]] = 'G';
        cout << "While walking through open grass fields you see a strange podium holding a golden compass " << endl
             << "when you pick up the compass you find a note: " << endl
             << "\t\"Use me to get to your goal\"" << endl
             << endl;
        this->p.inventory.push_back('C');
        map[p.pos[0]][p.pos[1]] = 'P';
        // sleep(6);
    }

    /**
     * @brief This is the main game loop
     *
     */
    void start()
    {
        this->gameState = 0;
        char userInput;
        bool validInput;
        setUp(); // place the goal on the map and add the compass to the users inventory
        // system("cls");
        while (gameState == 0)
        {
            // want a user input loop
            do
            {
                cout << endl
                     << endl;
                this->p.displayStatus();
                look();

                cout << endl;
                cout << "MENU" << endl;
                cout << "____________________" << endl;
                cout << "1 : Move" << endl;
                cout << "2 : Inspect area" << endl;
                cout << "3 : Use Item" << endl;
                cout << "4 : Give up" << endl;
                cout << "____________________" << endl;
                cin >> userInput;
                system("cls"); // This is weirdly tempermental and doesn't consistently work
                validInput = true;
                switch (userInput)
                {
                case '1':
                    cout << "You have chosen to move:" << endl;
                    movePlayer();
                    break;
                case '2':
                    cout << "You have chosen to inspect an an area:" << endl;
                    inspect();
                    break;
                case '3':
                    cout << "You have chosen to use an item:" << endl;
                    p.useItem();
                    break;
                case '4':
                    cout << "You have perished..." << endl;
                    gameState = -1;
                    break;
                default:
                    validInput = false;
                }
                if (p.health < 1)
                    gameState = -1;
            } while (!validInput);
        }
        // display ending message

        switch (gameState)
        {
        case 1:
            cout << "congrats you have won" << endl;
            break;
        case -1:
            cout << "congrats you have died..." << endl
                 << "Tip: don't die" << endl;
            break;
        default:
            cout << "ERROR unexpected value";
            break;
        }
    }
};

int main()
{
    system("cls");

    game application;

    application.start();

    return 0;
}
