#include <iostream>
#include <fstream>

#include "catalogue.h"

using namespace nmq;

Card mapFromRaw() {
    std::fstream fstm( "file.txt" );    
    Card result;

    fstm >> result.offset;
    
    return result;
}
