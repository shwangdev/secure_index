/**
 * @file   command.cpp
 * @author  <devil@Funtoo>
 * @date   Mon Jul  8 09:37:11 2013
 * 
 * @brief  Command Class implementation
 * 
 * 
 */

#include "command.hpp"
#include "secure_index_service.hpp"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <utility>
#include <string>
#include <iostream>

namespace secureindex
{

    /*
     * Definiation of all Commands.
     */

    //UploadFileCommand: upload file to database.
    UploadFileCommand::UploadFileCommand(boost::shared_ptr<SecureIndexService> ss): secure_index(ss)
    {
        
    }
    //Operation for UploadFileCommand.
    bool UploadFileCommand::operator()( const std::string & command, cli::ShellArguments const & argv)
    {
        // UploadCommand require 3 arguments.
        if ( argv.arguments.size() < 3)
            help();
        else 
        {
            //Real operation in secure_index_service.cpp.
            size_t size = argv.arguments.size();
            
            for(int i = 0 ; i < size -2; i ++)
            {
                if ( boost::filesystem::is_directory( boost::filesystem::path(argv.arguments[i+1]) ))
                    secure_index->upload_folder(boost::filesystem::path(argv.arguments[i+1]),  argv.arguments[size-1] );
                else
                    secure_index->upload_file( boost::filesystem::path(argv.arguments[i+1]),  argv.arguments[size-1] );
            }
            
            std::cout<<"Success"<<std::endl;
        }
        return false;
    }
    
    // Help info for UploadFileCommand.
    void UploadFileCommand::help()
    {
        std::cout<<"hint: upload [file names] [password]"<<std::endl;
    }
    
    //Constructor.
    UploadFileCommand::~UploadFileCommand()
    {
    }
    
    
    //ListFileCommand: list file in database.
    ListFileCommand::ListFileCommand(boost::shared_ptr<SecureIndexService> ss): secure_index (ss)
    {
        

    }
    
    //Opetation for ListFileCommand.
    bool ListFileCommand::operator()( const std::string & command, cli::ShellArguments const & argv)
    {
        //need 1 or 2 arguments. For 1 argv, display all files, for 2 argvs, display matchec files.
        if ( argv.arguments.size() != 1 && argv.arguments.size()!= 2)
            help();
        else 
        {
            //Real operation in secure_index_service.cpp, return types is std::vector<std::pair<std::string, std::string>>
            std::vector<std::pair<std::string, std::string> > vec = secure_index->get_uploaded_file_list();
            std::cout<<cli::prettyprint::prettyprint;
            std::cout<<"File Name"<<"\t\tFile ID"<<std::endl;
            std::cout<<"--------------------------------"<<std::endl;
                
            if ( argv.arguments.size() == 1)
            {
                //Display all files in database
                for( int i = 0; i< vec.size(); i ++ )
                    std::cout<<vec[i].second <<"\t\t"<<vec[i].first<<std::endl;
                
            }
            else
            {
                //loop each matched file, display file info
                for ( int i = 0; i < vec.size(); i ++ )
                {
                    //match condition
                    if ( vec[i].first == argv.arguments[2] || vec[i].second == argv.arguments[2])
                    {
                
                        std::cout<<vec[i].second <<"\t\t"<<vec[i].first<<std::endl;
                        break;
                    }
                }
            }
            std::cout<<cli::prettyprint::noprettyprint;
        }
        
        return false;
    }
   
    //Help info for ListFileCommand 
    void ListFileCommand::help()
    {
        std::cout<<"Hint : list | [File Name]|[File ID]"<<std::endl;
        
        std::cout<<"List all the file in remote server."<<std::endl;
    }
    
    ListFileCommand::~ListFileCommand()
    {
    }


    //DeleteFileCommand
    DeleteFileCommand::DeleteFileCommand(boost::shared_ptr<SecureIndexService> ss):secure_index(ss)
    {
    }
    
    DeleteFileCommand::~DeleteFileCommand()
    {
    }
    
