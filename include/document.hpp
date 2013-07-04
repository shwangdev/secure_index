#ifndef __DOCUMENT_HPP__
#define __DOCUMENT_HPP__

#include <string>
#include <list>
#include "secure_index.hpp"
namespace secureindex{
    
    class Document
    {

    public:
        friend class SecureIndex;

        explicit Document (const std::string & path );
        Document (const Document & other);
        std::string get_document_id();
        std::string get_document_path();

    private:

        void parse_doc();
        std::string doc_path;
        std::list<std::string> words;
        std::list<std::string> unique_words;
        Index index;
    };
}
#endif
