#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "tree.h"
#include "tree_debug.h"

int varDtor(Var var)
{
    if (!var.name)
        return 0;

    free(var.name);

    return 0;
}

Node * nodeConnect(Node *parent, const char dest)
{
    ASSERT((parent != nullptr));

#ifdef DEBUG
    printf("\e[0;31mconnect_called to %p dest %d\e[0m\n", parent, dest);
#endif

    Node * new_node = (Node *)calloc(1, sizeof(Node));
    
    switch(dest)
    {
        case LEFT_SON:
                parent->l_son = new_node;
                break;
        case RIGHT_SON:
                parent->r_son = new_node;
                break;
    }

    new_node->parent = parent;

    // dumpExpNode(new_node);
    return new_node;

}

Node * nodeCtor()
{
    return (Node *)calloc(1, sizeof(Node));
}

int copySingle(const Node * node, Node *new_node)
{
    if (!node)
        return -1;

    new_node->type = node->type;
    new_node->value = node->value;

    new_node->l_son = nullptr;
    new_node->r_son = nullptr;

#ifdef DEBUG
    dumpExpNode(new_node);
#endif

    return 0;

}

Node * copy(const Node * node)
{
    if (!node)
        return nullptr;
    
    Node *new_node = nodeCtor();

    copyNodeData(node, new_node);

#ifdef DEBUG
    (new_node);
#endif

    return new_node;

}

int copyNodeData(const Node *src_node, Node *dest_node)
{
    if (src_node == nullptr || dest_node == nullptr)
        return -1;


    dest_node->type = src_node->type;
    
    if (src_node->type == VAR)
    {
        dest_node->value.var.name = strdup(src_node->value.var.name);
        dest_node->value.var.value = src_node->value.var.value;
    }else
    {
        dest_node->value = src_node->value;

    }

    if (src_node->l_son)
    {
        nodeConnect(dest_node, LEFT_SON);
        copyNodeData(src_node->l_son, dest_node->l_son);
        dest_node->l_son->parent = dest_node;
    }

    if (src_node->r_son)
    {
        nodeConnect(dest_node, RIGHT_SON);
        copyNodeData(src_node->r_son, dest_node->r_son);
        dest_node->r_son->parent = dest_node;
    }

#ifdef DEBUG
    dumpExpNode(dest_node);
#endif

    return 0;

}

int fillVarArray(Var v_arr[], Node * node, size_t free_index)
{
    if (!node)
        return 0;

    if (node->type == VAR)
    {
        free_index = addVarNameByIndex(v_arr, node->value.var.name, free_index);
    }

    if (node->l_son)
    {
        fillVarArray(v_arr, node->l_son, free_index);        
    }

    if (node->r_son)
    {
        fillVarArray(v_arr, node->r_son, free_index);
    }

    return 0;
}

int getVarIndex(Var v_arr[], const char * name)
{
    int var_index = NUMBER_OF_VARS;

    for (int counter = 0; counter < NUMBER_OF_VARS; counter++)
    {
        var_index = counter;

        if(v_arr[counter].name == NULL)
        {
            v_arr[counter].name = strdup(name);
            break;

        }else if (stringEquals(name, v_arr[counter].name))
        {
            break;
        }

    }

    if (var_index == NUMBER_OF_VARS)
    {
        printf("TOO MANY VARIABLES! EXPAND THE SIZE OF ARR!\n");
        return -1;
    }

    return var_index;
}

bool checkExistence(Var v_arr[], const char * name)
{
    // STRING_DUMP(name);

    for (int counter = 0; counter < NUMBER_OF_VARS  && v_arr[counter].name != NULL; counter++)
    {
        if (stringEquals(name, v_arr[counter].name))
        {

            return true;
        }

    }

    return false;
}

double getVarValue(Var v_arr[], const char * name)
{
    double result = NAN;

    for (int counter = 0; counter < NUMBER_OF_VARS && v_arr[counter].name != NULL; counter++)
    {
        if (stringEquals(name, v_arr[counter].name))
        {
            result = v_arr[counter].value;
            break;
        }

    }

    return result;
}

int addVarValueByIndex(Var v_arr[], double value, size_t index)
{
    if (index >= NUMBER_OF_VARS || v_arr[index].name == NULL)
    {
        printf("Cannot find current index %ld!\n", index);
        DBG_OUT;
        return -1;

    }else if(v_arr[index].value != 0)
    {
        printf("WARNING! You're trying to overwrite the value of variable that already has one!\n");
        DBG_OUT;
    }

    v_arr[index].value = value;

    return 0;
}

