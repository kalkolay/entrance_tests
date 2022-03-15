//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#include "Buff.h"

Buff::Buff()
    : BaseInventoryElement()
{
    std::ifstream in("../configs/default_buff.json");
    in >> m_jsonParams;

    setFilters();
}

Buff::Buff(const std::string& jsonParams)
    : BaseInventoryElement(jsonParams)
{
    if ( m_jsonParams.empty() )
    {
        /// Fallback
        std::cout << "Initializing with default JSON data..." << std::endl;
        std::ifstream in("../configs/default_buff.json");
        in >> m_jsonParams;
    }

    setFilters();
}

Buff::Buff(std::string_view ident,
           BuffFilterTypeIL filters,
           std::string_view type,
           double           value)
    : BaseInventoryElement()
    , m_filters(filters)
{
    try
    {
        m_jsonParams["ident"]   = ident;
        m_jsonParams["filters"] = json::array();
        m_jsonParams["type"]    = type;
        m_jsonParams["value"]   = value;

        for (const auto& filter : filters)
            m_jsonParams["filters"].push_back( { filter } );
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        /// Fallback
        std::cout << "Initializing with default JSON data..." << std::endl;
        std::ifstream in("../configs/default_buff.json");
        in >> m_jsonParams;
    }
}

std::vector<BuffFilterType> Buff::getFilters() const { return m_filters; }

void Buff::setFilters()
{
    auto filters = m_jsonParams["filters"];
    for ( auto& [idx, filter] : filters.items() )
    {
        std::string curr = filter.dump();
        json jFilter = json::parse(curr);
        for ( auto& [key, val] : jFilter.items() )
            m_filters.emplace_back(key, val);
    }
}

