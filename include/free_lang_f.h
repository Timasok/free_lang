#ifndef FREE_LANG_H
#define FREE_LANG_H

#include <stdio.h>
#include "tree.h"

const int MAX_FILE_NAME_LENGTH = 128;
const int MAX_COMMAND_LENGTH = 128;

enum translation_result
{
    TRANSLATION_SUCCEEDEED                 = 0,
    TRANSLATION_TERMINATED_SYNTAX_ERROR    = 1,
    TRANSLATION_TERMINATED_OPEN_FILE_ERROR = 2,
    TRANSLATION_TERMINATED_SAVE_FILE_ERROR = 3,

};

int translateLanguage(const char *input_file_name, const char *output_file_name);
int handleProgram(Node *node, FILE *output_file);
int handleDefinition(Node *node, FILE *output_file);
int handleFunc(Node *node, Var variables[], FILE *output_file);
int handleLangTree(Node *node, Var variables[], FILE *output_file);

#endif