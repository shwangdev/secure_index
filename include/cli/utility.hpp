/*
 * utility.hpp - Components useful to other parts of the library
 *
 *   Copyright 2010-2012 Jesús Torres <jmtorres@ull.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <cwchar>
#include <ostream>
#include <string>
#include <vector>

#include <boost/shared_array.hpp>

#include <cli/chart_literal.hpp>
#include <cli/prettyprint.hpp>

namespace cli { namespace utility
{
    //
    // Functions for std::vector<std::string> to char*[] conversion
    //

    char** stdVectorStringToArgV(const std::vector<std::string> &strings);

    boost::shared_array<char*> stdVectorStringToSmartArgV(
        const std::vector<std::string> &strings);
}}

//
// Insert operator overload to provide support to print std::vector containers
//

namespace std
{
    template <typename CharT, typename Traits, typename T, typename Alloc>
    basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os,
        const vector<T, Alloc>& vector)
    {
        using namespace cli;

        typename std::vector<T, Alloc>::const_iterator j = vector.begin();
        if (prettyprint::isPrettyprintEnabled(os)) {
            os << CHART_LITERAL(CharT, "(vector)[");
            if (j != vector.end()) {
                os << prettyprint::endlAndIndent;
                os << CHART_LITERAL(CharT, "[0]: ") << *(j++);
                for (int i = 1; j < vector.end(); ++i, ++j) {
                    os << CHART_LITERAL(CharT, ",") << prettyprint::endl;
                    os << CHART_LITERAL(CharT, "[") << i
                        << CHART_LITERAL(CharT, "]: ") << *j;
                }
                os << prettyprint::endlAndDeindent;
            }
        }
        else {
            os << CHART_LITERAL(CharT, "[");
            if (j != vector.end()) {
                os << *j;
                for (j += 1; j < vector.end(); ++j) {
                    os << CHART_LITERAL(CharT, ", ") << *j;
                }
            }
        }

        return os << CHART_LITERAL(CharT, "]");
    }
}

#endif /* UTILITY_HPP_ */
