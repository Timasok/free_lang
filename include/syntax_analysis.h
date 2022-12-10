#ifndef SYNTAX_F_H
#define SYNTAX_F_H

#include "text_funcs.h"
#include "lexical_analysis.h"

Node * getGeneral(const char * str);
Node * getExpression();
Node * getT();
Node * getDegree();
Node * getPolice();
Node * getVariable();
Node * getUnarOperation();
Node * getNumber();

#endif