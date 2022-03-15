//
// Created by Nikolay Fedotenko on 06.03.2022.
//

#include "BaseInventoryElement.h"

BaseInventoryElement::BaseInventoryElement()
    : m_jsonParams( {} )
{}

BaseInventoryElement::BaseInventoryElement(const std::string& jsonParams)
{
    try
    {
        m_jsonParams = json::parse(jsonParams);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        /// Fallback
        m_jsonParams = json::object();
    }
}

json BaseInventoryElement::getAllParameters() const
{
    return m_jsonParams;
}

std::istream& operator >>(std::istream&         is,
                          BaseInventoryElement& elem)
{
    std::string buffer, line;
    while ( std::getline(is, line) )  // Reading until EOF
        buffer += line;

    json jsonData;
    try
    {
        jsonData = json::parse(buffer);
        elem.m_jsonParams = jsonData;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return is;
}

std::ostream& operator <<(std::ostream&               os,
                          const BaseInventoryElement& elem)
{
    os << elem.m_jsonParams.dump(4);
    return os;
}

std::string BaseInventoryElement::getId() const
{
    std::string id = std::string();
    try
    {
        id = m_jsonParams["ident"];
    }
    catch (json::type_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return id;
}
