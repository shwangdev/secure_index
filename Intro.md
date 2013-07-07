#Secure Index Architecture
The whole project use MVC, a useful design pattern. M mean model, database related codes are in this layer. V means view, it is the user interface. C means controller, the logic layer. Detail information about every layer(including source files) is below:
##V - view
*main.cpp: the enter point of whole project, it provide a command interface for user.
##C - controller, core codes of secure index are in this layer
*secure_index.cpp: define core algorithm of secure index search, including Kpriv, ToopDoor, BuildIndex, BuildIndex for occurence search, file encryption, file decryption. Detail info please refer to the source code.
*document.cpp: define a document structure.
*command.cpp: interface for main.cpp, all user input commands are here.
*secure_index_service.cpp: the interface between controller with model.
##M - model, database access layer
*db_conn_pool.cpp: initiation a database connection pool.
*db_adapter.cpp: access database.
*config.cpp: database connection configuration.
##Guide for source codes
Please follow these steps to read the code:
1Read main.cpp, to understand how mand commands are provided.
2Read command.cpp, to find the command definition.
3Read secure_index_service.cpp and secure_index.cpp(the two important file), to understand secure index algorithm.
4Read db_adapter.cpp, to understand database related operations.
