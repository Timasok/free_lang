#include <string.h>
#include <stdlib.h>

#include "inout_ASM.h"
#include "debug_ASM.h"

#define ASSERT_OK(asmPtr)                                                   \
   do{                                                                      \
      if (returnAsmError(asmPtr) != 0)                                      \
       {                                                                    \
           dumpAsm(asmPtr, __FILE__, __FUNCTION__, __LINE__);               \
       }                                                                    \
   } while (0)

int asmCtor(Asm_info *executable, char * asm_name, int maximum_quantity)
{
    DBG_OUT;
    ASSERT_OK(executable);
    DBG_OUT;
    executable->ip = 0;
    executable->number_of_comands = 0;
    executable->asm_file = fopen(asm_name, "wb");

    // memset(executable->labels.labels, LABEL_POISON, 2 * LABELS_SIZE * sizeof(Label));

    for (int counter = 0; counter < LABELS_SIZE; counter++)
    {
        executable->labels[counter].address = LABEL_POISON;
        executable->labels[counter].name = "imp";
    }

#ifdef USING_INT
    executable->code = (int *)calloc(maximum_quantity, sizeof(int));
#elif defined USING_DOUBLE
    executable->code = (double *)calloc(maximum_quantity, sizeof(double));
#endif

#ifdef USING_INT
#elif defined USING_DOUBLE
#endif

    ASSERT_OK(executable);
    return EXIT_SUCCESS;
}

int writeAssemblerInFile(Asm_info * executable, FILE * txt_reserve)
{
    ASSERT_OK(executable);
    
    //КОСТЫЛЬ С СИГНАТУРОЙ    
    if(strcasecmp(executable->signature, "CP") == 0)
    {
        // fwrite (executable->signature, sizeof(int), 1, executable->asm_file);
        // fwrite (&executable->ip, sizeof(int), 1, executable->asm_file);
        // fwrite (&executable->number_of_comands, sizeof(int), 1, executable->asm_file);
        // fwrite (&executable->code, sizeof(int), executable->ip, executable->asm_file);
        
    int * buffer = (int *)calloc(executable->ip + 3, sizeof(int));

    buffer[0] = 'C' + 'P'*256;
    buffer[1] = executable->ip;
    buffer[2] = executable->number_of_comands;
    for (int counter = 0; counter < executable->ip; counter++)
        buffer[counter + 3] = executable->code[counter];

    fwrite (buffer, sizeof(int), executable->ip + 3, executable->asm_file);

    free(buffer);

    for (int counter = 0; counter < executable->ip; counter++)
        fprintf(txt_reserve, "%d\t", executable->code[counter]);

        return EXIT_SUCCESS;

    } else {

        ASSERT_OK(executable);
        return ASM_ERROR_INCORRECT_SIGNATURE;

    }
}

int asmDtor(Asm_info *executable)
{
    ASSERT_OK(executable);
    free(executable->code);
    
    fclose(executable->asm_file);
    fclose(executable->asm_log);

    return EXIT_SUCCESS;

}

#undef ASSERT_OK