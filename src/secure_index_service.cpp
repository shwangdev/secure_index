/**
 * @file   secure_index_service.cpp
 * @author  <devil@Funtoo>
 * @date   Tue Jul  9 13:21:30 2013
 * 
 * @brief  Secure Index Service Implementation
 * 
 * 
 */
#include "secure_index_service.hpp"
#include "secure_index.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <cryptopp/base64.h>
#include <cryptopp/base32.h>
#include <cryptopp/des.h>        // DES
#include <cryptopp/modes.h>      // CBC_Mode< >
#include <cryptopp/filters.h>    // StringSource
#include <cryptopp/files.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>

namespace secureindex{

    byte key[CryptoPP::DES::DEFAULT_KEYLENGTH] = {0x29, 0x4F, 0x5B, 0x4E, 0x42, 0x5D, 0x36, 0x2C};
    byte iv[ CryptoPP::DES::BLOCKSIZE ] = {0};

    SecureIndexService::SecureIndexService(boost::shared_ptr<Config> config_)
    {
        config = config_;
       
        
        db_adapter = boost::shared_ptr<DBAdapter> (new DBAdapter(config));
        
    }
    
    /** 
     * Encryption document content
     *
     * @param file_path 
     *
     * @return 
     */
    std::string SecureIndexService::file_encryption(boost::filesystem::path const & file_path)
    {
        
        std::string result;

        if ( boost::filesystem::exists(file_path))
        {
            //encrption algorithm
            CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption Encryptor(key, sizeof(key),iv);

            CryptoPP::FileSource( file_path.string().c_str() , true,
                                  new CryptoPP::StreamTransformationFilter(Encryptor,
                                                                           new CryptoPP::Base64Encoder(
                                                                               new CryptoPP::StringSink(result)
                                                                               )));
        }
        else
            std::cerr<<"Invalid file path"<<std::endl;
        return result;
    }

    /** 
     * Decryption document content and sotre the content in file system.
     *
     * @param data 
     * @param dist_file 
     *
     * @return 
     */

    bool SecureIndexService::file_decryption(std::string  const & data , boost::filesystem::path const & dist_file)
    {
        try{
            
            CryptoPP::CBC_Mode<CryptoPP::DES>::Decryption Decryptor(key, sizeof(key), iv);
    
            CryptoPP::StringSource( data, true,
                                    new CryptoPP::Base64Decoder(
                                        new CryptoPP::StreamTransformationFilter( Decryptor,
                                                                                  new CryptoPP::FileSink(dist_file.string().c_str())
                                            )
                                        ) 
                );
        }catch(...)
        {
            return false;
        }
        return true;
        
    }

    /** 
     * List file in database
     *
     *
     * @return 
     */
    std::vector<std::pair<std::string, std::string> > SecureIndexService::get_uploaded_file_list()
    {
        return db_adapter->get_all_remote_file_list();
    }
        
    //upload file to database
    void SecureIndexService::upload_file(const boost::filesystem::path & local_file,
                                         const std::string & password)
    {
        std::string file_content = file_encryption(local_file);
        boost::shared_ptr<Document> doc (new Document(local_file.string()));

        Kpriv k(password, 4);
        
        SecureIndex(doc, k);
        
        Index index = doc->index;
        
        Index oindex = doc->oindex;
        
        std::stringstream ssi, sso;

        boost::archive::text_oarchive oai(ssi);
        boost::archive::text_oarchive oao(sso);
        
        oai << index;
        oao << oindex;
        
        std::string index_content = ssi.str();
        std::string oindex_content = sso.str();
        
        std::cout<<"Upload File: "<< doc->get_document_name()<<std::endl;
        std::cout<<"File   ID  : "<< doc->get_document_id()<<std::endl;
        
        db_adapter->add_document(doc->get_document_id(),
                                 doc->get_document_name(),
                                 index_content,
                                 oindex_content,
                                 file_content);
                
    }
    
    /** 
     * Delete document in database
     *
     * @param doc_id 
     */
    void SecureIndexService::delete_file_by_id(std::string const & doc_id)
    {

        db_adapter->delete_document_by_id(doc_id);
        
    }
    
    /** 
     * Delete document in database
     *
     * @param doc_name 
     */
    void SecureIndexService::delete_file_by_name(std::string const & doc_name)
    {
        db_adapter->delete_document_by_name(doc_name);
    }
    
    /** 
     * Search a word in a file
     *
     * @param word 
     * @param remote_file 
     * @param password 
     *
     * @return 
     */
    bool SecureIndexService::search_word_in_file(std::string const & word, 
                                                 std::string const & remote_file,
                                                 std::string const & password)
    {
        
        Kpriv k ( password, 4);

        std::string index_content = db_adapter->get_document_index_by_name(remote_file);
        
        std::stringstream ss(index_content);
        boost::archive::text_iarchive ia(ss);
        
        Index index ;
        ia >> index;
        boost::shared_ptr<SecureIndex> secure_index ( new SecureIndex(k));
        Trapdoor t( k, word);
        return secure_index->search_index(t, index );
    }

    /** 
     * Occrrence search in a file
     *
     * @param word 
     * @param remote_file 
     * @param occur 
     * @param password 
     *
     * @return 
     */

    bool SecureIndexService::occurrence_word_in_file(const std::string & word, 
                                                    const std::string & remote_file,
                                                    int occur,
                                                    const std::string & password)
    {
        Kpriv k(password, 4);
        
        std::string oindex_content = db_adapter->get_document_oindex_by_name(remote_file);
        
        std::stringstream ss(oindex_content);
        boost::archive::text_iarchive ia(ss);
        
        Index oindex;
        
        ia>> oindex;
        
        boost::shared_ptr<SecureIndex> secure_index (new SecureIndex(k));
        Trapdoor t( k, word, occur);
        return secure_index->search_index(t,oindex);
    }
    
    /** 
     * Download file from database, use file_decryption
     *
     * @param doc_name 
     * @param dist_path 
     *
     * @return 
     */
    bool SecureIndexService::download_file_by_name ( const std::string & doc_name, const  std::string & dist_path) 
    {
        std::string ciphertext = db_adapter->get_document_by_name(doc_name);
        boost::filesystem::path dist_file (dist_path + "/" + doc_name);
        return file_decryption(ciphertext, dist_file);
    }

}
