
#include <iomanip>
#include <iostream>
#include "calc.h"
#include "menu.h"

int main() {
    cout << fixed << setprecision(1);
    loadAll();
    if (teamCount == 0 || bossCount == 0 || characterCount == 0) {
        cout << "Missing data. Run the program from the folder with the txt files.\n";
        return 1;
    }

    int choice = -1;
    while (choice != 0) {
        cout << "\nTeam DPS Simulator\n";
        cout << "Team: " << teams[selectedTeam].name << "\nBoss: " << bosses[selectedBoss].name << "\n\n";
        cout << "1 = select team\n2 = select boss\n3 = start sim\n4 = edit team\n0 = quit\nChoice: ";
        choice = readNumber();

        if (choice == 1) chooseTeam();
        else if (choice == 2) chooseBoss();
        else if (choice == 3) runSim();
        else if (choice == 4) editTeam();
        else if (choice != 0) cout << "Please type 0, 1, 2, 3 or 4.\n";
    }
    return 0;
}
