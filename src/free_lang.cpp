#include <stdio.h>

#include "free_lang_f.h"
#include "text_funcs.h"
#include "tree_debug.h"
#include "calc_f.h"

int main(int argc, const char* argv[])
{

    Text_info text1 = {};
    
    textCtor(&text1, "./timasok_sources/input.tim");

    printText(&text1);

    printf("Ride to hell!\n");

    openLogs();
    Exp_node * result = getGeneral(text1.buf);
    
    TREE_DUMP_OPTIONAL(result, "initial tree"); 
    printIn(result);
    printf("\n");
    closeLogs();

    nodeDtor(&result);
    textDtor(&text1);


    return 0;
}