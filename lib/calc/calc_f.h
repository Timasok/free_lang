#ifndef CALC_F_H
#define CALC_F_H

#include "text_funcs.h"
#include "tree.h"

const char * deleteSpaces(const char * str);
bool stringEquals(const char *s_1, const char * s_2);

Node * getGeneral(const char * str);
Node * getExpression();
Node * getT();
Node * getDegree();
Node * getPolice();
Node * getVariable();
Node * getUnarOperation();
Node * getNumber();

#endif