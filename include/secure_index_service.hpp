#ifndef __SECURE_INDEX_SERVICE_HPP__
#define __SECURE_DINEX_SERVICE_HPP__
#include "document.hpp"
#include "secure_index.hpp"
#include "db_adapter.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include "config.hpp"
#include <iostream>

namespace secureindex{

    class SecureIndexService: boost::noncopyable
    {
    public:
        
        SecureIndexService(boost::shared_ptr<Config> config);

        std::vector<std::pair<std::string, std::string> > get_uploaded_file_list();
        
        void upload_file( const boost::filesystem::path & file_name, 
                          std::string const & password);

        void upload_folder( const boost::filesystem::path & local_folder,
                            std::string const & password);
        
        void delete_file_by_id( std::string const & doc_id);
        
        void delete_file_by_name(std::string const & doc_name);
        
        void download_file_by_id ( const std::string & doc_id, 
                                   const  boost::filesystem::path & dist_file);
        
        bool download_file_by_name( const std::string & doc_name,
                                    const std::string & dist_path);
        
        bool search_word_in_file( const std::string & word, 
                                  const std::string & remote_file_name,
                                  const std::string & password);

        bool occurrence_word_in_file(const std::string & word, 
                                     const std::string & remote_file,
                                     int occur,
                                     const std::string & password);
        

            void  update_file(std::string const & remote_doc_id, 
                              boost::filesystem::path const & local_file);
            
    private:
        
        boost::shared_ptr<Config> config;
        boost::shared_ptr<DBAdapter> db_adapter;
        
        /**/
        std::string file_encryption(boost::filesystem::path const & source_file_path);
        
        bool file_decryption( const std::string & data , const boost::filesystem::path & dist_file_path);
        
    };
    
}
#endif
