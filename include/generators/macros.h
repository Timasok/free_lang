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