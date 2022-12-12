#ifndef SYNTAX_F_H
#define SYNTAX_F_H

#include "text_funcs.h"
#include "lexical_analysis.h"

Node * getGeneral       (Program_tokens * program_tokens);
Node * getExpression    (Program_tokens * program_tokens);
Node * getTerm          (Program_tokens * program_tokens);
Node * getDegree        (Program_tokens * program_tokens);
Node * getPolice        (Program_tokens * program_tokens);
Node * getVariable      (Program_tokens * program_tokens);
Node * getUnarOperation (Program_tokens * program_tokens);
Node * getNumber        (Program_tokens * program_tokens);
Node * getBlock         (Program_tokens * program_tokens);
Node * getAssignment    (Program_tokens * program_tokens);
int getLangTree(Program_tokens * program_tokens, Node * predecessor);

#endif