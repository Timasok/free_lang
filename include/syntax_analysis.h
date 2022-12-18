#ifndef SYNTAX_F_H
#define SYNTAX_F_H

#include "text_funcs.h"
#include "lexical_analysis.h"

Node * getGeneral       (Program_tokens * tokens);
Node * getExpression    ();
Node * getComparison    ();
Node * getTerm          ();
Node * getDegree        ();
Node * getPolice        ();
Node * getVariable      ();
Node * getUnarOperation ();
Node * getNumber        ();
Node * getAssignment    ();
Node * getKeyWord       ();
Node * getBlock         ();
Node * getFuncDefinition();
Node * getArgumentsNames();
Node * getFunc          ();
Node * getArguments     ();
Node * getIfElseBlocks  ();

int getLangTree(Node * predecessor);

#endif