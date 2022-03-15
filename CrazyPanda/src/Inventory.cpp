//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#include "Inventory.h"

Inventory& Inventory::getInstance()
{
    static Inventory instance;
    return instance;
}

Items Inventory::getItems() const { return m_items; }

Buffs Inventory::getBuffs() const { return m_buffs; }

std::shared_ptr<Item>
    Inventory::getItemById(const std::string& itemId) const
{
    auto itemIter = std::find_if(
                   m_items.begin(),
                   m_items.end(),
                  [itemId](const auto& elem) {
                            return elem->getId() == itemId;
                    });

    return (itemIter == m_items.end() ? std::shared_ptr<Item>()
                                      : *itemIter);
}

std::shared_ptr<Buff>
    Inventory::getBuffById(const std::string& buffId) const
{
    auto buffIter = std::find_if(
            m_buffs.begin(),
            m_buffs.end(),
            [buffId](const auto& elem) {
                return elem->getId() == buffId;
            });

    return (buffIter == m_buffs.end() ? std::shared_ptr<Buff>()
                                      : *buffIter);
}

void Inventory::addItem(const std::shared_ptr<Item>& item)
{
    if ( !getItemById( item->getId() ) )
    {
        m_items.push_back(item);
        m_filter.setItems(m_items);
    }
}

void Inventory::addBuff(const std::shared_ptr<Buff>& buff)
{
    if ( !getBuffById( buff->getId() ) )
        m_buffs.push_back(buff);
}

void Inventory::clear()
{
    m_items.clear();
    m_buffs.clear();
    m_filter.setItems(m_items);
}

void Inventory::applyFilters(const std::shared_ptr<Buff>& buff,
                             bool shouldPrintModifiedItems)
{
    m_filter.setFilters( buff->getFilters() );
    m_items = m_filter.applyFilters();

    if (shouldPrintModifiedItems)
    {
        std::cout << "Items after modification:\n";
        if ( m_items.empty() )
            std::cout << "[Empty]" << std::endl;
        else
        {
            for (const auto& item : m_items)
                std::cout << *item << std::endl;
        }
    }
}

std::ostream& operator <<(std::ostream&    os,
                          const Inventory& inventory)
{
    os << "Inventory items:\n";
    for (const auto& elem : inventory.m_items)
        os << *elem;
    os << std::endl;

    os << "Inventory buffs:\n";
    for (const auto& elem : inventory.m_buffs)
        os << *elem;
    os << std::endl;

    return os;
}
