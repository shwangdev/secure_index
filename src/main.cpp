#include <iostream>
#include <string>

#include <cli/callbacks.hpp>
#include <cli/prettyprint.hpp>
#include <cli/shell.hpp>
#include <cli/utility.hpp>
#include "command.hpp"
#include <boost/bind.hpp>
#include "config.hpp"
#include "secure_index_service.hpp"

const char INTRO_TEXT[] = "\x1b[2J\x1b[H"
    "Simple Shell - C++ Implementation\n";


const char PROMPT_TEXT[] = "secure index > $ ";


bool exitCommandCallback(const std::string& command,
                         cli::ShellArguments const& arguments)
{
    std::cout << "command:   " << command << std::endl;
    std::cout << "arguments: " << arguments << std::endl;
    std::cout << std::endl;
    return true;
}

bool defaultCommandCallback(const std::string& command,
                            cli::ShellArguments const& arguments)
{
/*    using namespace cli::prettyprint;

    std::cout << prettyprint;
    std::cout << "command:   " << command << std::endl;
    std::cout << "arguments: " << arguments << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << noprettyprint << std::endl;
*/
    std::cout<<"Invalid Command"<<std::endl;
    return false;
}


int main(int argc, char** argv)
{

    cli::ShellInterpreter interpreter;
    interpreter.setIntroText(INTRO_TEXT);
    interpreter.setPromptText(PROMPT_TEXT);
    
    boost::shared_ptr<secureindex::Config> config (new secureindex::Config(argv[1]));
    boost::shared_ptr<secureindex::SecureIndexService> service (new secureindex::SecureIndexService(config));
    
    secureindex::UploadFileCommand upload(service);
    secureindex::ListFileCommand list(service);
    secureindex::DeleteFileCommand delete_file(service);
    secureindex::SearchWordCommand search(service);
    secureindex::DownloadFileCommand download(service);
    
    secureindex::ExitCommand exit;
    
    interpreter.setCallback<cli::callback::DoCommandCallback>( upload,"upload");
    interpreter.setCallback<cli::callback::DoCommandCallback>( list, "list");
    interpreter.setCallback<cli::callback::DoCommandCallback>( delete_file, "delete");
    interpreter.setCallback<cli::callback::DoCommandCallback>( search, "search");
    interpreter.setCallback<cli::callback::DoCommandCallback>( download, "download");
    interpreter.setCallback<cli::callback::DoCommandCallback>( exit , "exit");
    
    interpreter.setCallback<cli::callback::DoCommandCallback>(
        &defaultCommandCallback);

    interpreter.loop();

    return 0;
}
