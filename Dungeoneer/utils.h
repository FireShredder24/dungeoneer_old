#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int set1, set2, set3;

bool isDark, isWet, isSad, isHappy; // Booleans used to determine the overall "feel" of an area

void scan () { // Random sentence generator
	if (set1 == '\0') { // If set1 doesn't have a value yet, randomize.  If it does, don't change it until you move!
		set1 = rand() % 5;
		set2 = rand() % 5;
		set3 = rand() % 5;
	}
	
	switch (set1) {
		case 0:
			cout << "A lonely cliff stands surrounded by ";
			isSad = true;
			break;
		case 1:
			cout << "Salt flats bordered by ";
			isWet = false;
			break;
		case 2:
			cout << "Extensive mesas filled with ";
			isDark = true;
			isSad = false;
			break;
		case 3:
			cout << "A roaring river cuts through ";
			isWet = true;
			isHappy = true;
			break;
		case 4:
			cout << "A babbling brook winds through ";
			isWet = true;
			isHappy = true;
			break;
	}
	
	switch (set2) {
		case 0:
			cout << "dark forests and ";
			isDark = true;
			isSad = true;
			isHappy = false;
			break;
		case 1:
			cout << "fields of golden grass and ";
			isHappy = true;
			isDark = false;
			break;
		case 2:
			cout << "stark pillars of red rock and ";
			isDark = true;
			isHappy = false;
			isSad = false;
			break;
		case 3:
			cout << "meadows clogged with reeds and ";
			isHappy = false;
			isWet = true;
			break;
		case 4:
			cout << "damp swampland and ";
			isHappy = false;
			isWet = true;
			break;
	}
	
	switch (set3) {
		case 0:
			cout << "rocky ground." << endl;
			isHappy = false;
			break;
		case 1:
			cout << "stunted trees." << endl;
			isHappy = false;
			break;
		case 2:
			cout << "trickling streams." << endl;
			isWet = true;
			break;
		case 3:
			cout << "cattails." << endl;
			isWet = true;
			break;
		case 4:
			cout << "strong, tall trees." << endl;
			isHappy = true;
			isDark = true;
			break;
	}
}

int toprand (int max) {
	int temp = rand() % max;
	temp = temp + 1;
	return temp;
	delete &temp;
}

int getModifier (int abScore) { // Converting an ability score into its appropriate modifier(taken directly from the "Ability Scores & Modifiers" table in the Player's Basic Rules)
	switch (abScore) {
		case 1:
			return -5;
			break;
		case 2:
		case 3: // This acts like an "if (abScore == 2 || abScore == 3) {return -4;}" call
			return -4;
			break;
		case 4:
		case 5:
			return -3;
			break;
		case 6:
		case 7:
			return -2;
			break;
		case 8:
		case 9:
			return -1;
			break;
		case 10:
		case 11:
			return 0;
			break;
		case 12:
		case 13:
			return 1;
			break;
		case 14:
		case 15:
			return 2;
			break;
		case 16:
		case 17:
			return 3;
			break;
		case 18:
		case 19:
			return 4;
			break;
		case 20:
		case 21:
			return 5;
			break;
		case 22:
		case 23:
			return 6;
			break;
		case 24:
		case 25:
			return 7;
			break;
		case 26:
		case 27:
			return 8;
			break;
		case 28:
		case 29:
			return 9;
			break;
		case 30:
			return 10;
			break;
	}
}
