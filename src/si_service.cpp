#include <iostream>
#include <string>

#include <cli/callbacks.hpp>
#include <cli/prettyprint.hpp>
#include <cli/shell.hpp>
#include <cli/utility.hpp>
#include "command.hpp"
#include <boost/bind.hpp>

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
    using namespace cli::prettyprint;

    std::cout << prettyprint;
    std::cout << "command:   " << command << std::endl;
    std::cout << "arguments: " << arguments << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << noprettyprint << std::endl;
    return false;
}

//
// Main function
//

int main(int argc, char** argv)
{

    cli::ShellInterpreter interpreter;
    interpreter.setIntroText(INTRO_TEXT);
    interpreter.setPromptText(PROMPT_TEXT);

    secureindex::ExitCommand exit;
    interpreter.setCallback<cli::callback::DoCommandCallback>( exit , "exit");
    
    interpreter.setCallback<cli::callback::DoCommandCallback>(
        &defaultCommandCallback);
    


    interpreter.loop();

    return 0;
}
