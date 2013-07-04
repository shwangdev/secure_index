#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>
#include <cli/callbacks.hpp>
#include <cli/shell.hpp>
#include <cli/utility.hpp>
#include <boost/shared_ptr.hpp>

namespace secureindex
{
    
    class Command
    {
    public:
        Command(){}
        
        virtual bool operator()(const std::string & command, 
                                cli::ShellArguments const & arguments)
            {
                return true;        
            }
        
        virtual ~Command(){}
        

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
              
}
#endif
