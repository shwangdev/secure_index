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

//Enter point of projetct.
int main(int argc, char** argv)
{

    cli::ShellInterpreter interpreter;
    interpreter.setIntroText(INTRO_TEXT);
    interpreter.setPromptText(PROMPT_TEXT);
    
    boost::shared_ptr<secureindex::Config> config (new secureindex::Config(argv[1]));
    boost::shared_ptr<secureindex::SecureIndexService> service (new secureindex::SecureIndexService(config));
    
    //Definaitionof commands.
    secureindex::UploadFileCommand upload(service);//upload command: upload file to Database.
    secureindex::ListFileCommand list(service);//list command: list all uploaded files in Database.
    secureindex::DeleteFileCommand delete_file(service);//delete command: delete file in Database.
    secureindex::SearchWordCommand search(service);//search command: Secure Index search
    secureindex::DownloadFileCommand download(service);//download command: Download file from Database.
    secureindex::OccurrenceCommand occur(service);//occurrence search command: Occurrence Search
    secureindex::ExitCommand exit;
    
    //Definition of callback command, eg: UploadFileCommand will be executed for upload.
    interpreter.setCallback<cli::callback::DoCommandCallback>( upload,"upload");
    interpreter.setCallback<cli::callback::DoCommandCallback>( list, "list");
    interpreter.setCallback<cli::callback::DoCommandCallback>( delete_file, "delete");
    interpreter.setCallback<cli::callback::DoCommandCallback>( search, "search");
    interpreter.setCallback<cli::callback::DoCommandCallback>( download, "download");
    interpreter.setCallback<cli::callback::DoCommandCallback>( occur, "occur");
    interpreter.setCallback<cli::callback::DoCommandCallback>( exit , "exit");
    
    //default command will be executed if command no in "upload list delete ...."
    interpreter.setCallback<cli::callback::DoCommandCallback>(
        &defaultCommandCallback);

    interpreter.loop();

    return 0;
}