    bool DeleteFileCommand::operator()( const std::string & command , cli::ShellArguments const & argv)
    {
        //DeleteFileCommand need 2 argvs
        if ( argv.arguments.size() != 2 )
            help();
        else
        {
            std::vector<std::pair<std::string, std::string> > vec = secure_index->get_uploaded_file_list();
            for( int i = 0 ; i< vec.size(); i ++ )
            {
                if( vec[i].first == argv.arguments[1]|| vec[i].second == argv.arguments[1])
                {
                    
                    std::cout<<"Deleting File:"<<std::endl;
                    
                    std::cout<<"------------------------------"<<std::endl;
                    
                    std::cout<<"File Name"<<"\t\t"<<"File ID"<<std::endl;
                    
                    std::cout<<vec[i].second<<"\t\t"<<vec[i].first<<std::endl;
                    
                    //delete by file ID
                    if( vec[i].first == argv.arguments[1])
                        secure_index->delete_file_by_id(argv.arguments[1]);
                    else//delete by file name
                        secure_index->delete_file_by_name(argv.arguments[1]);
                    
                    std::cout<<std::endl<<"Deleted"<<std::endl;
                                        
                }
                
            }
                        
        }
        return false;
    }
    
    void DeleteFileCommand::help()
    {
        std::cout<<"Hit : delete [File Name] | [ File ID ]" <<std::endl;
    }            


    
    SearchWordCommand::SearchWordCommand(boost::shared_ptr<SecureIndexService> ss): secure_index(ss)
    {
        
    }
    SearchWordCommand::~SearchWordCommand()
    {
        
    }
    
    bool SearchWordCommand::operator()(const std::string & command, cli::ShellArguments const & argv)
    {
        //need 4 argvs
        if( argv.arguments.size()!= 4)
            help();
        
        else{
            //Real operation if secure_index_service.cpp
            if( secure_index->search_word_in_file( argv.arguments[1], 
                                                   argv.arguments[2], 
                                                   argv.arguments[3]) )
            {
                std::cout<<"Word: "<< argv.arguments[1] << " is in file "
                         <<argv.arguments[2]<<"."<<std::endl;
            }
            else
            {
                std::cout<<"Word: "<< argv.arguments[1] << " is not in file "
                         <<argv.arguments[2]<<"."<<std::endl;
            }
        }
        
        return false;
    }
    
    void SearchWordCommand::help()
    {
        std::cout<<"Hint : search [word] [file name] [password]"<<std::endl;
    }
      

    DownloadFileCommand::DownloadFileCommand(boost::shared_ptr<SecureIndexService> ss): secure_index(ss)
    {
        
    }
    DownloadFileCommand::~DownloadFileCommand()
    {
        
    }
    
    bool DownloadFileCommand::operator()(const std::string & command, cli::ShellArguments const & argv)
    {
    
        if( argv.arguments.size()!= 3)
            help();
        
        else{
            
            if(secure_index->download_file_by_name( argv.arguments[1], argv.arguments[2]) )
            {
                std::cout<<"Download Success"<<std::endl;
            }
            else 
            {
                std::cout<<"Download Error"<<std::endl;
            }
            
            
        }
        
        return false;
    }
    
    void DownloadFileCommand::help()
    {
        std::cout<<"Hint : downlod [File Name] [Destination Directory]"<<std::endl;
    }

    OccurrenceCommand::OccurrenceCommand( boost::shared_ptr<SecureIndexService> ss):secure_index(ss)
    {
        
    }
    
    OccurrenceCommand::~OccurrenceCommand()
    {

    }
    

    bool OccurrenceCommand::operator()( const std::string & command, cli::ShellArguments const & argv)
    {
        if ( argv.arguments.size() != 5 )
            help();
        
        else
        {
            int count = 0;
            try{
                count = boost::lexical_cast<int>( argv.arguments[2] );

                bool result = secure_index->occurrence_word_in_file( argv.arguments[1],
                                                                     argv.arguments[3],
                                                                     count , 
                                                                     argv.arguments[4]);

                if ( result)
                    std::cout<<"Word : "<< argv.arguments[1]
                             <<" occurs at least "<< count 
                             <<" times in file "<< argv.arguments[3]<<std::endl;
                else
                    std::cout<<"Word : "<< argv.arguments[1]
                             <<" does not occur "<<count
                             <<" times in file "<< argv.arguments[3]<<std::endl;
        
            }
            catch (boost::bad_lexical_cast &)
            {
                std::cerr<<"Invaid occurrence count"<<std::endl;
                return false;
            }
                        
        }
        return false;
    }

    void OccurrenceCommand::help()
    {
        std::cout<<"Hint : occur [ Word ] [ Numb ] [ File Name ] [ Password]" <<std::endl;
        
    }
    
    
}
