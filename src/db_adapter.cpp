#include "db_adapter.hpp"
#include <iostream>
#include "types.hpp"
namespace secureindex{

    /*
    * Database Access Layer, all operation related with database are here.
    * All theses methods are called in secure_index_service.cpp
    */
    
    //Initiation for database connection pool.
    DBAdapter::DBAdapter( boost::shared_ptr<Config> config ):config_(config)
    {
        
        poolptr = boost::shared_ptr<MysqlConnPool>(new MysqlConnPool());
        
        //read configuration file
        if ( ! poolptr->set_config(config))
        {
            std::cerr<<"Invalid Config for db connection pool"<<std::endl;
            return ;
        }
        mysqlpp::ScopedConnection cp(*poolptr, true);
        
        //test whether configuration is valid
        if (!cp->thread_aware()) {
            std::cerr << "MySQL++ wasn't built with thread awareness! "
                      << std::endl;
            return ;
        }
    }

    //get all files from database
    std::vector<std::pair<std::string,std::string> >  DBAdapter::get_all_remote_file_list()
    {
        std::vector<std::pair<std::string, std::string> >  vec;
        
        try
        {
            mysqlpp::ScopedConnection cp( * poolptr, true);
            mysqlpp::Query query ( cp->query());
            query<<"select * from T_DOC";
            mysqlpp::StoreQueryResult res = query.store();
            
            for ( int i = 0 ; i < res.size(); i ++ )
            {
                T_DOC doc = res[i];
                
                vec.push_back(std::make_pair( doc.doc_id, doc.doc_name));
                
            }
        }
        
        catch (const mysqlpp::BadQuery& er) {
            

            std::cerr << "Query error: " << er.what() << std::endl;
            
            
        }catch (const mysqlpp::BadConversion& er) {
            
            // Handle bad conversions
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
            
            
        }
        return vec;
                
    }
   
    //add document to database 
    void DBAdapter::add_document(std::string const & doc_id,
                                 std::string const & doc_name,
                                 std::string const & index,
                                 std::string const & o_index,
                                 std::string const & stream)
    {
        try{
            
            mysqlpp::ScopedConnection cp( *poolptr,true);
            std::string statement = "select * from T_DOC where doc_id =\'" + doc_id + "\'";
            
            mysqlpp::Query query (cp->query(statement));
            
            mysqlpp::StoreQueryResult res = query.store();
            
            T_DOC row(doc_id, doc_name);

            row.doc_index = index;
            row.doc_o_index = o_index;
            row.ciphertext = stream;
            
            query.reset();
            
            if (res.empty())
            {
                query.insert(row);

                query.execute();
                
            }
            else
            {
                std::cerr<<"Docment has been uploaded already!"<<std::endl;
            }
        }catch (const mysqlpp::BadQuery& er) {
            

            std::cerr << "Query error: " << er.what() << std::endl;
            
            return ;
            
        }
        
        catch (const mysqlpp::BadConversion& er) {
            
            // Handle bad conversions
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
            
            return ;
            
        }
    }
    

    //determie whether a file ID is in database
    bool DBAdapter::is_exist_doc_id(std::string const & doc_id)
    {
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_id = \'" + doc_id + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();
            query.reset();
            if (res.empty())
            {
                return false;
            }
            else
            {
                return true;
            }
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }
    
    //determine whether a  file name is in database
    bool DBAdapter::is_exist_doc_name(std::string const & doc_name)
    {
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_name = \'" + doc_name + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();

            query.reset();

            if (res.empty())
            {
                return false;
            }
            else
            {
                return true;
            }
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }

