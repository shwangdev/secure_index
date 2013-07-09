# Introduction

This project aims at implementation of secure index algorithm with C/C++ programming language. In order to demonstrat how to use this secure index algorithm , we developed a system which may interprete the usage of Secure index in searching words from encrypted documents which were stored in a remote database storage. For high performance concern , we improved some details in secure index algorithm implementation. For instance , a trie tree strcuture was used in filter dumplicated words in a word. Another example is we used a RBTree structure to calculate and store the words` occurrences.

## Dependency Packages

+ Boost 1.46 above 
+ Cryptopp
+ MysqlPP
+ Mysql-connector
+ CMake

# Archetacture

## Global View

Our system was divided into a tree layer Arch which might be considered as an implementation of MVC method. M means model, database access layer codes are in this layer. V means view , it can be an interface with user reactive. C means controller, which is the logic layer, the core algorithm of secure index is in this part.

On the other view of our project, we used "Command" design pattern, which makes command invoker and executer seperated. Which will inevidently improve the command execute rate, and our code distribution.

## Model Layer

This layer handles database related operation. And it will provide needed data for View layer. For performance concern, we implemented a database connection pool using "MysqlPP" library. The purpose of database connection pool is for improving the sql query rate, and pool is multi-thread build-in, so that it can handle quick and multy query to database smoothly.

Our database schema is a tuple which contains 4 domains, they are document id, document name , normal searcch index, occrruce search index, and encrypted document data.  Secure Index does not provide how to calculate an document id, we define a way to calculate the document as an uniq identifier with a document.  We calculate the AES hash of a document data, and put it into diemsion 256 and transferred to SHA256 result. For the document encryption we used DES algorithm in CBC mode with the usage of cryptopp library.

## Control Layer

This layer we implement the Secure Index algorithm, and wrapper it by secure index service. This secure index service will handle the read/write request from high level to model layer, and also provide necceary data to view layer. As for the secure index algorithm implementation, we used trie tree to filter duplicated words in building the normal document index, and used RBTree to count the times a word that occurred in a document. And Also , we implement bloom fiter , trapdoor , search index API in this part.  Secure Index Service will wrap them together to do a high level management. From a global view, the Secure Index servcice is singleton. There is only one instance in whole system running life-cycle. From anthor view, Secure index service can be considerred as a command invoker. It will invoke the command, and secure index build-in algorithm will handle the operation.


## View Layer

This layer we provide a bash like shell command line paser. An abstract Command Class was created and extended by real command we provided in our system.  After extended class instance was created, you have to register it to the intepreter. Each command constrcuture implementation , a seure index service param is provide , and it was a shared pointer from one memory blcok, which means no matter how many command you print in the cli shell, it will always call the same secure index servicec to handle the requests.

# Guide for source codes

Please follow these steps to read the code:

1 Read main.cpp, to understand how mand commands are provided.
2 Read command.hpp, to find the command definition.
3 Read secure_index_service.cpp and secure_index.cpp(the two important file), to understand secure index algorithm.
4 Read db_adapter.cpp, to understand database related operations.
