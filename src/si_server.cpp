#include <iostream>
#include "document.hpp"
#include "secure_index.hpp"
using namespace secureindex;

int main(int argc, char * argv[])
{
    Kpriv k ("1234",4);
    Document d(argv[1]);
    
    SecureIndex si(d,k);
    Index  i = si.build_index();
    
    Trapdoor t = Trapdoor(k,"This");
    if ( si.search_index(t, i))
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;
    
    return 0;
}
