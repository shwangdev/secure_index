SET(SI_SERVER_SRCS
  cli/basic_spirit.cpp  cli/fileno.cpp       cli/readline.cpp  cli/words.cpp
  cli/command.cpp       cli/glob.cpp         cli/shell.cpp
  cli/dl.cpp            cli/prettyprint.cpp  cli/utility.cpp  
  db_conn_pool.cpp config.cpp document.cpp secure_index.cpp db_adapter.cpp command.cpp secure_index_service.cpp 
  compress_util.cpp main.cpp

  )

ADD_EXECUTABLE(si_service ${SI_SERVER_SRCS} )
TARGET_LINK_LIBRARIES(si_service ${MySQLPP_LIBRARIES} boost_thread boost_serialization boost_system boost_filesystem cryptopp mysqlpp z)