    //delete a document in database by document id
    void DBAdapter::delete_document_by_id ( std::string const & doc_id)
    {
        try{
            
            if ( is_exist_doc_id(doc_id))
            {
                
                mysqlpp::ScopedConnection cp( * poolptr, true);
                
                std::string sql_query = "delete from T_DOC where doc_id = \'" + doc_id + "\'";
                
                mysqlpp::Query query( cp->query( sql_query));
                
                mysqlpp::StoreQueryResult res = query.store();

            }
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
        
    }
    
    //delete document by document name
    void DBAdapter::delete_document_by_name( std::string const & doc_name)
    {
        
        try{
            
            if ( is_exist_doc_name(doc_name))
            {
                
                mysqlpp::ScopedConnection cp( * poolptr, true);
                
                std::string sql_query = "delete from T_DOC where doc_name = \'" + doc_name + "\'";
                
                mysqlpp::Query query( cp->query( sql_query));
                
                mysqlpp::StoreQueryResult res = query.store();

            }
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    
    }

    //upload document in database by document id
    void DBAdapter::update_document_by_id( std::string const & old_id,
                                           std::string const & new_doc_id,
                                           std::string const & new_doc_name,
                                           std::string const & doc_index,
                                           std::string const & doc_o_index,
                                           std::string const & stream)
    {
        try{
            
            if ( is_exist_doc_id(old_id))
            {
                
                mysqlpp::ScopedConnection cp( * poolptr, true);
                
                mysqlpp::Query query( cp->query());
                query<<"select * from T_DOC where doc_id = \'"<< old_id<<"\'";
                mysqlpp::StoreQueryResult res = query.store();
                if (res.empty()) {
                    throw mysqlpp::BadQuery("UTF-8 bratwurst item not found in "
                                            "table, run resetdb");
                }
                
                T_DOC origin = res[0];
                
                T_DOC row(new_doc_id, new_doc_name);
                
                row.doc_index = doc_index;
                
                row.doc_o_index = doc_o_index;
                
                row.ciphertext = stream;
                
                query.update(origin, row);

                query.execute();
            }
            else
            {
                std::cerr<<"File "<< old_id <<" do not exist in remote server"<<std::endl;
                
            }
            
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }

    //upload document in database by document name
    void DBAdapter::update_document_by_name( std::string const & old_name,
                                             std::string const & new_doc_id,
                                             std::string const & new_doc_name,
                                             std::string const & doc_index,
                                             std::string const & doc_o_index,
                                             std::string const & stream)
    {
        try{
            
            if ( is_exist_doc_name(old_name))
            {
                
                mysqlpp::ScopedConnection cp( * poolptr, true);
                
                mysqlpp::Query query( cp->query());
                query<<"select * from T_DOC where doc_name = \'"<< old_name<<"\'";

                mysqlpp::StoreQueryResult res = query.store();
                if (res.empty()) {
                    throw mysqlpp::BadQuery("UTF-8 bratwurst item not found in "
                                            "table, run resetdb");
                }
                
                T_DOC origin = res[0];
                
                T_DOC row(new_doc_id, new_doc_name);
                
                row.doc_index = doc_index;
                
                row.doc_o_index = doc_index;
                
                row.ciphertext = stream;
                
                query.update(origin, row);

                query.execute();
            }
            else
            {
                std::cerr<<"File "<< old_name <<" does not exist in remote server"<<std::endl;
                
            }
            
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }    

    //get document index by document id
    std::string DBAdapter::get_document_index_by_id(std::string const & doc_id)
    {
        std::string result ;
        
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_id = \'" + doc_id + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();
         
            if (res.empty())
            {
                std::cerr<<"File id " << doc_id <<" does not exist in remote server"<<std::endl;

            }
            else
            {
                mysqlpp::String s =  res[0][2];
                result = std::string(s.data(), s.length());
                
            }
            return result;
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
        
    }


    //get document index by document name
    std::string DBAdapter::get_document_index_by_name(std::string const & doc_name)
    {
        std::string result ;
        
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_name = \'" + doc_name + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();
         
            if (res.empty())
            {
                std::cerr<<"File " << doc_name <<" does not exist in remote server"<<std::endl;

            }
            else
            {
                mysqlpp::String s = res[0][2];
                result = std::string(s.data(), s.length());
                            
            }
            return result;
            
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }

    std::string DBAdapter::get_document_oindex_by_name(std::string const & doc_name)
    {
        std::string result ;
        
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_name = \'" + doc_name + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();
         
            if (res.empty())
            {
                std::cerr<<"File " << doc_name <<" does not exist in remote server"<<std::endl;

            }
            else
            {
                mysqlpp::String s = res[0][3];
                result = std::string(s.data(), s.length());
                            
            }
            return result;
            
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }


    }

    //get document content from database by document name
    std::string DBAdapter::get_document_by_name(std::string const & doc_name)
    {
        std::string result;
        try{
            mysqlpp::ScopedConnection cp( *poolptr,true);

            std::string statement = "select * from T_DOC where doc_name = \'" + doc_name + "\'";

            mysqlpp::Query query (cp->query(statement));

            mysqlpp::StoreQueryResult res = query.store();
         
            if (res.empty())
            {
                std::cerr<<"File name " << doc_name <<" does not exist in remote server"<<std::endl;

            }
            else
            { 
                mysqlpp::String s = res[0][4];
                result = std::string(s.data(), s.length());            
            }
            
            return result;
        }catch (const mysqlpp::BadQuery& er) {
            std::cerr << "Query error: " << er.what() << std::endl;
        }
        catch (const mysqlpp::BadConversion& er) {
            std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        }
    }
}
