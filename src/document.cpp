/**
 * @file   document.cpp
 * @author  <devil@Funtoo>
 * @date   Mon Jul  8 09:40:13 2013
 * 
 * @brief  Document class implementation
 * 
 * 
 */

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "document.hpp"
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include<boost/tokenizer.hpp>
#include "trie/hat_set.h"
#include "secure_index.hpp"

using namespace stx;

namespace secureindex
{

/** 
 * 
 *
 * @param path 
 */
    Document::Document(const std::string & path):doc_path(path)
    {
        if ( ! boost::filesystem::exists(doc_path) || 
             ! boost::filesystem::is_regular_file(doc_path))
        {
            std::cout<<"File : "<< doc_path << " is not existed"<<std::endl;
            
        }
        
        doc_path = get_document_path();
        doc_name = get_document_name();
    }

    //constructor
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

    //calculate document id
    std::string Document::get_document_id()
    {
        //determine whether document path is valid
        if ( boost::filesystem::exists(doc_path) &&
             boost::filesystem::is_regular_file(doc_path))
        {
            std::string result;
                 
            CryptoPP::SHA1 hash;
                 
            //calculate file sha1 as Document id
            CryptoPP::FileSource(doc_path.c_str(),true, 
                                 new CryptoPP::HashFilter(hash,  
                                                          new CryptoPP::StringSink(result),true));
                 
            std::string digest = dimension256(result+ doc_path);
            return digest;
        }
        else
        {
            std::cerr<<"Invalid document: "<< doc_path<<std::endl;
            return std::string("");
        }
    }

    //get file path of document    
    std::string Document::get_document_path()
    {
        if ( boost::filesystem::exists(doc_path))
        {
            //return boost::filesystem::canonical(doc_path).string();
            return boost::filesystem::absolute(doc_path).string();
        }
        else
            throw "Invalid document path";
    }
    
    //get file name of document
    std::string Document::get_document_name()
    {
        if ( boost::filesystem::exists(doc_path))
            return boost::filesystem::basename(doc_path) + boost::filesystem::extension(doc_path);
        else
            std::cerr<<"Invalid document path"<<std::endl;
    }
    
    //read through the document, to store all words in document
    void Document::parse_doc()
    {
        if ( boost::filesystem::exists(doc_path) && 
             boost::filesystem::is_regular_file(doc_path))
        {
            //std::string word;
            //use tree structure to compress the document size
            hat_trie_traits traits;
            traits.burst_threshold = 2;
            hat_set<std::string> h(traits);
            std::ifstream f(doc_path);
            std::string content;
            while(std::getline(f, content))
            {
                boost::tokenizer<> tok(content);

                for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
                {
                    
                    //store all words
                    words.push_back(*beg);
                    
                    if(h.insert(*beg))
                    //store unique words
                        unique_words.push_back(*beg);
                }
                
            }
            f.close();
        }
        
        else
            throw "Invalid document file";
    }
}
