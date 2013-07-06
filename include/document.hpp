#ifndef __DOCUMENT_HPP__
#define __DOCUMENT_HPP__

#include <string>
#include <list>
#include "secure_index.hpp"

namespace secureindex{
    class SecureIndexService;
    
    class Document
    {

    public:
        friend class SecureIndex;
        friend class SecureIndexService;
        
        explicit Document (const std::string & path );
        Document (const Document & other);
        std::string get_document_id();
        std::string get_document_path();
        std::string get_document_name();

    private:

        void parse_doc();
        std::string doc_path;
        std::string doc_name;
        std::list<std::string> words;
        std::list<std::string> unique_words;
        Index index;
        Index oindex;
    };
}
#endif
