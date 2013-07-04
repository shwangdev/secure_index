#ifndef __TYPES_HPP__
#define __TYPES_HPP__
#include <mysql++.h>
#include <ssqls.h>

namespace secureindex{
    sql_create_4(T_DOC,
                 1,2,
                 mysqlpp::sql_char, doc_id,
                 mysqlpp::sql_char, doc_name,
                 mysqlpp::sql_text, doc_index,
                 mysqlpp::sql_longblob, ciphertext)

}
#endif
