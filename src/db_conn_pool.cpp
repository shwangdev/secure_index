#include "db_conn_pool.hpp"
#include <iostream>
#include <string>
namespace secureindex{
    
    bool MysqlConnPool::set_config(boost::shared_ptr <Config>  config )
    {
        config_ = config;
        if ( config_ ->is_inited())
        {
            user_ = config_->get<std::string>("DataBase","user");
            password_ = config_ ->get<std::string>("DataBase","password");
            db_ = config_->get<std::string>("DataBase", "dbname");
            server_ = config_ -> get<std::string>("DataBase", "host");
            return true;
        }
        return false;
    }
}
