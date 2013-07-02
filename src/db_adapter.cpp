#include "db_adapter.hpp"
#include <iostream>

namespace secureindex{
    
    DBAdapter::DBAdapter( boost::shared_ptr<Config> config ):config_(config)
    {
        
        poolptr = boost::shared_ptr<MysqlConnPool>(new MysqlConnPool());
        
        if ( ! poolptr->set_config(config))
        {
            std::cerr<<"Invalid Config for db connection pool"<<std::endl;
            return ;
        }
        mysqlpp::ScopedConnection cp(*poolptr, true);
        
        if (!cp->thread_aware()) {
            std::cerr << "MySQL++ wasn't built with thread awareness! "
                      << std::endl;
            return ;
        }
    }
}
