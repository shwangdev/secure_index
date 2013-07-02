#ifndef __SECURE_INDEX_HPP__
#define __SECURE_INDEX_HPP__

#include <string>
#include <vector>
#include <boost/bloom_filter/basic_bloom_filter.hpp>

namespace secureindex{

    typedef boost::bloom_filters::basic_bloom_filter<std::string, 32768> bloom_filter;

    std::string pesudo_function(const std::string & raw, const std::string & key);

    std::string dimension256(const std::string & raw);

    class Kpriv
    {
    public:
        /*
         *@param passcode is the password that you privided to secure index algorithm
         *@param r is the dimension of secure index
         */
        explicit Kpriv(const std::string & passcode , int r);

        /*Store r strings in a vec */
        std::vector<std::string> codes;
    private:
        std::string password;
        int r;
    };
    
    struct Index
    {
        std::string doc_id;
        bloom_filter bf;
    };
    
    struct Trapdoor
    {
        explicit Trapdoor( const Kpriv & k, const std::string & word);
        std::vector<std::string> codes;
    private:
        std::string word;
        Kpriv key;
    };
    
    class Document;

    class SecureIndex
    {
    public:
        
        explicit SecureIndex( Document & doc, const Kpriv &k);
                
        Index build_index();
        
        bool search_index(const Trapdoor & t, const Index & index);

    private:
        
        Document & doc;
        const Kpriv key;
        
    };
}

#endif
