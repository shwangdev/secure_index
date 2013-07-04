#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "document.hpp"
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "trie/hat_set.h"
#include "secure_index.hpp"

using namespace stx;

namespace secureindex
{
    Document::Document(const std::string & path):doc_path(path)
    {
        if ( ! boost::filesystem::exists(doc_path) || 
             ! boost::filesystem::is_regular_file(doc_path))
        {
            std::cout<<"File : "<< doc_path << " is not existed"<<std::endl;
            
        }
    }
    
    Document::Document(const Document & doc)
    {
        doc_path = doc.doc_path;

        if (! doc.words.empty())
        {
            this->words = doc.words;
        }
        
        if ( ! doc.unique_words.empty())
        {
            this->unique_words = doc.unique_words;
        }
    }

    std::string Document::get_document_id()
    {
        if ( boost::filesystem::exists(doc_path) &&
             boost::filesystem::is_regular_file(doc_path))
            {
                std::string result;
                 
                CryptoPP::SHA1 hash;
                 
                CryptoPP::FileSource(doc_path.c_str(),true, 
                                     new CryptoPP::HashFilter(hash,  
                                                              new CryptoPP::StringSink(result),true));
                 
                std::string digest = dimension256(result);
                return digest;
            }
        else
        {
            std::cerr<<"Invalid document: "<< doc_path<<std::endl;
            return std::string("");
        }
    }
    
    std::string Document::get_document_path()
    {
        return doc_path;
        
    }

    void Document::parse_doc()
    {
        if ( boost::filesystem::exists(doc_path) && 
             boost::filesystem::is_regular_file(doc_path))
            {
                std::string word;
                hat_trie_traits traits;
                traits.burst_threshold = 2;
                hat_set<std::string> h(traits);
                std::ifstream f(doc_path);

                while(f)
                {
                    f>>word;
                    words.push_back(word);

                    if(h.insert(word))
                        unique_words.push_back(word);
                }
                f.close();
            }
        
        else
            throw "Invalid document file";
    }
}
