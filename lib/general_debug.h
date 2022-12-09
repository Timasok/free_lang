#ifndef GEN_DEBUG_H
#define GEN_DEBUG_H

#define DBG_OUT printf("DBG at %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define PRINT_ERROR_CONSOLE(error_specifier)                                        \
        fprintf(stderr, "%s", #error_specifier)                                     \
        
#define ASSERT(condition)                                               \
    do{                                                                 \
        if (!condition)                                                 \
        {                                                               \
            fprintf(stderr, "%s failed: file: %s func: %s line: %d\n",  \
                     #condition, __FILE__, __FUNCTION__, __LINE__);     \
        }                                                               \
    } while(0)                                                          \


#endif