int addVarNameByIndex(Var v_arr[], char * name, size_t index)
{
    int new_index = index;

    if (index >= NUMBER_OF_VARS)
    {
        printf("Cannot find current index %ld!\n", index);
        DBG_OUT;
        return -1;

    }else if(v_arr[index].name != NULL)
    {
        new_index = getVarIndex(v_arr, name) + 1;

    }else
    {
        v_arr[index].name = strdup(name);
        new_index = index + 1;
    }

    return new_index;
}

int addValueToVarArray(Var v_arr[], double value, const char *name)
{
    int var_index = getVarIndex(v_arr, name);

    return addVarValueByIndex(v_arr, value, var_index);
    
}

int varArrayDtor(Var v_arr[])
{
    for (int counter = 0; counter < NUMBER_OF_VARS && v_arr[counter].name != NULL; counter++)
    {
        v_arr[counter].value = 0;
        free(v_arr[counter].name);
    }

    return 0;
}

int dumpVarArray(Var v_arr[])
{
    for (int counter = 0; counter < NUMBER_OF_VARS; counter++)
    {
        printf("VAR[%d] = {%s, %g}\n", counter, v_arr[counter].name, v_arr[counter].value);
    }
    return 0;
}

int processVarNode(Node *node, Var v_arr[], const char * var_name)
{
    double possible_replace = getVarValue(v_arr, var_name);

    // printf("%g\n", possible_replace);

    if (possible_replace != NAN)
    {
        node->type = NUM;
        free(node->value.var.name);
        node->value.dbl_value = possible_replace;
    }

    return 0;
}

int substitudeVariables(Node *node, Var v_arr[], const char * name_of_not_replaced_var)
{
    if (!node)
        return 0;

    if (node->type == VAR && (name_of_not_replaced_var == NULL || !stringEquals(name_of_not_replaced_var, node->value.var.name)))
    {
        processVarNode(node, v_arr, node->value.var.name);
    }

    substitudeVariables(node->l_son, v_arr, name_of_not_replaced_var);

    substitudeVariables(node->r_son, v_arr, name_of_not_replaced_var);

    return 0;
}

int fillVarValues(Var v_arr[])
{
    printf("Enter values: \n");

    for (int counter = 0; counter < NUMBER_OF_VARS && v_arr[counter].name != NULL; counter++)
    {   
        // fscanf(stdin, "%*[\n]" );

        printf("%s: =  ", v_arr[counter].name);

        while (scanf(" %lf", &v_arr[counter].value) != 1)
        {
                scanf( "%*[^\n]" );
                printf("%s: =  ", v_arr[counter].name);
        }

    }

    // dumpVarArray(v_arr);

    return 0;
}

#define DEF_OP(op_name, priority, op_code, name_in_lang)               \
        else if(node->value.op_value == op_name)                                 \
        {                                                                        \
            return priority;                                                     \
        }                                                                        \

int getPriority(const Node *node)
{

    if (0)
    {}
        #include "operations.h"

    return 0;
}

#undef DEF_OP

Node * createNode(Node_type type, Value value, Node * l_son, Node * r_son)
{
    int error_code = 0;
    PARSE_ERROR(error_code, type == NUL, LANG_ERROR_INVALID_TYPE);
    // PARSE_ERROR(error_code, !value, MATAN_KILLER_ERROR_INVALID_VALUE);

    if (error_code != 0)
        return nullptr;

    Node * new_node = nodeCtor(); 

    new_node->type = type;
    new_node->value = value;
    new_node->l_son = l_son;
    new_node->r_son = r_son;

    // dumpExpNode(new_node);
    return new_node;

}

Node * createEmpty()
{
    Node * new_node = nodeCtor(); 
    new_node->type = EMPTY;

    return new_node;
}

Node * nodeConnect(Node_type type, Value value, Node * l_son, Node * r_son)
{
    Node * result = createNode(type, value, l_son, r_son);
    linkToParent(result, l_son);
    linkToParent(result, r_son);

    return result;

}

Node * createNum(double number)
{
    Value val = {};
    val.dbl_value = number;
    return createNode(NUM, val, nullptr, nullptr);
}

