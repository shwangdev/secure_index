#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "singleton.hpp"
#include <iostream>
#include <string>
#include <boost/noncopyable.hpp>

namespace secureindex{
    
    class Config : boost::noncopyable
    {
    public:

        Config(const std::string & file);
        
        void read_ini(const std::string & file);

        bool is_inited();
        
        template<typename T>
        T get(const std::string & section, const std::string & key )
            {                
                if ( ! is_inited())
                {
                    std::cerr<<"No ini file defined"<<std::endl;
                }
                std::string sk = section + "." + key;
                
                T r = pt.get<T>(sk);
                return r;
            }
        
        virtual ~Config(){};
        
    private:
        boost::property_tree::ptree pt;
        std::string ini_file;
    };
}
#endif
