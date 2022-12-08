#ifndef CALC_F_H
#define CALC_F_H

#include "text_funcs.h"
#include "lexical_analys.h"

Node * getGeneral(const char * str);
Node * getExpression();
Node * getT();
Node * getDegree();
Node * getPolice();
Node * getVariable();
Node * getUnarOperation();
Node * getNumber();

#endif