//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#pragma once

#include "BaseInventoryElement.h"

using BuffFilterType   = std::pair<std::string, std::string>;
using BuffFilterTypeIL = std::initializer_list<BuffFilterType>;


/*!
 *  @class Modification (Buff) of the inventory
 *  @inherit BaseInventoryElement
 *  @ref Inventory
 */
class Buff : public BaseInventoryElement
{
public:
    /*!
     *  @brief Default constructor
     *
     *  @ref Buff::setFilters()
     *  @details Loads a default params from config
     */
    Buff();

    /*!
     *  @brief Parameterized constructor with fallbacks
     *  @param jsonParams Input JSON params
     *
     *  @ref Buff::setFilters()
     *  @note Might catch std::invalid_argument
     */
    explicit Buff(const std::string& jsonParams);

    /*!
     *  @brief Parameterized constructor with fallbacks
     *  @param ident Buff's ID
     *  @param filters Buff's filters (list)
     *  @param type Buff's type
     *  @param value Buff's value
     *
     *  @note Might catch json::type_error, json::parse_error
     */
    Buff(std::string_view ident,
         BuffFilterTypeIL filters,
         std::string_view type,
         double           value);

    /*!
     *  @brief Getter for buff's filters
     *  @return std::vector of all filters
     */
    [[nodiscard]] std::vector<BuffFilterType> getFilters() const;

private:
    /*!
     *  @brief All filters (array)
     */
    std::vector<BuffFilterType> m_filters;

    /*!
     *  @brief Helper method to set filters
     */
    void setFilters();
};

using Buffs = std::vector< std::shared_ptr<Buff> >;
