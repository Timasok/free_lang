DEF_MACRO(PRINT, "print", 
{
    Node * arg = func->l_son;

    while (arg != nullptr)
    {
        pushArgInFuncall(arg, variables);

        fprintf(output_file, "out\n");       

        arg = arg->l_son;

    }
  
})

DEF_MACRO(PRINT_LINE, "print_line", 
{
    Node * arg = func->l_son;

    char * string  = (char *)calloc(MAX_LINE_LENGTH, sizeof(char));

    size_t str_length = 0;

    while (arg != nullptr && arg->type != NUM)
    {
        size_t tmp_len = 0;

        /* switch(arg->type)
        // {
        //     case NUM:
        //     {
        //         // STRING_DUMP(string);

        //         // char buffer [MAX_LINE_LENGTH] = {};
        //         // sprintf(buffer, "%g", arg->value.dbl_value);
                
        //         // strcat(string, buffer);
        //         // tmp_len = strlen(string + str_length);
        //         break;
        //     }
        //     case VAR:
        //     {
        //         strncat(string, arg->value.var.name, tmp_len);
        //         tmp_len = strlen(arg->value.var.name);
        //         break;
        //     }
        // };
        // STRING_DUMP(arg->value.var.name); */   

        strncat(string, arg->value.var.name, tmp_len);
        tmp_len = strlen(arg->value.var.name);

        printf("tmp_len = %lu\n", tmp_len);

        strcat(string, " ");

        str_length += tmp_len+1;
        arg = arg->l_son;

    }

    STRING_DUMP(string);

    for (int idx = str_length - 1; idx >= 0; idx--)
    {
        fprintf(output_file, "push %d\n", string[idx]);
    }

    for (int idx = 0; idx < str_length; idx++)
    {
        fprintf(output_file, "pop [%d]\n", idx);
    }

    fprintf(output_file, "show\n");       
    free(string);

})

DEF_MACRO(INPUT, "input", 
{
    Node * arg = func->l_son;

    while (arg != nullptr)
    {
        ASSERT(arg->type == VAR); // пока без исключений

        size_t arg_index_in_def = getVarIndex(variables, arg->value.var.name);

        fprintf(output_file, "in\n");

        fprintf(output_file, "pop [%s+%d]\n", def_reg, arg_index_in_def);
        
        arg = arg->l_son;

    }
  
})

// DEF_MACRO(RETURN, "return", 
// {
//     Node * arg = func->l_son;

// }