// This code is protected by the MIT License.  See README.txt for more information.  If README.txt is not present, simply Google "MIT License".  Copyright 2018 John Nguyen.
/* 
 ___               __   ___  __          ___  ___ ___
|   \ |  | |\   | /  \ |    /  \ |\   | |    |    |  \
|   | |  | | \  | | _  |--- |  | | \  | |--- |--- |--/
|   | |  | |  \ | |  \ |    |  | |  \ | |    |    | \
|___/ \__/ |   \| \__/ |___ \__/ |   \| |___ |___ |  \

*/
#include <iostream>
#include <string>
#include <stdlib.h> // These are preprocessor directives -- they tell the compiler to "include" these specified libraries of pre-written functions into your code
#include "utils.h"

using namespace std;

class NPC { // The non-player character class.  Includes all the variables and member functions required for a good D&D character.
	protected:
		unsigned short int strg, cons, dext, intl, chas, wisd; // Ability scores
		bool isIncap; // Is the NPC incapacitated?
		bool isUndead; // Is it an undead(aka a zombie)?  (this determines whether it can be damaged by a cleric presenting their holy symbol, for example)
		string wepstr1, wepstr2, wepstr3;
	public:
		unsigned short int damDie[3]; // Damage dies for the three attack slots
		string Name;
		short int npcHP; // HP for the NPC
		short int maxHP; // Maximum HP
		bool isEnemy; // Whether to damage the enemy or the player
		bool isDead; // Used to validate kill messages so the attack function doesn't have to
		unsigned int ID; // A number that could in later versions be used to identify NPCs
		unsigned short int lvl; // Level
		string tempstr;
	// Member functions
		void dealDamage (int, bool, bool); // Attack
		void takeDamage (int); // Modify own HP
		NPC (string, int, int, int, int, int, int, int, int, int, int, bool, int, int, string, string, string); // Set initial values
};

// Predefining functions to prevent compilation errors
void act ();
void move ();
void attack (int, bool, bool);
void setClass ();
int getModifier (int);

// Predefining variables to prevent compilation errors
unsigned short int init; // Unsigned short means from 0 to 255
unsigned short int cla;
int clas;
int atkRoll;
bool inCombat = false;

unsigned short int Str, Dex, Con, Intl, Wis, Cha, AC;
unsigned short int DamDie [3] = { 8, 4, 8 };
unsigned short int ScoreAssoc [3];
signed short int HP; // Signed short means from -255 to 255
unsigned short int maxHP;
// Predefining NPC "Enemy" (which is redefined and reused throughout Dungeoneer) to prevent compile errors
// Notice that this generic NPC is a flat character--its ability scores are all the same!
// An actual player would never create a character like this.(but its a good representation of a generic 1st level unclassed enemy)(and might be hilarious for a 1st-level wizard...)
NPC Enemy ("Enemy", 13, 13, 13, 13, 13, 13, 6, 8, 4, 12, true, -1, 1, "scimitar", "light crossbow", "dagger");

