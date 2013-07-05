#ifndef __FOLLOWME_DB_ADAPTER_HPP__
#define __FLOOLWME_DB_ADAPTER_HPP__
#include "db_conn_pool.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "db_conn_pool.hpp"
#include "config.hpp"
#include <vector>
#include <utility>
#include <map>

namespace secureindex{
    
    class T_DOC;
    
    class DBAdapter :public boost::noncopyable
    {
    public:

        DBAdapter( boost::shared_ptr<Config> config );
        
        std::vector<std::pair<std::string, std::string> >  get_all_remote_file_list();
        
        
        void add_document( std::string const & doc_id, 
                           std::string const & doc_name,
                           std::string const & index,
                           std::string const & encrypt_stream);
        
        void delete_document_by_id(std::string const & doc_id);
        
        void delete_document_by_name(std::string const & doc_name);

        void update_document_by_id( std::string const & old_doc_id,
                                    std::string const & new_doc_id,
                                    std::string const & new_doc_name,
                                    std::string const & new_doc_index,
                                    std::string const & stream);
        
        void update_document_by_name ( std::string const & old_doc_name,
                                       std::string const & new_doc_name,
                                       std::string const & new_doc_id,
                                       std::string const & new_doc_index,
                                       std::string const & stream);
        
        std::string get_document_index_by_id(std::string const & doc_id);
        
        std::string get_document_index_by_name(std::string const & doc_name);
        
        std::string get_document_by_name(std::string const & doc_name);
        
    private:
        
        boost::shared_ptr<MysqlConnPool> poolptr;
        boost::shared_ptr<Config> config_;
        
        bool is_exist_doc_id(std::string const & doc_id);
        
        bool is_exist_doc_name( std::string const & name);
        
    };
}
#endif
