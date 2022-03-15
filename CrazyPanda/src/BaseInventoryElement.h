//
// Created by Nikolay Fedotenko on 06.03.2022.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <initializer_list>

/*!
 *  @brief Third-party library for JSON files
 */
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;  // No need to sort JSON items

/*!
 *  @class Base class for the inventory element
 *  @ref Inventory
 *
 *  @details
 *  - nlohmann/json is used to store the params
 *  - Object of this class cannot be created (inheritance only)
 */
class BaseInventoryElement
{
public:
    /*!
     *  @brief Getter for all JSON params of the element
     *  @return m_jsonParams
     */
    [[nodiscard]] json getAllParameters() const;
    /*!
     *  @brief Getter for ID of the element
     *  @return m_jsonParams["ident"] as string
     *
     *  @note Might catch json::type_error
     */
    [[nodiscard]] std::string getId() const;

    /*!
     *  @brief Input operator for base inventory element
     *  @param is selected std::istream object
     *  @param elem current class object
     *  @return Reference of selected std::istream object
     *
     *  @note Might catch json::type_error, json::parse_error
     */
    friend std::istream& operator >>(std::istream&         is,
                                     BaseInventoryElement& elem);
    /*!
     *  @brief Output operator for base inventory element
     *  @param os selected std::ostream object
     *  @param elem current class object
     *  @return Reference of selected std::ostream object
     */
    friend std::ostream& operator <<(std::ostream&               os,
                                     const BaseInventoryElement& elem);

protected:
    /*!
     *  @brief All JSON params of the element
     *
     *  @see nlohmann/json
     */
    json m_jsonParams;

    /*!
     *  @brief Protected default constructor
     */
    BaseInventoryElement();
    /*!
     *  @brief Protected copy constructor
     *
     *  @note Might catch json::type_error, json::parse_error
     */
    explicit BaseInventoryElement(const std::string& jsonParams);

    /*!
     *  @brief Virtual destructor
     */
    virtual ~BaseInventoryElement() = default;
};
