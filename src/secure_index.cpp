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
        std::string ec = dimension256(w);
        for( std::vector<std::string>::const_iterator it = k.codes.begin();
             it != k.codes.end(); it ++)
        {
            std::string s = pesudo_function( ec , *it);
            codes.push_back(s);
        }
    }
    
    SecureIndex::SecureIndex(Document & d , const Kpriv & k):doc(d), key(k)
    {
        
    }
    
    Index SecureIndex::build_index()
    {
        doc.parse_doc();
        doc.index.doc_id = doc.get_document_id();

        for( std::list<std::string>::iterator it = doc.unique_words.begin();
             it != doc.unique_words.end(); it ++ )
        {
            std::string ec = dimension256(*it);
            for ( std::vector<std::string>::const_iterator itx = key.codes.begin();
                  itx != key.codes.end(); itx++)
            {
                std::string x = pesudo_function( ec, *itx);
                std::string y = pesudo_function( doc.index.doc_id , x);
                
                doc.index.bf.insert(y);
            }
            
        }

        return doc.index;
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
