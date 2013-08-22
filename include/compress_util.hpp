#ifndef __COMPRESS_UTIL_HPP__
#define __COMPRESS_UTIL_HPP__
#include <string>
#include <zlib.h>
namespace secureindex
{

    namespace CompressUtil
    {
        std::string compress_string(const std::string& str,
                                    int compressionlevel = Z_BEST_COMPRESSION);
        
        std::string decompress_string(const std::string& str);
        
    }
}
#endif
