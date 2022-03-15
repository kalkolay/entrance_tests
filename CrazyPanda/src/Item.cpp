//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#include "Item.h"

Item::Item()
    : BaseInventoryElement()
{
    std::ifstream in("../configs/default_item.json");
    in >> m_jsonParams;
}

Item::Item(const std::string& jsonParams)
    : BaseInventoryElement(jsonParams)
{
    try
    {
        if ( !isRarityValid() )
            throw std::invalid_argument("Invalid Rarity value (must be the enum member)");
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;

        /// Fallback
        m_jsonParams["rarity"] = m_rarityMap[EItemRarity::None];
    }

    if ( m_jsonParams.empty() )
    {
        /// Fallback
        std::cout << "Initializing with default JSON data..." << std::endl;
        std::ifstream in("../configs/default_item.json");
        in >> m_jsonParams;
    }
}

Item::Item(std::string_view   ident,
           std::string_view   type,
           int                level,
           EItemRarity        rarity,
           ItemSpecificTypeIL specificTypes)
    : BaseInventoryElement()
{
    try
    {
        m_jsonParams["ident"]  = ident;
        m_jsonParams["type"]   = type;
        m_jsonParams["level"]  = level;
        m_jsonParams["rarity"] = m_rarityMap[rarity];

        if ( !isRarityValid() )
            m_jsonParams["rarity"] = m_rarityMap[EItemRarity::None];

        for (auto specificType : specificTypes)
            m_jsonParams[specificType.first.data()] = specificType.second;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        /// Fallback
        std::cout << "Initializing with default JSON data..." << std::endl;
        std::ifstream in("../configs/default_item.json");
        in >> m_jsonParams;
    }
}

std::string Item::getType() const { return m_jsonParams["type"]; }

double Item::getLevel() const { return m_jsonParams["level"]; }

std::string Item::getRarityStr() const { return m_jsonParams["rarity"]; }

std::istream& operator >>(std::istream& is, Item& item)
{
    is >> static_cast<BaseInventoryElement&>(item);

    try
    {
        if ( !item.isRarityValid() )
            throw std::invalid_argument("Invalid Rarity value (must be the enum member)");
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;

        item.m_jsonParams["rarity"] = item.m_rarityMap[EItemRarity::None];
    }

    return is;
}

bool Item::isRarityValid() const
{
    std::string rarityValue = m_jsonParams["rarity"];

    if ( rarityValue.empty() )
        return false;

    auto findValueInEnumMap = std::find_if(
            m_rarityMap.begin(),
            m_rarityMap.end(),
            [rarityValue](const auto& elem) {
                return elem.second == rarityValue;
            });
    if ( findValueInEnumMap == m_rarityMap.end() )
        return false;

    return true;
}
