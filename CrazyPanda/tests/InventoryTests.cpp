//
// Created by Nikolay Fedotenko on 05.03.2022.
//

#include <gtest/gtest.h>

#include <Inventory.h>

/*! @file
 *  This source contains all unit tests for the project
 */

namespace
{
    /// Item tests

    TEST(ItemTests, EmptyItemTest)  // NOLINT
    {
        std::unique_ptr<Item> item( new Item() );

        EXPECT_TRUE( item.get() );
        EXPECT_TRUE(&item);
    }

    TEST(ItemTests, StringItemTest)  // NOLINT
    {
        Item item;
        std::string jsonData = item.getAllParameters().dump();

        std::unique_ptr<Item> stringItem( new Item(jsonData) );

        EXPECT_TRUE( stringItem.get() );
        EXPECT_TRUE(&stringItem);
    }

    TEST(ItemTests, ParameterizedItemTest)  // NOLINT
    {
        std::unique_ptr<Item> item( new Item(
                  "axe_01",
                  "Melee",
                  3,
                  EItemRarity::Common,
                  {
                      {"damage", 12},
                      {"speed", 0.9}
                  })
        );

        EXPECT_TRUE( item.get() );
        EXPECT_TRUE(&item);
    }

    TEST(ItemTests, DeletedItemTest)  // NOLINT
    {
        std::unique_ptr<Item> item( new Item() );
        Item* pItem = item.release();

        EXPECT_FALSE( item.get() );
        EXPECT_FALSE(item);

        delete pItem;
    }

    TEST(ItemTests, GetItemParamsTest)  // NOLINT
    {
        std::unique_ptr<Item> item( new Item() );

        std::ifstream in("../configs/default_item.json");
        json jsonSampleData;
        in >> jsonSampleData;

        EXPECT_EQ( jsonSampleData, item->getAllParameters() );
    }

    TEST(ItemTests, InputItemTest)  // NOLINT
    {
        Item tmpItem;
        std::string tmpItemParams = tmpItem.getAllParameters().dump(4);

        std::ifstream in("../configs/default_item.json");
        Item item;
        in >> item;
        std::string itemParams = item.getAllParameters().dump(4);

        EXPECT_STREQ( itemParams.c_str(), tmpItemParams.c_str() );
    }

    TEST(ItemTests, OutputItemTest)  // NOLINT
    {
        Item item;
        std::string itemParams = item.getAllParameters().dump(4);

        testing::internal::CaptureStdout();
        std::cout << item;
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_STREQ( output.c_str(), itemParams.c_str() );
    }

    /// Buff tests

    TEST(BuffTests, EmptyBuffTest)  // NOLINT
    {
        std::unique_ptr<Buff> buff( new Buff() );

        EXPECT_TRUE( buff.get() );
        EXPECT_TRUE(&buff);
    }

    TEST(BuffTests, StringBuffTest)  // NOLINT
    {
        Buff buff;
        std::string jsonData = buff.getAllParameters().dump();

        std::unique_ptr<Buff> stringBuff( new Buff(jsonData) );

        EXPECT_TRUE( stringBuff.get() );
        EXPECT_TRUE(&stringBuff);
    }

    TEST(BuffTests, ParameterizedBuffTest)  // NOLINT
    {
        std::unique_ptr<Buff> buff( new Buff(
                "eagle_eye",
                {
                    { "level", ">= 4" },
                    { "type", "Range" }
                },
                "DamageBuff",
                10)
        );

        EXPECT_TRUE( buff.get() );
        EXPECT_TRUE(&buff);
    }

    TEST(BuffTests, DeletedBuffTest)  // NOLINT
    {
        std::unique_ptr<Buff> buff( new Buff() );
        Buff* pBuff = buff.release();

        EXPECT_FALSE( buff.get() );
        EXPECT_FALSE(buff);

        delete pBuff;
    }

    TEST(BuffTests, GetBuffParamsTest)  // NOLINT
    {
        std::unique_ptr<Buff> buff( new Buff() );

        std::ifstream in("../configs/default_buff.json");
        json jsonSampleData;
        in >> jsonSampleData;

        EXPECT_EQ( jsonSampleData, buff->getAllParameters() );
    }

    TEST(BuffTests, InputBuffTest)  // NOLINT
    {
        Buff tmpBuff;
        std::string tmpBuffParams = tmpBuff.getAllParameters().dump(4);

        std::ifstream in("../configs/default_buff.json");
        Buff buff;
        in >> buff;
        std::string buffParams = buff.getAllParameters().dump(4);

        EXPECT_STREQ( buffParams.c_str(), tmpBuffParams.c_str() );
    }

    TEST(BuffTests, OutputBuffTest)  // NOLINT
    {
        Buff buff;
        std::string buffParams = buff.getAllParameters().dump(4);

        testing::internal::CaptureStdout();
        std::cout << buff;
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_STREQ( output.c_str(), buffParams.c_str() );
    }

    /// BuffFilter tests

    TEST(BuffFilterTests, ApplyFiltersTest)  // NOLINT
    {
        Items items;
        std::shared_ptr<Item> item( new Item() );
        items.push_back(item);

        BuffFilter filter(items,
                          {
                            { "type", "Melee" },
                            { "level", "<= 4" }
                          }
        );
        EXPECT_TRUE(&filter);

        Items filteredItems = filter.applyFilters();
        EXPECT_EQ(filteredItems, items);
    }

    /// Inventory tests

    TEST(InventoryTests, EmptyInventoryTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();

        EXPECT_TRUE(&instance);
    }

    TEST(InventoryTests, FillInventoryTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();

        std::shared_ptr<Item> item( new Item() );
        instance.addItem(item);
        EXPECT_FALSE( instance.getItems().empty() );

        std::shared_ptr<Buff> buff( new Buff() );
        instance.addBuff(buff);
        EXPECT_FALSE( instance.getBuffs().empty() );
    }

    TEST(InventoryTests, ClearInventoryTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();
        instance.clear();

        EXPECT_TRUE( instance.getItems().empty() );
        EXPECT_TRUE( instance.getBuffs().empty() );
    }

    TEST(InventoryTests, GetInventoryDataTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();

        std::shared_ptr<Item> item( new Item() );
        instance.addItem(item);

        json itemData = item->getAllParameters();
        EXPECT_FALSE( itemData.empty() );

        Items items = instance.getItems();
        json instanceData = items[0]->getAllParameters();
        EXPECT_EQ(itemData, instanceData);

        std::string itemId = item->getId();
        auto newItem = instance.getItemById(itemId);
        EXPECT_EQ(newItem, item);
    }

    TEST(InventoryTests, OutputInventoryTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();

        std::shared_ptr<Item> item( new Item() );
        instance.addItem(item);

        std::shared_ptr<Buff> buff( new Buff() );
        instance.addBuff(buff);

        testing::internal::CaptureStdout();
        std::cout << instance;
        std::string output = testing::internal::GetCapturedStdout();

        EXPECT_FALSE( output.empty() );
    }

    TEST(InventoryTests, ApplyFiltersInventoryTest)  // NOLINT
    {
        auto& instance = Inventory::getInstance();
        instance.clear();

        std::shared_ptr<Item> item( new Item() );
        instance.addItem(item);

        std::shared_ptr<Buff> buff( new Buff() );
        instance.addBuff(buff);

        instance.applyFilters(buff);
    }
}

/**
 *  @brief Run all tests
 */
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
