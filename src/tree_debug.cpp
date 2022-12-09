#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_debug.h"

const int MAX_OPER_LEN = 10;
const char HTML_LOG_PATH[] = "./logs/graph_log.html";

static FILE * HTM_LOG = 0;
static FILE * GRAPH_LOG = 0;

static int pic_number = 0;
static char dump_comand[128] = {};

static int nodeNumber = 0;
static int prevNumber = 0;

int dumpExpNode(const Node * node)
{
    if (!node)
        return -1;

    switch(node->type)
    {
        case ARITHM_OP:
            printf("\nOPERATION (%c)\n", node->value.op_value);
            break;
        case NUM:
            printf("\nNUMBER (%g)\n", node->value.dbl_value);
            break;
        case VAR:
            printf("\nVARIABLE (%s)\n", node->value.var.name);
            break;
        default:
            printf("\nБлэт не определился\n");
            break;

    }
    
    printf("\tparent %p\n", node->parent);
    printf("\texp_node %p\n", node);
    printf( "l_son %p, r_son %p\n", node->l_son, node->r_son);
    return 0;
}

int openLogs()
{
    HTM_LOG = fopen("./logs/log.htm", "w+");
    fflush(HTM_LOG);
    fprintf(HTM_LOG, "<pre>");

    return 0;

}

int closeLogs()
{
   fprintf(HTM_LOG, "\n</pre>");
   fclose(HTM_LOG);

   return 0;
}

int getDumpNumber()
{
    return pic_number - 1;
}

int findNext(const Node* node, int (*oper)(const Node *))
{
    if (!node)
        return 0;

    int currentNumber = ++nodeNumber;    
    oper(node);

    if (node->l_son)
    {
        prevNumber = nodeNumber;
        findNext(node->l_son, *oper);

    }

    if (node->r_son)
    {
        prevNumber = currentNumber;
        findNext(node->r_son, *oper);

    }

    return 0;

}

#define PRINT_DOT(...) fprintf(GRAPH_LOG, __VA_ARGS__)

static int oper(const Node * node)
{

    char data[MAX_BUFFER_LENGTH] = {};

    switch(node->type)
    {
        case ARITHM_OP:
            sprintf(data, "\nOPERATION (%c)\n", node->value.op_value);
            break;
        case NUM:
            sprintf(data, "\nNUMBER (%g)\n", node->value.dbl_value);
            break;
        case VAR:
            sprintf(data, "\nVARIABLE (%s)\n", node->value.var.name);
            break;
        default:
            sprintf(data, "\nБлэт не определился\n");
            break;

    }

    PRINT_DOT(  "node%d [\n" "label=<\n"
                "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n"
                "    <tr><td bgcolor=\"#dfdf91\" port = \"H%d\">NODE %d = %p PARENT = %p</td></tr>\n"
                "    <tr><td bgcolor= \"#5ca1cc\"><font color=\"black\"> %s </font></td></tr>\n"
                
                "    <tr>\n"
                "    <td>\n"
                
                "        <table border=\"0\" cellborder=\"1\">\n"
                
                "        <tr>\n"
                
                "            <td bgcolor = \"#70de9f\" port = \"P%d\">  LEFT = %p </td>\n"
                
                "            <td bgcolor = \"#e02828\" port = \"P%d\"> RIGHT = %p </td>\n"
                
                "        </tr> \n"
                
                "        </table> \n"
                
                "    </td>\n"
                
                "    </tr>\n" 
                
                "</table>>\n"
                "]\n\n", nodeNumber, nodeNumber, nodeNumber, node, node->parent, data, nodeNumber, node->l_son, nodeNumber, node->r_son);
    
    //There is no use yet to decide which port from the parent we should connect to his son GRAPHVIZ does it! 
    // const char output_port = 0;
    // if ()

    if (prevNumber != 0 && nodeNumber != 0)
        PRINT_DOT("node%d:P%d -> node%d:H%d;", prevNumber, prevNumber, nodeNumber, nodeNumber);

    
    return 0;
}

int makeDot(const Node *node)
{
    GRAPH_LOG = fopen(HTML_LOG_PATH, "w");
    
    PRINT_DOT("digraph MYG {\n");
    PRINT_DOT("rankdir = VR;\n");//or TB
    PRINT_DOT("graph [splines = ortho];\n");
    PRINT_DOT("bgcolor = \"white\";\n");

    PRINT_DOT("node [shape = \"plaintext\", style = \"solid\"];\n");
    
    findNext(node, *oper);

    PRINT_DOT("\n}");

    fclose(GRAPH_LOG);

    sprintf(dump_comand, "dot -Tjpeg %s > graph_dumps/dump_%d.jpeg", HTML_LOG_PATH, pic_number++);
    
    return 0;

}

int treeDump(const Node *node , const char * operation_info, const char *name_of_file, const char *name_of_func, int number_of_line)
{
    // graphDumpDot(list);
    nodeNumber = 0;
    makeDot(node);

    system(dump_comand);

    fprintf(HTM_LOG, "<h2>Dump No. %d\n", pic_number-1);
    fprintf(HTM_LOG, "\t\tDUMP CALLED at file: %s func: %s line: %d\n<h2>", name_of_file, name_of_func, number_of_line);
     
    if (strlen(operation_info) > 0)
    {
        fprintf(HTM_LOG, "\n\t\tOPERATION INFO %s<h2>", operation_info);   
    }

    fprintf(HTM_LOG, "\n<hr>\n<img src = ../graph_dumps/dump_%d.jpeg>\n", getDumpNumber());

    return 0;
}

#undef PRINT_DOT