#ifndef __DB_CONN_POOL_HPP__
#define __DB_CONN_POOL_HPP__

#include <mysql++.h>
#include "singleton.hpp"
#include "config.hpp"
#include <boost/shared_ptr.hpp>
namespace secureindex
{
    
    class MysqlConnPool :public mysqlpp::ConnectionPool 
    {
        
    public:
        
        bool set_config( boost::shared_ptr<Config>  config);
        
        ~MysqlConnPool()
            {
                clear();
            }

        mysqlpp::Connection* grab()
            {
                while (conns_in_use_ > 8) {
                    sleep(1);
                }

                ++conns_in_use_;
                return mysqlpp::ConnectionPool::grab();
            }

        void release(const mysqlpp::Connection* pc)
            {
                mysqlpp::ConnectionPool::release(pc);
                --conns_in_use_;
            }

        

        MysqlConnPool()
            {
                conns_in_use_ = 0;
                config_ = boost::shared_ptr<Config>();
                
            }
        
        mysqlpp::Connection* create()
            {

                return new mysqlpp::Connection(
                    db_.empty() ? 0 : db_.c_str(),
                    server_.empty() ? 0 : server_.c_str(),
                    user_.empty() ? 0 : user_.c_str(),
                    password_.empty() ? "" : password_.c_str());
            }
        
    protected:
        void destroy(mysqlpp::Connection* cp)
            {
                delete cp;
            }
        
        unsigned int max_idle_time()
            {
                return 3;
            }

    private:
        
        boost::shared_ptr<Config> config_ ;
        std::string db_, server_, user_, password_;
        unsigned int conns_in_use_ ;
    };    
}
#endif 
