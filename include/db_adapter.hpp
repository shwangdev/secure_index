#ifndef __FOLLOWME_DB_ADAPTER_HPP__
#define __FLOOLWME_DB_ADAPTER_HPP__
#include "db_conn_pool.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "db_conn_pool.hpp"
#include "config.hpp"

namespace secureindex{
    class DBAdapter :public boost::noncopyable
    {
    public:

        DBAdapter( boost::shared_ptr<Config> config );
        
    private:
        
        boost::shared_ptr<MysqlConnPool> poolptr;
        boost::shared_ptr<Config> config_;
        
    };
}
#endif
