//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#pragma once

#include <map>
#include <memory>

#include "BaseInventoryElement.h"

/*!
 *  @enum Possible values of item's rarity
 */
enum class EItemRarity
{
    Common,
    Rare,
    Epic,
    None    // Fallback (item not found)
};

using ItemSpecificType   = std::pair<const std::string_view, double>;
using ItemSpecificTypeIL = std::initializer_list<ItemSpecificType>;


/*!
 *  @class Item of the inventory
 *  @inherit BaseInventoryElement
 *  @ref Inventory
 */
class Item : public BaseInventoryElement
{
public:
    /*!
     *  @brief Default constructor
     *
     *  @details Loads a default params from config
     */
    Item();

    /*!
     *  @brief Parameterized constructor with fallbacks
     *  @param jsonParams Input JSON params
     *
     *  @ref Item::isRarityValid()
     *  @note Might catch std::invalid_argument
     */
    explicit Item(const std::string& jsonParams);

    /*!
     *  @brief Parameterized constructor with fallbacks
     *  @param ident Item's ID
     *  @param type Item's type
     *  @param level Item's level
     *  @param rarity Item's rarity
     *  @param specificTypes Item's specific types (list)
     *
     *  @note Might catch json::type_error, json::parse_error
     */
    Item(std::string_view   ident,
         std::string_view   type,
         int                level,
         EItemRarity        rarity,
         ItemSpecificTypeIL specificTypes);

    /*! Params getter methods */
    [[nodiscard]] std::string getType     () const;
    [[nodiscard]] double      getLevel    () const;
    [[nodiscard]] std::string getRarityStr() const;

    /*!
     *  @brief Input operator for Item object
     *  @overload BaseInventoryElement
     *  @param is selected std::istream object
     *  @param item current class object
     *  @return Reference of selected std::istream object
     *
     *  @ref Item::isRarityValid()
     *  @note Might catch std::invalid_argument
     */
    friend std::istream& operator >>(std::istream& is, Item& item);

private:
    /*!
     *  @brief Helper for mapping "rarity" param from enum to strings
     */
    using EItemRarityMap = std::map<EItemRarity,
                                    const std::string_view>;
    EItemRarityMap m_rarityMap = {
            { EItemRarity::Common, "Common" },
            { EItemRarity::Rare,   "Rare" },
            { EItemRarity::Epic,   "Epic" },
            { EItemRarity::None,   "NONE" }
    };

    /*!
     *  @brief Helper method to check if item's rarity is valid
     *  @return True if rarity is valid
     *
     *  @see m_rarityMap
     */
    [[nodiscard]] bool isRarityValid() const;
};

using Items = std::vector< std::shared_ptr<Item> >;
