#ifndef GEN_DEBUG_H
#define GEN_DEBUG_H
        
#define DBG_OUT printf("file: %s func: %s line: %d\n", __FILE__, __FUNCTION__, __LINE__)

#define ASSERT(condition)                                               \
    do{                                                                 \
        if (!condition)                                                 \
        {                                                               \
            fprintf(stderr, "%s failed: file: %s func: %s line: %d\n",  \
                     #condition, __FILE__, __FUNCTION__, __LINE__);     \
        }                                                               \
    } while(0)                                                          \


#endif