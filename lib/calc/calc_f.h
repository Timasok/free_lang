#ifndef CALC_F_H
#define CALC_F_H

#include "text_funcs.h"
#include "tree.h"

const char * deleteSpaces(const char * str);
bool stringEquals(const char *s_1, const char * s_2);

Exp_node * getGeneral(const char * str);
Exp_node * getExpression();
Exp_node * getT();
Exp_node * getDegree();
Exp_node * getPolice();
Exp_node * getVariable();
Exp_node * getUnarOperation();
Exp_node * getNumber();

#endif