#pragma once
struct DebugInfo
{
    int line;
    const char *func;
};


#define qassert(condition, info)         \
{                                              \
                                             \
    if (condition == NULL) printf ("[func: %s, line: %d]\n", info.func, info.line);                               \
    if (condition == NULL) assert (condition);                                      \
}  

#define getDebugInfo {__LINE__, __FUNCSIG__}