#include "util.h"

void util_itoa(int len, char* str)
{
    if(str == NULL)
    {
        return;
    }
    sprintf(str, "%d", len);
}