Node * createVar(char var)
{
    Value val = {};
    val.var.value = var;
    return createNode(VAR, val, nullptr, nullptr);
}

Node * createVar(char * var_name)
{
    Value val = {};
    val.var.name = strdup(var_name);
    return createNode(VAR, val, nullptr, nullptr);
}

Node * createOp(Operator op)
{
    Value val = {};
    val.op_value = op;
    return createNode(OP, val, nullptr, nullptr);
}

#define DEF_OP(op_name, priority, op_code, name_in_lang)    \
            else if(operation == op_code)                   \
            {                                               \
                val.op_value = op_name;                     \
            }                                               \

Node * createOp(int operation)
{
    Value val = {};
    val.op_value = NOT_OP;

    if(0)
    { }
    #include "operations.h"

    return createNode(OP, val, nullptr, nullptr);
}

#undef DEF_OP

bool isTerminal(Node *node)
{
    if (!node)
        return false;

    if (!node->l_son && !node->r_son)
    {
        return true;

    }else {
        
        return false;
    }

}

bool hasSons(Node *node)
{
    if (node->l_son && node->r_son)
    {
        return true;

    } else {

        return false;
    }
}

int linkToParent(Node *parent, Node *orphan)
{
    if (parent == nullptr || orphan == nullptr)
        return -1;

    orphan->parent = parent;

    return 0;

}

int linkSonsToParent(Node *node)
{
    if (node->l_son)
        node->l_son->parent = node;

    if (node->r_son)
        node->r_son->parent = node;

    return 0;
}

int linkSonsToParent(Node *node, Node *left_son, Node *right_son)
{
    if (!node)
        return -1;

    if (left_son)    
    {
        node->l_son = left_son;
        node->l_son->parent = node;
    }

    if (right_son)    
    {
        node->r_son = right_son;
        node->r_son->parent = node;
    }

    return 0;
}

int pickCubs(Node * prev_parent, Node * new_parent)
{
    if (!hasSons(prev_parent))
        return -1;

    new_parent->l_son = prev_parent->l_son;
    new_parent->r_son = prev_parent->r_son;

    linkToParent(new_parent, prev_parent->l_son);
    linkToParent(new_parent, prev_parent->r_son);

    prev_parent->l_son = nullptr;
    prev_parent->r_son = nullptr;
    
    return 0;
}

void printIn(const Node * node)
{
    if (!node)
        return;        

    printf("(");

    if (node->l_son)
    {
        printIn(node->l_son);

    }

    switch(node->type)
    {
        case OP:
            printf("%c", node->value.op_value);
            break;
        case KEY_WORD:
            printf("%c", node->value.key_value);
            break;
        case NUM:
            printf("%g", node->value.dbl_value);
            break;
        case VAR:
            printf("\"%s\"", node->value.var.name);
            break;
        case EMPTY:
            printf("EMPTY");
            break;
        default:
            printf("smth cringe\n");
            break;

    }
 
    if (node->r_son)
    {
        printIn(node->r_son);

    }

        printf(")");
    return;
}

int nodeDtor(Node *node)
{
    if (!node)
        return 0;

    while(1)
    {
    #ifdef DEBUG

    if (node->parent == nullptr)
    {
        printf("%s called for tree\n", __PRETTY_FUNCTION__);
    }
        printf("\n\e[0;32mдолжны были зафришеть - \e[0m");
        dumpExpNode(node);
    #endif

        if (node->l_son == nullptr && node->r_son == nullptr)
        {
            if (node->parent == nullptr)
                return 0;

            if (node->parent->l_son == node)
            {
                node->parent->l_son = nullptr;
                    
            }
            else if(node->parent->r_son == node)
            {
                node->parent->r_son = nullptr;
            }

        #ifdef DEBUG
            printf("\n\e[0;31mзафришен - \e[0m");
            dumpExpNode(node);
        #endif
            if (node->type == VAR || node->type == FUNC)
            {
                if (node->value.var.name != NULL)
                {
                    free(node->value.var.name);
                    node->value.var.name = NULL;
                }
            }

            free(node);
            return 0;

        }

        nodeDtor(node->l_son);

        nodeDtor(node->r_son);
    }

    
    
    return 0;
}

int nodeDtor(Node **node)
{
    nodeDtor(*node);
    free(*node);

    return 0;
}