void NPC::dealDamage (int atkType, bool hasAdv, bool hasDis) { // Implement method of detecting the "side" of an NPC and therefore which NPC(or player) to damage when it attacks
	int atkRoll = toprand(20);
	int damage;
	if (hasAdv) {
		int atkRoll2 = toprand(20);
		if (atkRoll > atkRoll2) {}
		else
			atkRoll = atkRoll2;
	}
	if (hasDis) {
		int atkRoll2 = toprand(20);
		if (atkRoll < atkRoll2) {}
		else
			atkRoll = atkRoll2;
	}
	if (atkRoll >= 10 && isEnemy && !isDead && !isIncap) {
		damage = toprand(damDie[atkType]);
		damage = damage - AC; // Taking account for armor class
		damage = damage + getModifier(strg); // Adding strength modifier
		HP = HP - damage;
		switch (atkType) {
			case 1:
				tempstr = wepstr1;
				break;
			case 2:
				tempstr = wepstr2;
				break;
			case 3:
				tempstr = wepstr3;
				break;
		}
		cout << "Your enemy attacks with his " << tempstr << ", dealing " << damage << " damage to you.  You are now at " << HP << " HP." << endl;
	} else if (damage >= damDie[atkType] / 2 && !isEnemy && !isDead && !isIncap) {
		Enemy.takeDamage (damage);
	} else if (damage < damDie[atkType] / 2) 
		cout << "Your enemy swings at you with his " << tempstr << ", but the blow does not connect." << endl;
}
// Subtracts a specified number of hitpoints from the host NPC and incapacitates it/kills it depending upon the amount of damage taken
void NPC::takeDamage (int damage) {
	npcHP = npcHP - damage;
	if (npcHP <= 0) {
		if (abs(npcHP) >= maxHP) {// If the opposite of the NPC's hitpoint maximum is greater than the current number of hitpoints, kill the NPC
			isDead = true;
		}
		else {
		isIncap = true; // If not, render it unconcious
		//cout << "Kim Jong-Un attacks, dealing 9001 Communist Damage!" << endl;
		cout << "Your enemy faints from the pain of your attack and is now unconcious!";			
		}
	} // This system is not completely accurate to D&D, as in the game most DMs have enemies die when they drop to 0 HP.  Also, this stores HP as a negative value--something not done in D&D.
}
// This function is basically the script that handles the input you stick here: "NPC joe (here);" upon object creation
// Note (for beginners) that there is no type specification; this is required for constructors
NPC::NPC (string name, int str, int con, int dex, int inl, int cha, int wis, int dam1, int dam2, int dam3, int HP, bool enemy, int id, int LVL, string wep1, string wep2, string wep3) {
	// Handling input values
	Name = name;
	strg = str;
	cons = con;
	dext = dex;
	intl = inl;
	chas = cha;
	wisd = wis;
	damDie[1] = dam1;
	damDie[2] = dam2;
	damDie[3] = dam3;
	npcHP = HP;
	isEnemy = enemy;
	ID = id;
	lvl = LVL;
	// Setting other data
	maxHP = npcHP;
	isDead = false;
	isIncap = false;
	wepstr1 = wep1;
	wepstr2 = wep2;
	wepstr3 = wep3;
}
void attack (int atkType, bool hasAdv, bool hasDis) {
	inCombat = true;
	int atkRoll = toprand(20);
	if (hasAdv) {
		int atkRoll2 = toprand(20);
		if (atkRoll > atkRoll2) {}
		else
			atkRoll = atkRoll2;
	}
	if (hasDis) {
		int atkRoll2 = toprand(20);
		if (atkRoll < atkRoll2) {}
		else
			atkRoll = atkRoll2;
	}
	if (atkRoll >= 10) {
		int damage = rand() % 8;
		if (damage == 0)
			damage = 1;
		damage = damage + getModifier(ScoreAssoc[atkType]);
		Enemy.takeDamage (damage);
		cout << "You successfully damaged your target, dealing " << damage << " damage.  Your target is now at " << Enemy.npcHP << " HP. " << endl;
		if (Enemy.isDead == true) {
			cout << "You killed your enemy." << endl;
			NPC Enemy ("Enemy", 13, 13, 13, 13, 13, 13, 6, 8, 4, 12, true, -1, 1, "scimitar", "light crossbow", "dagger");
			inCombat = false;
			act ();
		} else {
			Enemy.dealDamage (toprand(3), false, false);
			act();
		}
	} else if (atkRoll == 1) {
		HP = HP - 1;
		cout << "You failed so horribly to damage your target that you pulled a muscle.  -1 HP. You are now at " << HP << " HP." << endl;
		Enemy.dealDamage (toprand(3), true, false);
		act();
	} else {
		cout << "You failed to damage your target. " << endl;
		Enemy.dealDamage (toprand(3), false, false);
		act();
	}
}
void move () {
	if (inCombat == false) {
		int temp = rand() % 5;
		if (init == 3) {
			cout << "You stay in your village, gathering a militia and building a palisade wall." << endl;
			act ();
		}
		else {
			switch (temp) {
				case 0:
					cout << "Trudging alone through seemingly impassable wilderness, you spend five days in transit to a dwarven hillocks." << endl;
					break;
				case 1:
					cout << "Riding fast over golden fields, you reach a human hamlet within two days." << endl;
					break;
				case 2:
					cout << "After three days' march through a damp swampland, you reach an elven forest retreat." << endl;
					break;
				case 3:
					cout << "You amble offroad through the unforgiving wilderness, stopping in a green vale after a day's travel." << endl;
					break;
				case 4:
					cout << "You stay at your current location after a bad omen persuades you to stall." << endl;
					break;
			}
		}
	} else
		cout << "You are in combat. ";
	act ();
}
void ACT () { // This function is here so act() can be called from within itself
	act ();
}
void act () {
	cout << "What would you like to do? ";
	string act;
	cin >> act;
	if (inCombat == false) {
		if (act == "attack" || act == "1") {
			// weapon chooser
			int wep;
			cout << "Which weapon will you use? Choices: ";
			if (cla == 1)
				cout << "longsword(1), dagger(2), light crossbow(3) "; // Fighter loadout
			if (cla == 2)
				cout << "short sword(1), crossbow(2), handaxe(3) "; // Rogue loadout
			if (cla == 3)
				cout << "magic missile(1), dagger(2), fireball(3) "; // Wizard loadout
			if (cla == 4)
				cout << "silvered shortsword(1), warhammer(2), light crossbow(3) "; // Cleric loadout
			cin >> wep;
			attack (wep, false, false);
		}
		else if (act == "move" || act == "2" || act == "travel") { // Travel
			move ();
		}
		else if (act == "longrest" || act == "3" || act == "long") { // Long rest
			if (inCombat = false) {
				HP = maxHP;
				cout << "You completed a long rest(8 hours), so you regained all lost hit points." << endl;
			} else
				cout << "You are in combat; you cannot rest now.";
			ACT ();
		}
		else if (act == "shortrest" || act == "4" || act == "short") { // Short rest
			if (inCombat = false) {
				int ranD = toprand(4); // Calculating a random number to add to your current HP
				HP = HP + ranD;
				if (HP > maxHP) // Making sure current HP after addition isn't higher than your maximum HP
					HP = maxHP;
				cout << "You completed a short rest(1 hour), so you regained " << ranD << " HP." << endl;
			} else
				cout << "You are in combat; you cannot rest now.";
			ACT ();
		}
		else if (act == "scan" || act == "look" || act == "6") {
			cout << "You look around and see... ";
			scan ();
			ACT ();
		}
		else {
			cout << "Unacceptable input.  Refer to the README for more information." << endl;
			ACT ();
		}
	} else {
		if (act == "attack" || act == "1") {
			// weapon chooser
			int wep;
			cout << "Which weapon will you use? Choices: ";
			if (cla == 1)
				cout << "longsword(1), dagger(2), light crossbow(3) "; // Fighter loadout
			if (cla == 2)
				cout << "short sword(1), crossbow(2), handaxe(3) "; // Rogue loadout
			if (cla == 3)
				cout << "magic missile(1), dagger(2), fireball(3) "; // Wizard loadout
			if (cla == 4)
				cout << "silvered shortsword(1), warhammer(2), light crossbow(3) "; // Cleric loadout
			cin >> wep;
			attack (wep, false, false);
		}
		else if (act == "dodge" || act == "2") {
			int temp = toprand(20);
			if (temp >= 10) {
				cout << "You dodged away from your enemy's blow, giving him a disadvantage." << endl;
				Enemy.dealDamage(toprand(3), false, true);
				ACT();
			}
		}
		else if (act == "disengage" || act == "3") {
			int ranD = rand() % 2;
			if (ranD == 1) {
				inCombat = false;
				cout << "You successfully escaped from your enemies." << endl;
				NPC Enemy ("Enemy", 13, 13, 13, 13, 13, 13, 6, 8, 4, 12, true, -1, 1, "scimitar", "light crossbow", "dagger");
				ACT ();
			} else {
				cout << "You could not escape.  Your movement cost you a turn, so your enemy attacks." << endl;
				Enemy.dealDamage (toprand(3), true, false);
				ACT ();
			}
		}
	}
}
void setClass () {
	switch (clas) {
		case 1:
			Str = 15;
			Dex = 14;
			Con = 14;
			Intl = 11;
			Wis = 12;
			Cha = 10;
			DamDie[1] = 8;
			DamDie[2] = 4;
			DamDie[3] = 8;
			HP = 12;
			maxHP = 12;
			cla = 1;
			AC = 3;
			cout << "You are a fighter: a seasoned warrior proficient in nearly all methods of combat.  You are equipped with a longsword, a dagger, and a light crossbow and quiver of 20 bolts.  Over your broad shoulders is slung an explorer's pack, filled with useful items such as a tinderbox, a canteen, and a 50-foot rope." << endl;
			ScoreAssoc[1] = Str;
			ScoreAssoc[2] = Str;
			ScoreAssoc[3] = Dex;
			break;
		case 2:
			Str = 14;
			Dex = 15;
			Con = 13;
			Intl = 11;
			Wis = 14;
			Cha = 8;
			DamDie[1] = 6;
			DamDie[2] = 10;
			DamDie[3] = 6;
			HP = 9;
			maxHP = 9;
			cla = 2;
			AC = 2;
			cout << "You are a rogue: a stealthy bandit skilled with the crossbow and wielding a shortsword.  You are equipped with a shortsword, a crossbow with 40 bolts, and a handaxe.  On your back you wear a thieve's pack, equipped with a crowbar, assorted knives, food, and a 50-foot rope." << endl;
			ScoreAssoc[1] = Str;
			ScoreAssoc[2] = Dex;
			ScoreAssoc[3] = Str;			
			break;
		case 3:
			Str = 10;
			Dex = 12;
			Con = 14;
			Intl = 15;
			Wis = 13;
			Cha = 8;
			DamDie[1] = 12;
			DamDie[2] = 4;
			DamDie[3] = 6;
			HP = 8;
			maxHP = 8;
			cla = 3;
			AC = 1;
			cout << "You are a wizard: a person shrouded in both mystery and the star-spangled cloak that covers your shoulders.  You can cast the magic missile and fireball spells.  You also carry a dagger.  On your emaciated shoulders lays the burden of a mage's sack, containing all the material elements needed to cast your spells." << endl;
			ScoreAssoc[1] = Intl;
			ScoreAssoc[2] = Str;
			ScoreAssoc[3] = Intl;			
			break;
		case 4:
			Str = 14;
			Dex = 12;
			Con = 12;
			Intl = 14;
			Wis = 10;
			Cha = 14;
			DamDie[1] = 9;
			DamDie[2] = 8;
			DamDie[3] = 8;
			HP = 8;
			maxHP = 8;
			cla = 4;
			AC = 2;
			cout << "You are a cleric: a priest of a deity who definitely does not scorn warfare.  You carry a silvered shortsword, a warhammer, and a light crossbow with 20 bolts.  On your blessed back you carry a priest's pack, filled with holy water and the tools of your solemn profession." << endl;
			ScoreAssoc[1] = Str;
			ScoreAssoc[2] = Str;
			ScoreAssoc[3] = Dex;			
			break;
	}
}
int main () {
	cout << "Welcome to Dungeoneer!" << endl;
	cout << "You are an adventurer embarking on a daring mission to... ";
	int init = rand() % 5;
	switch (init) {
		case 0:
			cout << "save a kidnapped girl!" << endl;
			break;
		case 1:
			cout << "recover lost treasure!" << endl;
			break;
		case 2:
			cout << "redeem yourself in society!" << endl;
			break;
		case 3:
			cout << "stop a heathen horde from destroying your village!" << endl;
			break;
		case 4:
			cout << "loot dungeons and overall be an idiot!" << endl;
			break;
	}
	cout << "Which class will your adventurer be?  Choices: fighter(1), rogue(2), wizard(3), cleric(4)" << endl;
	cin >> clas;
	setClass ();
	cout << "Looking around, you see... ";
	scan ();
	act ();
	return 0;
}
