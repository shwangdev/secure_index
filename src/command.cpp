#include "command.hpp"
#include "secure_index_service.hpp"
#include <boost/filesystem.hpp>
#include <vector>
#include <utility>
#include <string>
#include <iostream>

namespace secureindex
{
 
    UploadFileCommand::UploadFileCommand(boost::shared_ptr<SecureIndexService> ss): secure_index(ss)
    {
        
    }
    bool UploadFileCommand::operator()( const std::string & command, cli::ShellArguments const & argv)
    {
        if ( argv.arguments.size() != 3)
            help();
        else 
        {
            secure_index->upload_file( boost::filesystem::path(argv.arguments[1]),  argv.arguments[2] );
            std::cout<<"Success"<<std::endl;
        }
        return false;
    }
    
    void UploadFileCommand::help()
    {
        std::cout<<"hint: upload [file name] [password]"<<std::endl;
    }
    
    UploadFileCommand::~UploadFileCommand()
    {
    }
    
    ListFileCommand::ListFileCommand(boost::shared_ptr<SecureIndexService> ss): secure_index (ss)
    {
        

    }
    
    bool ListFileCommand::operator()( const std::string & command, cli::ShellArguments const & argv)
    {
        
        if ( argv.arguments.size() != 1 && argv.arguments.size()!= 2)
            help();
        else 
        {
            std::vector<std::pair<std::string, std::string> > vec = secure_index->get_uploaded_file_list();
            std::cout<<cli::prettyprint::prettyprint;
            std::cout<<"File Name"<<"\t\tFile ID"<<std::endl;
            std::cout<<"--------------------------------"<<std::endl;
                
            if ( argv.arguments.size() == 1)
            {
                
                for( int i = 0; i< vec.size(); i ++ )
                    std::cout<<vec[i].second <<"\t\t"<<vec[i].first<<std::endl;
                
            }
            else
            {
                for ( int i = 0; i < vec.size(); i ++ )
                {
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
    
    void ListFileCommand::help()
    {
        std::cout<<"Hint : list | [File Name]|[File ID]"<<std::endl;
        
        std::cout<<"List all the file in remote server."<<std::endl;
    }
    
    ListFileCommand::~ListFileCommand()
    {
    }

    DeleteFileCommand::DeleteFileCommand(boost::shared_ptr<SecureIndexService> ss):secure_index(ss)
    {
    }
    
    DeleteFileCommand::~DeleteFileCommand()
    {
    }
    
    bool DeleteFileCommand::operator()( const std::string & command , cli::ShellArguments const & argv)
    {
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
                    if( vec[i].first == argv.arguments[1])
                        secure_index->delete_file_by_id(argv.arguments[1]);
                    else
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
    
        if( argv.arguments.size()!= 4)
            help();
        
        else{
            
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
      

}
