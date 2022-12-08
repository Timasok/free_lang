#ifndef TREE_DEBUG_H
#define TREE_DEBUG_H

#include "tree.h"

#define DEF_ERR(name, num) \
            MATAN_KILLER_ERROR_##name = 1 << num,

#define PARSE_ERROR(error_code, condition, error_specifier)                    \
                error_code |= ((condition) & error_specifier);

#define PRINT_ERROR(error_specifier)                                        \
        fprintf(log, "%s", #error_specifier);
        
int dumpExpNode(const Node * node);

int openLogs();
int closeLogs();
int getDumpNumber();

int findNext(const Node* node, int (*oper)(const Node *));
int makeDot(const Node * node);

int treeDump(const Node *node, const char * operation_info, const char *name_of_file, const char *name_of_func, int number_of_line);

#define TREE_DUMP(nodePtr)                                                  \
        treeDump(nodePtr, "", __FILE__, __PRETTY_FUNCTION__,__LINE__)

#define TREE_DUMP_OPTIONAL(nodePtr, operation_info)                                                  \
        treeDump(nodePtr, operation_info, __FILE__, __PRETTY_FUNCTION__,__LINE__)

enum MATAN_KILLER_ERRORS
{
        #include "errors.h"
};

#undef DEF_ERR
#endif