#include "menu.h"
#include <cstdlib>
#include <iostream>
#include "calc.h"

int readNumber() {
    string line;
    if (!getline(cin, line)) return 0;                   //act like quit because input endde
    line = trim(line);
    if (line.size() == 0 || line[0] < '0' || line[0] > '9') return -1;
    return atoi(line.c_str());
}

// One artifact on one line: ID, [main stat], then the substats
void printArtifact(int slot, int index) {
    Artifact a = artifacts[slot][index];
    cout << "ID " << a.id << ": [" << a.statName[0] << " " << a.statValue[0] << "]";
    for (int i = 1; i < 5; i++) cout << " | " << a.statName[i] << " " << a.statValue[i];
    cout << "\n";
}

void chooseTeam() {
    for (int i = 0; i < teamCount; i++) cout << i + 1 << ") " << teams[i].name << "\n   " << teams[i].description << "\n";
    cout << "Choose a team (0 = back): ";
    int choice = readNumber();
    if (choice >= 1 && choice <= teamCount) selectedTeam = choice - 1;
}

void chooseBoss() {
    for (int i = 0; i < bossCount; i++) cout << i + 1 << ") " << bosses[i].name << "\n";
    cout << "Choose a boss (0 = back): ";
    int choice = readNumber();
    if (choice >= 1 && choice <= bossCount) selectedBoss = choice - 1;
}

// Shows one character's artifacts and lets the user change them
void editCharacter(int c) {
    while (true) {
        Stats s = calcStats(characters[c]);
        cout << "\n=== " << characters[c].name << " ===\n";
        for (int slot = 0; slot < 5; slot++) {
            cout << slot + 1 << ") " << slotNames[slot] << ": ";
            int a = findArtifact(slot, characters[c].equipped[slot]);
            if (a >= 0) printArtifact(slot, a);
            else cout << "(nothing equipped)\n";
        }
        cout << "\nHP " << s.hp << "   ATK " << s.atk << "   DEF " << s.def << "   EM " << s.em << "\n";
        cout << "CRIT Rate " << s.cr << "%   CRIT DMG " << s.cd << "%   ER " << s.er << "%\n";
        cout << "Elemental DMG Bonus " << s.dmg << "%   Healing Bonus " << s.heal << "%\n";

        cout << "\nChoose a slot 1-5 to change (0 = back): ";
        int slot = readNumber() - 1;                     // arrays start at 0
        if (slot == -1) return;
        if (slot < 0 || slot > 4) continue;

        cout << "\n--- " << slotNames[slot] << " inventory ---\n";
        for (int i = 0; i < artifactCount[slot]; i++) printArtifact(slot, i);
        cout << "Type the ID of the artifact to equip (0 = cancel): ";
        int id = readNumber();
        if (findArtifact(slot, id) < 0) {
            if (id != 0) cout << "There is no artifact with that ID in this slot.\n";
        } else {
            characters[c].equipped[slot] = id;
            saveEquipped();
        }
    }
}

void editTeam() {
    int members[MAX];                                    // positions of this team's characters
    int memberCount = 0;
    for (int i = 0; i < characterCount; i++) {
        if (characters[i].team == selectedTeam + 1) {
            members[memberCount] = i;
            memberCount++;
            cout << memberCount << ") " << characters[i].name << "\n";
        }
    }
    cout << "Choose a character (0 = back): ";
    int choice = readNumber();
    if (choice >= 1 && choice <= memberCount) editCharacter(members[choice - 1]);
}
