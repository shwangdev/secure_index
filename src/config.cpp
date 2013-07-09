#include "config.hpp"

namespace secureindex
{
    /** 
     * Config structure construction
     *
     * @param file 
     */
    Config::Config(std::string const & file)
    {
        read_ini(file);
    }
    
    /** 
     * read the config file
     *
     * @param file 
     */
    void Config::read_ini(const std::string & file)
    {
        ini_file = file;
        boost::property_tree::ini_parser::read_ini(ini_file, pt);
    }

    /** 
     * check if config instance is initalized
     *
     *
     * @return 
     */
    bool Config::is_inited()
    {
        if (ini_file == "")
            return false;

        return true;
    }
}

