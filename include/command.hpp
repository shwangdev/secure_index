/**
 * @file   command.hpp
 * @author  <devil@Funtoo>
 * @date   Mon Jul  8 09:35:29 2013
 * 
 * @brief  The command class is the real action when supported commands were enterred from cli
 * 
 * 
 */


#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>
#include <cli/callbacks.hpp>
#include <cli/shell.hpp>
#include <cli/utility.hpp>
#include <boost/shared_ptr.hpp>

namespace secureindex
{
    class SecureIndexService;
    
    
    class Command
    {
    public:
        Command(){}

        /** 
         * 
         * Abstruct call back function which will be implemented by extended classes.
         *
         * @return 
         */
        virtual bool operator()(const std::string & command, 
                                cli::ShellArguments const & arguments)
            {
                return true;        
            }
        
        virtual ~Command(){}
        
        virtual void help()
            {
                std::cout<<"Hint: Help"<<std::endl;
            }
        
    private:

        std::string command_name;
    };
    
    class ExitCommand: public Command
    {
    public:
        ExitCommand()
            {
            }
        
        virtual bool operator()( const std::string & command, cli::ShellArguments const & argv)
            {
                return true;
                
            }
    };
    
    class HelpCommand: public Command
    {
    public:
        HelpCommand()
            {
            }
        
        virtual bool operator() ( const std::string & command , const cli::ShellArguments & argv );
    };


    class UploadFileCommand: public Command
    {
    public:
        UploadFileCommand( boost::shared_ptr<SecureIndexService> ss);
        
        virtual bool operator()( const std::string & command, cli::ShellArguments const & argv);
        virtual void help();
        virtual ~UploadFileCommand();
    private:
        boost::shared_ptr<SecureIndexService> secure_index;
    };

    
    class ListFileCommand:public Command
    {
    public:

        ListFileCommand (boost::shared_ptr<SecureIndexService> ss);
        
        virtual bool operator()( const std::string & command, cli::ShellArguments const & argv);
        
        virtual void help();
        virtual ~ListFileCommand();
    private:
        boost::shared_ptr<SecureIndexService> secure_index;
        
    };
    
    class DeleteFileCommand:public Command
    {
    public:
        DeleteFileCommand(boost::shared_ptr<SecureIndexService> ss);
        virtual bool operator()(const std::string &command, cli::ShellArguments const & argv);
        virtual void help();
        ~DeleteFileCommand();
        
    private:
        boost::shared_ptr<SecureIndexService> secure_index;
    };
    
    
    class SearchWordCommand : public Command
    {
    public:
        SearchWordCommand(boost::shared_ptr<SecureIndexService> ss);
        virtual bool operator()( const std::string & command , cli::ShellArguments const & argv);
        virtual void help();
        virtual ~SearchWordCommand( );
    private:
        boost::shared_ptr<SecureIndexService> secure_index;

    };

    class DownloadFileCommand : public Command
    {
    public:
        DownloadFileCommand(boost::shared_ptr<SecureIndexService> ss);
        virtual bool operator()( const std::string & command , cli::ShellArguments const & argv);
        virtual void help();
        virtual ~DownloadFileCommand( );
    private:
        boost::shared_ptr<SecureIndexService> secure_index;

    };
    
    class OccurrenceCommand: public Command
    {
    public:

        OccurrenceCommand ( boost::shared_ptr<SecureIndexService> ss);
        
        virtual bool operator()( const std::string & command, cli::ShellArguments const & argv);
        virtual void help();
        
        virtual ~OccurrenceCommand();

    private:

        boost::shared_ptr<SecureIndexService> secure_index;
    };


    class TestCommand: public Command
    {
    public:
        TestCommand (boost::shared_ptr<SecureIndexService> ss);
        
        virtual bool operator()( const std::string & command , cli::ShellArguments const & argv);

        virtual void help();

        virtual ~TestCommand();
        
    private:

        boost::shared_ptr<SecureIndexService> secure_index;
        
    };
                      
}
#endif
