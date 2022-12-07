DEF_CMD(HLT, 0, 0, {}, 
{
    return 0;
})

DEF_CMD(PUSH, 1, 1, 
{
if (strchr(arg_string, '[') != nullptr && strchr(arg_string, ']') != nullptr)
    {
        isMemory = true;

        output->code[output->ip - 1] |= MEM_MASK;

        *strchr(arg_string, '[') = ' ';
        *strchr(arg_string, ']') = '\0';
    }

    fprintf(output->asm_log, "%s\n", arg_string);
    fprintf(output->asm_log, "%d\n", atoi(arg_string));

    if(sscanf(arg_string, " %d", &argument) == 1)
    {
        pushDmp(output->asm_log, argument, isRegister, isMemory);

        output->code[output->ip - 1] |= IMMED_MASK;

        output->code[output->ip++] = argument;
        break;

    } else {

        isRegister = true;
        char * reg_var;
        
        char * reg_immed_separator = strchr(arg_string, '+');

        if (reg_immed_separator != nullptr)
        {
            if (sscanf(reg_immed_separator + 1, " %d", &second_argument) == 1)
            {
                output->code[output->ip - 1] |= IMMED_MASK;
                *reg_immed_separator = '\0';
  
            } else
            {
                fprintf(output->asm_log, "GET HIGH POPING");
            }
        }

        sscanf(arg_string, "%ms", &reg_var);
        fprintf(output->asm_log, "%s\n", reg_var);

        GET_REG;

        pushDmp(output->asm_log, argument, isRegister, isMemory);

        output->code[output->ip - 1] |= REG_MASK;

        output->code[output->ip++] = argument;

        if (second_argument != 0)
        {
            output->code[output->ip++] = second_argument;
        }

        break;
    }

    fprintf(output->asm_log, "GET HIGH POPING");
} 
,
{ 
    
})

DEF_CMD(POP, 2, 1 ,
{
if (strchr(arg_string, '[') != nullptr && strchr(arg_string, ']') != nullptr)
    {
        isMemory = true;

#ifdef USING_INT
        output->code[output->ip - 1] |= MEM_MASK;
#elif defined USING_DOUBLE
        comand_name_transfer = output->code[output->ip - 1];
        output->code[output->ip - 1] = comand_name_transfer |= MEM_MASK;
#endif

        *strchr(arg_string, '[') = ' ';
        *strchr(arg_string, ']') = '\0';
    }

    fprintf(output->asm_log, "%s\n", arg_string);
    fprintf(output->asm_log, "%d\n", atoi(arg_string));

    if(sscanf(arg_string, " %d", &argument) == 1)
    {
        pushDmp(output->asm_log, argument, isRegister, isMemory);

#ifdef USING_INT
        output->code[output->ip - 1] |= IMMED_MASK;
#elif defined USING_DOUBLE
        comand_name_transfer = output->code[output->ip - 1];
        output->code[output->ip - 1] = comand_name_transfer |= IMMED_MASK;
#endif

        output->code[output->ip++] = argument;
        break;

    } else {

        isRegister = true;
        char * reg_var;
        
        char * reg_immed_separator = strchr(arg_string, '+');

        if (reg_immed_separator != nullptr)
        {
            if (sscanf(reg_immed_separator + 1, " %d", &second_argument) == 1)
            {
                output->code[output->ip - 1] |= IMMED_MASK;
                *reg_immed_separator = '\0';
  
            } else
            {
                fprintf(output->asm_log, "GET HIGH POPING");
            }
        }

        sscanf(arg_string, "%ms", &reg_var);
        fprintf(output->asm_log, "%s\n", reg_var);

        GET_REG;

        pushDmp(output->asm_log, argument, isRegister, isMemory);

        output->code[output->ip - 1] |= REG_MASK;

        output->code[output->ip++] = argument;

        if (second_argument != 0)
        {
            output->code[output->ip++] = second_argument;
        }

        break;
    }
   
    fprintf(output->asm_log, "GET HIGH PUSHING");
}
, 
{ 

})

DEF_CMD (ADD, 3, 0 , {}, 
{ 
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( + , cpu);
    ARITHM_DBG(+);
})

DEF_CMD (SUB, 4, 0 , {},  
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( - , cpu);
    ARITHM_DBG(-);
})

DEF_CMD (MUL, 5, 0 , {},  
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( * , cpu);
    ARITHM_DBG(*);
})

DEF_CMD (DIV, 6, 0 , {}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);                
    ARITHM( /, cpu);
    ARITHM_DBG(/);    
    
})

DEF_CMD(OUT, 7, 0 , {}, 
{
    elem_t tmp;
    SINGLE_POP(cpu, &tmp);
    tmp /= 1000;
    OUT(cpu, tmp);

})

DEF_CMD(DUP, 8, 0 ,  {}, 
{
    elem_t tmp;
    SINGLE_POP(cpu, &tmp);
    SINGLE_PUSH(cpu, tmp);
    SINGLE_PUSH(cpu, tmp);

})

DEF_CMD(RET, 9 , 0,  {}, 
{
    RET(cpu);
})

DEF_CMD(CALL, 10 , 1,  
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
         
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;

}, 
{ 
    CALL(cpu);
})

DEF_CMD(JMP, 11 , 1, 
{
    
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
         

        sscanf(strchr(arg_string, ':'), " :%s", label_name);        

        getFromLabels(output, arg_string, &argument);
 
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;


}, 
{
    JUMP(cpu);
})

DEF_CMD(JB, 12, 1,  
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
         
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
    }, 
{ 
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, <);

})

DEF_CMD(JBE, 13 , 1,  
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
         
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, <=);
})

DEF_CMD(JA, 14, 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, >);
})

DEF_CMD(JAE, 15 , 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, >=);
})

DEF_CMD(JE, 16, 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
         
          
        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
},
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, ==);
})

DEF_CMD(JNE, 17 , 1, 
{
    if (strchr(arg_string, ':') != nullptr)
    {
        output->compile_once *= 0;
          

        // assert((strchr(arg_string, ':') + 1) != NULL);
        sscanf(strchr(arg_string, ':'), " :%s", label_name);        
         
        getFromLabels(output, arg_string, &argument);
        //free(label_name);
    }
    else
    {
        sscanf(arg_string, " %d", &argument);
    }

    output->code[output->ip++] = argument;
}, 
{
    FIRST_POP(cpu);
    SECOND_POP(cpu);
    COND_JUMP(cpu, !=);
})

DEF_CMD(IN, 18, 0,  {}, 
{ 
    printf("\e[0;32mENTER VALUE: \e[0m");
    double tmp;
    scanf("%lf", &tmp);
    SINGLE_PUSH(cpu, (int)(tmp*1000));
})  

DEF_CMD(SHOW, 19, 0, {}, 
{
    
    for (int counter = 0; (cpu->RAM[counter] != 0) && (counter < RAM_CAPACITY); counter++)
    {
        printf("\e[0;31m%c\e[0m", cpu->RAM[counter]);
        cpu->RAM[counter] = 0;
        
    }
    printf("\n");

})

DEF_CMD(DMP, 20 , 0,  {}, 
{ 
    DUMP();
})

//write 
DEF_CMD(GRAPH, 21, 1, 
{
    sscanf(arg_string, " %d", &argument);
    output->code[output->ip++] = argument;

}, 
{
    GRAPH(cpu);
})

DEF_CMD(MAKE_CIRCLE, 22, 1, 
{
    sscanf(arg_string, " %d", &argument);
    output->code[output->ip++] = argument;

}, 
{
    MAKE_CIRCLE(cpu);
})