#ifndef FREE_LANG_H
#define FREE_LANG_H

#include <stdio.h>
#include "tree.h"

const int MAX_FILE_NAME_LENGTH = 128;
const int MAX_COMMAND_LENGTH = 128;

const int START_ARG_INDEX     = 10240;
const int INTERNAL_FUNC_INDEX = 14336;

enum translation_result
{
    TRANSLATION_SUCCEEDEED                 = 0,
    TRANSLATION_TERMINATED_SYNTAX_ERROR    = 1,
    TRANSLATION_TERMINATED_OPEN_FILE_ERROR = 2,
    TRANSLATION_TERMINATED_SAVE_FILE_ERROR = 3,

};

int translateLanguage(const char *input_file_name, const char *output_file_name);
int handleProgram(Node *node);

int copyFromFunccallMemory(Node * func_name, Var variables[]);
int handleDefinition(Node *node);

int handleFunc(Node *node, Var variables[]);
int handleLangTree(Node *node, Var variables[]);

int handleASG(Node *node, Var variables[]);
int handleFunccall(Node * func_name, Var variables[]);

#endif