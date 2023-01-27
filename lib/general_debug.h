#ifndef GEN_DEBUG_H
#define GEN_DEBUG_H

// #ifdef DEBUG_MODE
// #define DBG_OUT printf("DBG at %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__)
// #endif
// #ifndef DEBUG_MODE
// #define DBG_OUT do{}while(0)
// #endif
#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n", \
                        __LINE__, __FILE__, __FUNCTION__)

#define PARSE_ERROR(cpuPtr, condition, error_specifier) \
    cpuPtr->code_of_error |= ((condition)&error_specifier);

#endif
#ifndef DEBUG_MODE
#endif


#define DBG_OUT printf("DBG at %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define PRINT_ERROR_CONSOLE(error_specifier)                                        \
        fprintf(stderr, "\e[0;31m%s\e[0m", #error_specifier)                        \
        
#define ASSERT(condition)                                                       \
    do{                                                                         \
        if (!condition)                                                         \
        {                                                                       \
            fprintf(stderr, "\e[0;31m%s failed at %s at %s(%d)\e[0m\n",         \
                     #condition, __PRETTY_FUNCTION__, __FILE__, __LINE__);      \
        }                                                                       \
    } while(0)                                                                  \

#define ASSERT_ASS(condition)                                           \
    do{                                                                 \
        if (!condition)                                                 \
        {                                                               \
            fprintf(stderr, "%s failed: file: %s func: %s line: %d\n",  \
                     #condition, __FILE__, __FUNCTION__, __LINE__);     \
            return -1;                                                  \
        }                                                               \
    } while(0)                                                          \


#define WHILE_VERIFY(condition)                                                                        \
    do{                                                                                                \
        switch (condition)                                                                             \
        {                                                                                              \
            case true:                                                                                 \
            {                                                                                          \
                printf("\e[0;32mWHILE(%s) in %s completed\e[0m\n", #condition, __PRETTY_FUNCTION__);   \
                break;                                                                                 \
            }                                                                                          \
            case false:                                                                                \
            {                                                                                          \
                printf("\e[0;31mWHILE(%s) in %s termination\e[0m\n", #condition, __PRETTY_FUNCTION__); \
                break;                                                                                 \
            }                                                                                          \
        }                                                                                              \
    } while(0)                                                                                         \

#define IF_VERIFY(condition)                                                                        \
    do{                                                                                             \
        switch (condition)                                                                          \
        {                                                                                           \
            case true:                                                                              \
            {                                                                                       \
                printf("\e[0;32mIF(%s) in %s completed\e[0m\n", #condition, __PRETTY_FUNCTION__);   \
                break;                                                                              \
            }                                                                                       \
            case false:                                                                             \
            {                                                                                       \
                printf("\e[0;31mIF(%s) in %s termination\e[0m\n", #condition, __PRETTY_FUNCTION__); \
                break;                                                                              \
            }                                                                                       \
        }                                                                                           \
    } while(0)                                                                                      \


#endif