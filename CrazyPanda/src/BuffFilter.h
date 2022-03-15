//
// Created by Nikolay Fedotenko on 06.03.2022.
//

#pragma once

#include <functional>

#include "Item.h"

using BuffFilterType   = std::pair<std::string, std::string>;
using BuffFilterTypeIL = std::initializer_list<BuffFilterType>;
using FilterTypeFn     = std::function<bool(std::shared_ptr<Item>)>;


/*!
 *  @class Helper class for applying filters on items from buffs
 *  @ref Item, Buff, Inventory
 */
class BuffFilter
{
public:
    /*!
     *  @brief Default constructor
     */
    BuffFilter() = default;

    /*!
     *  Parameterized constructor
     *  @param items Vector of items' shared pointers
     *  @param filters List of buffs' filters
     */
    explicit BuffFilter(Items            items,
                        BuffFilterTypeIL filters = {} );

    /*! Setters */
    void setItems  (Items                       items);
    void setFilters(std::vector<BuffFilterType> newFilters);

    /*!
     *  @brief Add a new item
     *  @param item Shared pointer of item
     */
    void addItem(const std::shared_ptr<Item>& item);

    /*!
     *  @brief Applies all filters to each item
     *  @return
     *  Vector of items (shared_ptrs) that suit all filters
     *
     *  @ref util::strSplit()
     *  @details Uses std::function to apply parsed filters
     */
    Items applyFilters();

private:
    /*!
     *  @brief Vector of items' shared pointers
     */
    Items                       m_items;
    /*!
     *  @brief Vector of buffs' filters
     */
    std::vector<BuffFilterType> m_filters;
};
