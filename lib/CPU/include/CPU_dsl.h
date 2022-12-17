#define PRINT_ERR(...)                      \
    do                                      \
    {                                       \
        fprintf(stderr, "\e[0;31mERROR: "); \
        fprintf(stderr, __VA_ARGS__);       \
        fprintf(stderr, "\e[0m\n");         \
    } while (0)

#define FIRST_POP(cpuPtr)                                            \
    do                                                               \
    {                                                                \
        stackPop(&cpuPtr->stack, &first_popped);                     \
    } while (0)                                                      \

#define SECOND_POP(cpuPtr)                                            \
    do                                                                \
    {                                                                 \
        stackPop(&cpuPtr->stack, &second_popped);                     \
    } while (0)                                                       \

#define ARITHM(operation, cpuPtr)                                          \
    do                                                                     \
    {                                                                      \
        stackPush(&cpuPtr->stack, (second_popped operation first_popped)); \
                                                                           \
    } while (0)

#define UNAR(operation, cpuPtr)                              \
    do                                                       \
    {                                                        \
        stackPush(&cpuPtr->stack, operation(first_popped)); \
                                                             \
    } while (0)

#define BINAR(operation, cpuPtr)                                            \
    do                                                                      \
    {                                                                       \
        stackPush(&cpuPtr->stack, operation(second_popped, first_popped));  \
                                                                            \
    } while (0)

#define SINGLE_POP(cpuPtr, poppedPtr)                            \
    do                                                           \
    {                                                            \
        stackPop(&cpuPtr->stack, poppedPtr);                     \
    } while (0)

#define SINGLE_PUSH(cpuPtr, element)        \
    do                                      \
    {                                       \
        stackPush(&cpuPtr->stack, element); \
    } while (0)

#define OUT(cpuPtr, element)                             \
    do                                                   \
    {                                                    \
        fprintf(cpuPtr->log_file, "OUT: = %g", element); \
        printf("\e[0;32m%g\e[0m\n", element);   \
    } while (0)

#define ARITHM_DBG(operation)                                                          \
    do                                                                                 \
    {                                                                                  \
        fprintf(cpu->log_file, "%g %s %g\n", second_popped, #operation, first_popped); \
    } while (0)

#define UNAR_DBG(operation)                                             \
    do                                                                  \
    {                                                                   \
        fprintf(cpu->log_file, " %s(%g)\n", #operation, first_popped); \
    } while (0)

#define BINAR_DBG(operation)                                                              \
    do                                                                                    \
    {                                                                                     \
        fprintf(cpu->log_file, " %s(%g, %g)\n", #operation, second_popped, first_popped); \
    } while (0)

#define JUMP(cpuPtr)                                                                      \
    do                                                                                    \
    {                                                                                     \
        if (cpuPtr->code[cpuPtr->ip] < 0 || cpuPtr->code[cpuPtr->ip] >= cpuPtr->quantity) \
            PRINT_ERR("LABEL POINTS OUTDOORS ip = %d", cpuPtr->ip);                       \
        cpuPtr->ip = cpuPtr->code[cpuPtr->ip];                                            \
    } while (0)

#define COND_JUMP(cpuPtr, condition)                   \
    do                                                 \
    {                                                  \
        if (second_popped condition first_popped)      \
        {                                              \
            JUMP(cpuPtr);                              \
            /*PRINT_ERR("Condition succeded\n");  */   \
        }                                              \
        else                                           \
        {                                              \
            /*PRINT_ERR("Condition failed\n");      */ \
            cpuPtr->ip++;                              \
        }                                              \
    } while (0)

#define CALL(cpuPtr)                                                                      \
    do                                                                                    \
    {                                                                                     \
        if (cpuPtr->code[cpuPtr->ip] < 0 || cpuPtr->code[cpuPtr->ip] >= cpuPtr->quantity) \
        {                                                                                 \
            break;                                                                        \
        }                                                                                 \
        stackPush(&cpuPtr->funcs_stack, cpu->ip);                                         \
        cpuPtr->ip = cpuPtr->code[cpuPtr->ip];                                            \
        /*fprintf(stderr, "CALL to ip = %d\n", cpu->ip); */                               \
    } while (0)

// add assert
#define RET(cpuPtr)                                            \
    do                                                         \
    {                                                          \
        elem_t ret_position;                                   \
        stackPop(&cpuPtr->funcs_stack, &ret_position);         \
        /*fprintf(stderr, "RET to ip = %d\n", ret_position);*/ \
        cpu->ip = ++ret_position;                              \
                                                               \
    } while (0)

#define GRAPH(cpuPtr)                                                  \
    do                                                                 \
    {                                                                  \
        first_graph_index = cpuPtr->code[cpuPtr->ip++];                \
                                                                       \
        if (first_graph_index < 0 || first_graph_index > RAM_CAPACITY) \
        {                                                              \
            printf("GRAPH_ARG_OVERFLOW!\n");                           \
            return -1;                                                 \
        }                                                              \
        draw(cpuPtr, first_graph_index);                               \
    } while (0)

#define MAKE_CIRCLE(cpuPtr)                                                                            \
    do                                                                                                 \
    {                                                                                                  \
        int radius = cpuPtr->code[cpuPtr->ip++];                                                       \
        int center = first_graph_index + cpuPtr->win_size.width * ((cpuPtr->win_size.height) / 2 + 1); \
                                                                                                       \
        cpuPtr->RAM[center] = 1;                                                                       \
    } while (0)

#define DUMP()                                      \
    do                                              \
    {                                               \
        PRINT_ERR("ENABLE TO CALCULATE FACTORIAL"); \
                                                    \
    } while (0)