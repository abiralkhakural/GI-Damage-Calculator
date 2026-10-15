#ifndef CALC_H
#define CALC_H

#include "data.h"

struct Stats { double hp, atk, def, em, er, cr, cd, heal, dmg; };    // the final stats of a character

Stats calcStats(Character c);
double calcDamage(double motionValue, double count, Variable v, Character c, Boss boss);
void runSim();

#endif
