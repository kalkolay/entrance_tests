//
// Created by Nikolay Fedotenko on 06.03.2022.
//

#include "BuffFilter.h"
#include "Utility.h"

#include <algorithm>

BuffFilter::BuffFilter(Items            items,
                       BuffFilterTypeIL filters)
    : m_items  ( std::move(items) )
    , m_filters( filters )
{}

void BuffFilter::setItems(Items items)
{
    m_items = std::move(items);
}

void BuffFilter::setFilters(std::vector<BuffFilterType> newFilters)
{
    m_filters = std::move(newFilters);
}

void BuffFilter::addItem(const std::shared_ptr<Item>& item)
{
    m_items.push_back(item);
}

Items BuffFilter::applyFilters()
{
    Items tmpItems = m_items;
    for (const auto& [key, value] : m_filters)
    {
        std::string valueStr = value;  // Cast filter's value to string

        FilterTypeFn checkFn;
        if (key == "type")
        {
            checkFn = [valueStr](const auto& elem) {
                return elem->getType() != valueStr;
            };
        }
        else if (key == "level")
        {
            /// Parse level's value
            auto valueSplit = util::strSplit(value);
            std::string operation = valueSplit.first;
            double levelValue = std::stod(valueSplit.second);

            /// Check every compare operation
            checkFn = [operation, levelValue](const auto& elem) {
                if (operation == "<")
                    return elem->getLevel() >= levelValue;
                else if (operation == "<=")
                    return elem->getLevel() > levelValue;
                else if (operation == ">")
                    return elem->getLevel() <= levelValue;
                else if (operation == ">=")
                    return elem->getLevel() < levelValue;
                else if (operation == "==")
                    return elem->getLevel() != levelValue;
                else if (operation == "!=")
                    return elem->getLevel() == levelValue;
                return false;
            };
        }
        else if (key == "rarity")
        {
            checkFn = [valueStr](const auto& elem) {
                return elem->getRarityStr() != valueStr;
            };
        }
        else
        {
            /// Fallback
            std::cerr << "Invalid filter type: " << key << std::endl;
            continue;
        }

        /// Remove all items that don't suit current filter
        tmpItems.erase(
                std::remove_if(
                        tmpItems.begin(),
                        tmpItems.end(),
                        checkFn
                ),
                tmpItems.end()
        );
    }
    return tmpItems;
}
