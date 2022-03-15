//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#pragma once

/*! @file
 *  This header contains all utility functions for the project
 */

#include <iostream>
#include <string>

namespace util
{
    /*!
     *  @brief Helper function to split given string by space
     *  @param str Input string (lvalue)
     *  @return A pair of substrings: before/after the first space
     *
     *  @note Desired for parsing the json value for a filter
     *  @see  BuffFilter::applyFilters()
     */
    std::pair<std::string, std::string> strSplit(const std::string& str)
    {
        std::size_t pos = str.find(' ');
        if (pos == std::string::npos)
        {
            std::cerr << "[Incorrect string] Should contain at least one space" << std::endl;
            return {};
        }

        std::string firstStr  = str.substr(0, pos);
        std::string secondStr = str.substr(pos + 1);
        return {firstStr, secondStr};
    }
}
