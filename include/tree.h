#ifndef TREE_FUNCS_H
#define TREE_FUNCS_H

// #define DEBUG 47
#include "general_debug.h"
#include "timasok_standart.h"

const int MAX_BUFFER_LENGTH = 512;
const int NUMBER_OF_VARS = 16; 
const int MAX_FUNC_NUMBER = 20;

#define ASSERT(condition)                                               \
    do{                                                                 \
        if (!condition)                                                 \
        {                                                               \
            fprintf(stderr, "%s failed: file: %s func: %s line: %d\n",  \
                     #condition, __FILE__, __FUNCTION__, __LINE__);     \
        }                                                               \
    } while(0)                                                          \

enum son
{
    LEFT_SON   = 0,
    RIGHT_SON  = 1,
    HOLLOW = -1,
};

enum Node_type
{
    NUL = 0,
    OP  = 1,
    VAR = 2,
    NUM = 3,

};

#define DEF_OP(op_name, priority, op_code, name_in_lang)   \
    op_name = op_code,                                     \

enum Operator
{
    #include "operations.h"

};
  
#undef DEF_OP

typedef struct Var_t
{
    char *name;
    double value;
} Var;

union Value
{
    Operator op_value;
    double dbl_value;
    Var var;

};

struct Node
{

    Node * l_son = nullptr;
    Node * r_son = nullptr;
    Node * parent = nullptr;

    Node_type type = NUL;

    Value value;
    
};

int getPriority(const Node *node);

Node * nodeConnect(Node *parent, const char dest);
Node * nodeConnect(Node_type type, Value value, Node * l_son, Node * r_son);
Node * nodeCtor();

int getVarIndex(Var v_arr[], const char * name);
double getVarValue(Var v_arr[], const char * name);
int addValueToVarArray(Var v_arr[], double value, const char *name);
int addVarValueByIndex(Var v_arr[], double value, size_t index);
int addVarNameByIndex(Var v_arr[], char * name, size_t index);

int fillVarArray(Var v_arr[], Node * node, size_t free_index);
int dumpVarArray(Var v_arr[]);
int varArrayDtor(Var v_arr[]);

Node * createNode(Node_type type, Value value, Node * l_son, Node * r_son);
Node * createNum(double number);
Node * createVar(char var);
Node * createVar(char * var_name);
Node * createOp(Operator op);
Node * createOp(int operation);

int substitudeVariables(Node *node, Var v_arr[], const char * name_of_not_replaced_var);
int processVarNode(Node *node, Var v_arr[], const char * var_name);
int fillVarValues(Var v_arr[]);
bool checkExistence(Var v_arr[], const char * name);

#define CREATE_OP(op_name)          \
    createOp( ##op_name)

int linkToParent(Node *parent, Node *orphan);
int linkSonsToParent(Node *node);
int pickCubs(Node * prev_parent, Node * new_parent);
// int tearNode(Node * prev_parent, Node * new_parent);

bool isTerminal(Node *node);
bool hasSons(Node *node);

Node * copy(const Node * node);
int copyNodeData(const Node *src_node, Node *dest_node);
int copySingle(const Node * node, Node *new_node);

Node * findNode(Node *node, const char *string);//TODO change completely this function do we need her?
int nodeDtor(Node *node);
int nodeDtor(Node **node);

void printPre(const Node * node);
void printIn(const Node * node);
void printPost(const Node * node);

///////////////////////////////////////////////////////////////

int verifyTree(const Node *node);


#endif