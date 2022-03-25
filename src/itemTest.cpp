#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

using std::cin;
using std::cout;
using std::endl;
using std::printf;
using std::string;
using std::vector;

class player;

class item
{
protected:
    string desc;

public:
    string getDesc() const;
    virtual void use(player &p) = 0;
};

class player
{
public:
    int pos[2] = {9, 9};
    int health = 5;         // may add enemies or traps, undecided but want a type of hazard
    vector<item> inventory; // may change

    void displayStatus();

    void useItem();
};

string item::getDesc() const
{
    return desc;
}

class compass : public item
{
private:
public:
    compass()
    {
        desc = "A golden compass that points to your goal...\n";
    }

    void use(player &p) override
    {
        cout << "COMPASS FUNCTION" << endl;
        // float distance = sqrt((constants::GOAL_POS[1] - p->pos[1]) + (constants::GOAL_POS[0] - p->pos[0]));

        // printf("You are %05.2f spaces away from your goal.\n"); // may go back and change goal to be a user inputted string
    }
};

// for some reason this code doesn't want to work in the main program

void player::displayStatus()
{
    // print out inventory items and health.
}

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
            cout << " " << i << " : " << it->getDesc() << endl;
        }

        cout << "Enter the number of the item you wish to use, otherwise enter 0:" << endl;
        cin >> userInput;

    } while (userInput != 0 && (userInput < 0 && userInput > inventory.size()));

    if (userInput == 0)
        return;

    int index = userInput - 1;
    cout << index << " " << inventory.at(index).getDesc();
    inventory.at(index).use(*this);
}

int main()
{
    player p = player();
    item *a;
    compass b = compass();
    a = &b;
    a->use(p);
    // I dont know why this is working here but not in the main program...

    vector<item> vec;
    vec.push_back(b);
    // vec.at(0).use(p);
    //^this part is the issue so i have decided to ignore it for now and investigate later
}