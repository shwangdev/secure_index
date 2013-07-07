#include "config.hpp"

namespace secureindex
{
    //configuration of database
    Config::Config(std::string const & file)
    {
        read_ini(file);
    }
    
    void Config::read_ini(const std::string & file)
    {
        ini_file = file;
        boost::property_tree::ini_parser::read_ini(ini_file, pt);
    }
    bool Config::is_inited()
    {
        if (ini_file == "")
            return false;

        return true;
    }
}

