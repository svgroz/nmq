#ifndef H_CATALOGUE
#define H_CATALOGUE

#include <stdint.h>

namespace nmq
{
    typedef struct Card
    {
        uint64_t offset;
        uint64_t size;
    } Card;

    Card mapFrom(char *source);
}

#endif