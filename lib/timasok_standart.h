#ifndef TIMASOK_CODING_HERO
#define TIMASOK_CODING_HERO

#define BASIC_PTR_ASSERT(pointer)                           \
        if (!pointer)                                       \
            return 0                                        \

bool equals(const double num_1, const double num_2);
bool equals(const int num_1, const int num_2);
bool stringEquals(const char *s_1, const char * s_2);

#endif