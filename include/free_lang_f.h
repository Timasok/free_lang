#ifndef FREE_LANG_H
#define FREE_LANG_H

#include <stdio.h>
#include "tree.h"

const int MAX_FILE_NAME_LENGTH = 128;
const int MAX_COMMAND_LENGTH = 128;

const int VAR_IN_FUNCCALL_INDEX = 128;//14336;
const int VAR_IN_DEF_INDEX      = 256;//10240;
const int SHIFT_IN_DEF          = 32;

enum def_change_mode
{
    INCREASE = 0,
    DECREASE = 1,
};

enum translation_result
{
    TRANSLATION_SUCCEEDEED                 = 0,
    TRANSLATION_TERMINATED_SYNTAX_ERROR    = 1,
    TRANSLATION_TERMINATED_OPEN_FILE_ERROR = 2,
    TRANSLATION_TERMINATED_SAVE_FILE_ERROR = 3,

};

int translateLanguage(const char *input_file_name, const char *output_file_name);
int handleProgram(Node *node);

int copyFromFunccallMemory(Node * func, Var variables[]);
int handleDefinition(Node *node);

int handleFunc(Node *node, Var variables[]);
int handleLangTree(Node *node, Var variables[]);

int handleASG(Node *node, Var variables[]);
int handleFunccall(Node * func, Var variables[]);

bool handleMacro(Node * func);

#endif