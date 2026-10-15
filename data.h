// data.h - the data loaded from the txt files, and the functions that load it

#ifndef DATA_H
#define DATA_H

#include <string>
using namespace std;

const int MAX = 50;                  // the most number of items of each kind that can be loaded

struct Team { string name, description, rotation; };
struct Boss { string name; double hp; int level; double resistance; };
struct Artifact { int id; string statName[5]; double statValue[5]; };
struct Variable { string name, character, scaling, element; double extraDmg; };
struct Buff { int team; string who, stat; double value; string label; };

struct Character {
    int team;
    string name;
    int level;
    double baseHP, baseATK, baseDEF, weaponATK, baseCR, baseCD, baseER;
    string bonusName[3];             // ascension stat, weapon stat, weapon passive
    double bonusValue[3];
    int equipped[5];                 // ID of the artifact worn in each slot (0 = nothing)
};

//lists are filled by loadAll(). "extern" means they are created in data.cpp.
extern Team teams[MAX];
extern Boss bosses[MAX];
extern Character characters[MAX];
extern Variable variables[MAX];
extern Buff buffs[MAX];
extern Artifact artifacts[5][MAX];   // artifacts[slot][number]
extern int teamCount, bossCount, characterCount, variableCount, buffCount, artifactCount[5];
extern const string slotNames[5];
extern int selectedTeam, selectedBoss;

void loadAll();
void saveEquipped();
string trim(string text);
int findCharacter(string name);      // the find functions return -1 if nothing is found
int findVariable(string name);
int findArtifact(int slot, int id);

#endif
