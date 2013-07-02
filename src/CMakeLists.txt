SET(SI_SERVER_SRCS
  db_conn_pool.cpp config.cpp document.cpp secure_index.cpp db_adapter.cpp si_server.cpp 
  )

ADD_EXECUTABLE(si_server ${SI_SERVER_SRCS} )
TARGET_LINK_LIBRARIES(si_server ${MySQLPP_LIBRARIES} boost_serialization 
  boost_system boost_filesystem cryptopp mysqlpp)
