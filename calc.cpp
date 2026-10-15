#include "calc.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

struct Bonus { double hp, atk, def, hpPct, atkPct, defPct, em, er, cr, cd, heal, dmg; };

void addStat(Bonus& b, string name, double value) {
    if (name == "HP") b.hp += value;
    else if (name == "ATK") b.atk += value;
    else if (name == "DEF") b.def += value;
    else if (name == "HP%") b.hpPct += value;
    else if (name == "ATK%") b.atkPct += value;
    else if (name == "DEF%") b.defPct += value;
    else if (name == "EM") b.em += value;
    else if (name == "ER") b.er += value;
    else if (name == "CR") b.cr += value;
    else if (name == "CD") b.cd += value;
    else if (name == "HEAL") b.heal += value;
    else if (name == "DMG") b.dmg += value;
    else if (name != "NONE") cout << "Unknown stat name: " << name << "\n";
}

Stats calcStats(Character c) {
    Bonus b = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 3; i++) addStat(b, c.bonusName[i], c.bonusValue[i]);

    for (int slot = 0; slot < 5; slot++) {
        int a = findArtifact(slot, c.equipped[slot]);
        if (a < 0) continue;
        for (int i = 0; i < 5; i++) addStat(b, artifacts[slot][a].statName[i], artifacts[slot][a].statValue[i]);
    }

    for (int i = 0; i < buffCount; i++)
        if (buffs[i].team == c.team && (buffs[i].who == "ALL" || buffs[i].who == c.name))
            addStat(b, buffs[i].stat, buffs[i].value);

    Stats s;
    s.hp = c.baseHP * (1 + b.hpPct / 100) + b.hp;
    s.atk = (c.baseATK + c.weaponATK) * (1 + b.atkPct / 100) + b.atk;
    s.def = c.baseDEF * (1 + b.defPct / 100) + b.def;
    s.em = b.em;
    s.er = c.baseER + b.er;
    s.cr = c.baseCR + b.cr;
    s.cd = c.baseCD + b.cd;
    s.heal = b.heal;
    s.dmg = b.dmg;
    return s;
}

// Damage of one part of the rotation, for example (210% * 14)SkirkNA
double calcDamage(double motionValue, double count, Variable v, Character c, Boss boss) {
    Stats s = calcStats(c);

    double stat = (v.scaling == "HP") ? s.hp : s.atk;    // the stat the attack scales with
    double baseDamage = motionValue / 100 * count * stat;
    double dmgBonus = 1 + (s.dmg + v.extraDmg) / 100;

    double critRate = (s.cr > 100) ? 100 : s.cr;         // crit rate can't go above 100%
    double crit = 1 + critRate / 100 * s.cd / 100;

    double defense = (c.level + 100.0) / ((c.level + 100.0) + (boss.level + 100.0));

    double res = boss.resistance;                        // lowered by every RES shred buff of this element
    for (int i = 0; i < buffCount; i++)
        if (buffs[i].team == c.team && buffs[i].who == "BOSS" && buffs[i].stat == v.element)
            res -= buffs[i].value;
    double resistance = (res < 0) ? 1 - res / 200 : 1 - res / 100;   // negative resistance counts half

    return baseDamage * dmgBonus * crit * defense * resistance;
}

// Runs one rotation of the selected team on the selected boss and prints the results
void runSim() {
    Team team = teams[selectedTeam];
    Boss boss = bosses[selectedBoss];

    // the rotation looks like:   terms | seconds
    size_t bar = team.rotation.find('|');
    if (bar == string::npos) {
        cout << "The rotation in teams.txt must end with | and the rotation length in seconds.\n";
        return;
    }
    string terms = team.rotation.substr(0, bar);
    double seconds = atof(team.rotation.substr(bar + 1).c_str());

    cout << "\n===== " << team.name << " vs " << boss.name << " =====\n";
    cout << fixed << setprecision(0) << "Boss HP " << boss.hp << ", level " << boss.level
         << ", resistance " << boss.resistance << "%\n\n";

    double totalDamage = 0;
    double charDamage[MAX] = {0};

    // go through the rotation one part at a time (the parts are separated by +)
    size_t start = 0;
    while (start < terms.size()) {
        size_t plus = terms.find('+', start);
        if (plus == string::npos) plus = terms.size();
        string piece = trim(terms.substr(start, plus - start));
        start = plus + 1;

        // cut  (210% * 14)SkirkNA  into 210, 14 and SkirkNA
        size_t open = piece.find('('), percent = piece.find('%'), star = piece.find('*'), close = piece.find(')');
        if (!(open < percent && percent < star && star < close)) {   // also false if one was not found
            cout << "Can't read this part of the rotation: " << piece << "\n";
            continue;
        }
        double motionValue = atof(piece.substr(open + 1, percent - open - 1).c_str());
        double count = atof(piece.substr(star + 1, close - star - 1).c_str());

        int v = findVariable(trim(piece.substr(close + 1)));
        int c = (v < 0) ? -1 : findCharacter(variables[v].character);
        if (c < 0) {
            cout << "Unknown variable or character in: " << piece << "\n";
            continue;
        }

        double damage = calcDamage(motionValue, count, variables[v], characters[c], boss);
        totalDamage += damage;
        charDamage[c] += damage;
        cout << piece << "  =  " << damage << "\n";
    }

    if (seconds <= 0 || totalDamage <= 0) {
        cout << "Nothing to calculate in teams.txt.\n";
        return;
    }

    cout << "\nDamage by character:\n";
    for (int i = 0; i < characterCount; i++)
        if (characters[i].team == selectedTeam + 1)
            cout << "  " << characters[i].name << ": " << charDamage[i] << "\n";

    double dps = totalDamage / seconds;
    cout << "\nDamage per rotation: " << totalDamage << "\n";
    cout << "DPS: " << dps << "\n" << setprecision(1);
    cout << "Rotations needed: " << boss.hp / totalDamage << "\n";
    cout << "Estimated time to defeat the boss: " << boss.hp / dps << " s\n";
}
