#include "data.h"
#include <fstream>

Team teams[MAX];
Boss bosses[MAX];
Character characters[MAX];
Variable variables[MAX];
Buff buffs[MAX];
Artifact artifacts[5][MAX];
int teamCount = 0, bossCount = 0, characterCount = 0, variableCount = 0, buffCount = 0;
int artifactCount[5] = {0, 0, 0, 0, 0};
const string slotNames[5] = {"Flower", "Feather", "Sands", "Goblet", "Circlet"};
const string slotFiles[5] = {"flower.txt", "feather.txt", "sands.txt", "goblet.txt", "circlet.txt"};
int selectedTeam = 0, selectedBoss = 0;

// Removes spaces from both ends of a string
string trim(string text) {
    while (text.size() > 0 && (text[0] == ' ' || text[0] == '\r')) text.erase(0, 1);
    while (text.size() > 0 && (text[text.size() - 1] == ' ' || text[text.size() - 1] == '\r'))
        text.erase(text.size() - 1, 1);
    return text;
}

int findCharacter(string name) {
    for (int i = 0; i < characterCount; i++)
        if (characters[i].name == name) return i;
    return -1;
}

int findVariable(string name) {
    for (int i = 0; i < variableCount; i++)
        if (variables[i].name == name) return i;
    return -1;
}

int findArtifact(int slot, int id) {
    for (int i = 0; i < artifactCount[slot]; i++)
        if (artifacts[slot][i].id == id) return i;
    return -1;
}

void loadTeams() {
    ifstream file("teams.txt");
    string line;
    while (teamCount < MAX && getline(file, line)) {
        if (trim(line) == "") continue;                  // skip the blank lines between teams
        teams[teamCount].name = trim(line);
        getline(file, teams[teamCount].description);
        getline(file, teams[teamCount].rotation);
        teamCount++;
    }
}

void loadBosses() {
    ifstream file("bosses.txt");
    string line, word1, word2, word3;                    // the words HP, Level and Resistance
    while (bossCount < MAX && getline(file, line)) {
        if (trim(line) == "") continue;
        bosses[bossCount].name = trim(line);
        file >> word1 >> bosses[bossCount].hp >> word2 >> bosses[bossCount].level >> word3
             >> bosses[bossCount].resistance;
        file.ignore(1000, '\n');                         // go to the next line
        bossCount++;
    }
}

void loadCharacters() {
    ifstream file("characters.txt");
    file.ignore(1000, '\n');
    Character c;
    while (characterCount < MAX && file >> c.team >> c.name >> c.level >> c.baseHP >> c.baseATK >>
           c.baseDEF >> c.weaponATK >> c.baseCR >> c.baseCD >> c.baseER) {
        for (int i = 0; i < 3; i++) file >> c.bonusName[i] >> c.bonusValue[i];
        for (int i = 0; i < 5; i++) c.equipped[i] = 0;
        characters[characterCount] = c;
        characterCount++;
    }
}

void loadVariables() {
    ifstream file("variables.txt");
    file.ignore(1000, '\n');
    Variable v;
    while (variableCount < MAX && file >> v.name >> v.character >> v.scaling >> v.element >> v.extraDmg) {
        variables[variableCount] = v;
        variableCount++;
    }
}

void loadBuffs() {
    ifstream file("buffs.txt");
    file.ignore(1000, '\n');
    Buff b;
    while (buffCount < MAX && file >> b.team >> b.who >> b.stat >> b.value >> b.label) {
        buffs[buffCount] = b;
        buffCount++;
    }
}

void loadArtifacts() {
    for (int slot = 0; slot < 5; slot++) {
        ifstream file(slotFiles[slot]);
        file.ignore(1000, '\n');
        Artifact a;
        while (artifactCount[slot] < MAX && file >> a.id) {
            for (int i = 0; i < 5; i++) file >> a.statName[i] >> a.statValue[i];
            artifacts[slot][artifactCount[slot]] = a;
            artifactCount[slot]++;
        }
    }
}

void loadEquipped() {
    ifstream file("equipped.txt");
    string name;
    while (file >> name) {
        int c = findCharacter(name);
        for (int slot = 0; slot < 5; slot++) {
            int id;
            file >> id;
            if (c >= 0) characters[c].equipped[slot] = id;
        }
    }
}

void loadAll() {
    loadTeams();
    loadBosses();
    loadCharacters();
    loadVariables();
    loadBuffs();
    loadArtifacts();
    loadEquipped();
}

void saveEquipped() {
    ofstream file("equipped.txt");
    for (int i = 0; i < characterCount; i++) {
        file << characters[i].name;
        for (int slot = 0; slot < 5; slot++) file << " " << characters[i].equipped[slot];
        file << "\n";
    }
}
