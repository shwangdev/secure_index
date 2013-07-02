//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

// introductory Boost.BloomFilter program
#include <boost/bloom_filter/basic_bloom_filter.hpp>
#include <sstream>
#include <string>
#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace boost::bloom_filters;
using namespace std;

const string gen_string(const size_t num)
{
    static stringstream stringer;
    string result;

    stringer << num;
    stringer >> result;
    stringer.clear();

    return result;
}

int main () {
    static const size_t INSERT_MAX = 5000;
    static const size_t CONTAINS_MAX = 10000;
    static const size_t NUM_BITS = 32768; // 8KB

    basic_bloom_filter<string, NUM_BITS> bloom;

    bloom.insert("hello");
    bloom.insert("fuck");
    if (! bloom.probably_contains("test"))
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;
  
    std::ostringstream  os;
    std::string content;
    boost::archive::text_oarchive oa(os);
    oa << bloom;
    content = os.str();
    std::istringstream is(content);
    boost::archive::text_iarchive ia(is);
    basic_bloom_filter<string, NUM_BITS> filter2 ;
    ia >> filter2;
    if ( bloom == filter2)
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;
    if ( filter2.probably_contains("fuck"))
    {
        std::cout<<"ok"<<std::endl;
    }
    else
        std::cout<<"fail"<<std::endl;
    
    return 0;
}
