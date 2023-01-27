DEF_INTRINSIC(PRINT, "print", 
{
    Node * arg = func->l_son;

    while (arg != nullptr)
    {
        pushArgInFuncall(arg, variables);

        fprintf(output_file, "out\n");       

        arg = arg->l_son;

    }
  
})

DEF_INTRINSIC(PRINT_LINE, "print_line", 
{
    Node * arg = func->l_son;

    char * string  = (char *)calloc(MAX_LINE_LENGTH, sizeof(char));

    // string = strdup("");
    size_t str_length = 0;

    while (arg != nullptr)
    {
        // STRING_DUMP(arg->value.var.name);

        size_t tmp_len = strlen(arg->value.var.name);
        strncat(string, arg->value.var.name, tmp_len);

        strcat(string, " ");

        str_length += tmp_len+1;
        arg = arg->l_son;

    }

    // STRING_DUMP(string);

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

DEF_INTRINSIC(INPUT, "input", 
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

DEF_INTRINSIC(RETURN, "return", 
{
    Node * arg = func->l_son;

    pushArgInFuncall(arg, variables);

    fprintf(output_file, "pop %s\n", ret_reg);

})