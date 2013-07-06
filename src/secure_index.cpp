#include "document.hpp"
#include "secure_index.hpp"
#include <iostream>
#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/hmac.h>
#include <sstream>
#include <vector>
#include <map>

namespace secureindex
{
    std::string pesudo_function(const std::string & raw, const std::string & key)
    {
        std::vector<byte> buf(key.begin(),key.end());
        byte * m = new byte[buf.size()];
        for ( int i = 0 ; i < buf.size(); i ++)
            m[i] = buf[i];
        CryptoPP::HMAC<CryptoPP::SHA1> hmac(m, key.size());
        std::string mac;
        CryptoPP::StringSource(raw, true,
                               new CryptoPP::HashFilter(hmac, 
                                                        new CryptoPP::HexEncoder(
                                                            new CryptoPP::StringSink(mac) ,false)));
        delete [] m;
        return mac;
    }

    std::string dimension256(const std::string & raw)
    {
        std::string digest;
        
        CryptoPP::SHA256 hash;
        CryptoPP::StringSource ss(raw , true,
                                  new CryptoPP::HashFilter(hash,
                                                           new CryptoPP::HexEncoder (
                                                               new CryptoPP::StringSink(digest),false)));

        return digest;
    }
    
    Kpriv::Kpriv(const std::string & p, int rs):password(p), r(rs)
    {
        std::string s = dimension256(password);

        for (int i = 0 ; i < r ; ++i )
        {
            std::stringstream ss;
            ss.clear();
            ss << i;
            codes.push_back(pesudo_function(ss.str(),s));
        }
    }

    Trapdoor::Trapdoor(const Kpriv & k, const  std::string & w):key(k),word(w)
    {
        codes.clear();
        //std::string ec = dimension256(w);
        for( std::vector<std::string>::const_iterator it = k.codes.begin();
             it != k.codes.end(); it ++)
        {
            std::string s = pesudo_function( w , *it);
            codes.push_back(s);
        }
    }
    
    Trapdoor::Trapdoor(const Kpriv & k , const std::string & w, int i ) :key(k),word(w)
    {
        codes.clear();
        for( std::vector<std::string>::const_iterator it = k.codes.begin();
             it != k.codes.end(); it ++)
        {
            std::stringstream ss ;
            ss<<i;
            
            std::string count = ss.str();
            
            std::string s = pesudo_function( count + w , *it);
            codes.push_back(s);
        }

    }
    

    SecureIndex::SecureIndex(boost::shared_ptr<Document> d , const Kpriv & k):doc(d), key(k)
    {
        build_index();
        o_build_index();
    }
    
    Index SecureIndex::build_index()
    {
        doc->parse_doc();

        doc->index.doc_id = doc->get_document_id();

        for( std::list<std::string>::iterator it = doc->unique_words.begin();
             it != doc->unique_words.end(); it ++ )
        {
            //std::string ec = dimension256(*it);
            for ( std::vector<std::string>::const_iterator itx = key.codes.begin();
                  itx != key.codes.end(); itx++)
            {
                std::string x = pesudo_function( *it , *itx);
                std::string y = pesudo_function( doc->index.doc_id , x);
                
                doc->index.bf.insert(y);
            }            
        }

        return doc->index;
    }
    
    Index SecureIndex::o_build_index()
    {
        
        if ( doc->words.empty())
            doc->parse_doc();

        doc->oindex.doc_id = doc->get_document_id();
        
        std::map<std::string, int> word_count;
        
        for( std::list<std::string>::iterator it = doc->words.begin();
             it != doc->words.end(); it ++)
        {
            std::map<std::string , int> ::iterator idx = word_count.find(*it);
            if ( idx == word_count.end())
                word_count[*it] = 1;
            else
                word_count[*it] = word_count[*it] +1 ;
            
            for ( std::vector<std::string>::const_iterator itx = key.codes.begin();
                  itx != key.codes.end(); itx ++)
            {
                std::stringstream ss;
                ss << word_count[*it];
                
                std::string x = pesudo_function( ss.str() + *it, *itx);
                std::string y = pesudo_function( doc->oindex.doc_id , x);
                doc->oindex.bf.insert(y);
            }
            
        }
        
        return doc->oindex;
    }
    
    
    bool SecureIndex::search_index(const Trapdoor & t , const Index & i)
    {
        for( std::vector<std::string>::const_iterator it = t.codes.begin();
             it != t.codes.end(); it ++)
        {
            
            std::string y = pesudo_function(i.doc_id, *it);
            if (! i.bf.probably_contains(y))
                return false;
        }
        return true;
    }
}
