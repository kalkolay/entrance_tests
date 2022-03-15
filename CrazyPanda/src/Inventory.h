//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#pragma once

#include "Item.h"
#include "Buff.h"
#include "BuffFilter.h"

/*!
 *  @class Inventory class itself (Singleton)
 *  @ref Item, Buff, BuffFilter
 */
class Inventory
{
public:
    /*!
     *  @details
     *  Delete copy constructor & assignment operator
     *  in order to implement a Singleton
     */
    Inventory(const Inventory&)       = delete;
    void operator =(const Inventory&) = delete;

    /*!
     *  @brief Static instance getter
     *  @return Instance of this class
     */
    static Inventory& getInstance();

    /*! Params getter methods */
    [[nodiscard]] Items getItems() const;
    [[nodiscard]] Buffs getBuffs() const;

    /*! Objects getter methods (by IDs) */
    [[nodiscard]] std::shared_ptr<Item>
        getItemById(const std::string& itemId) const;
    [[nodiscard]] std::shared_ptr<Buff>
        getBuffById(const std::string& buffId) const;

    /*! Add new elements in the inventory */
    void addItem(const std::shared_ptr<Item>& item);
    void addBuff(const std::shared_ptr<Buff>& buff);

    /*! Clears all data */
    void clear();

    /*!
     *  @brief Applies filter of given buff on all items
     *  @param buff Buff which filters do we need to apply
     *  @param shouldPrintModifiedItems
     *  True if we need to print the modified items
     */
    void applyFilters(const std::shared_ptr<Buff>& buff,
                      bool shouldPrintModifiedItems = false);

    /*!
     *  @brief Output operator for the inventory
     *  @param os selected std::ostream object
     *  @param inventory current class object
     *  @return Reference of selected std::ostream object
     */
    friend std::ostream& operator <<(std::ostream&    os,
                                     const Inventory& inventory);

private:
    /*! Aggregated objects */
    Items      m_items;
    Buffs      m_buffs;
    BuffFilter m_filter;

    /*!
     *  @brief Default constructor
     *  @details Private because of the Singleton
     */
    Inventory() = default;
};
