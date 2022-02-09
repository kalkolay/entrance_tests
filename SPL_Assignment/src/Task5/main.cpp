#include "List.h"

/// Task 5 : Testing

int main()
{
    std::cout << "====== Task 5 ======" << std::endl;

    List list;
    const char* pFilePath = "task5.txt";
    FILE* output = fopen(pFilePath, "wb");
    std::cout << "File opened for write: " << pFilePath << std::endl;

    auto node1 = new ListNode;
    node1->data = "Hello, World!";
    auto node2 = new ListNode;
    node2->data = "This is a test file for task 5.";
    auto node3 = new ListNode;
    node3->data = "";
    auto node4 = new ListNode;
    node4->data = "THE END";
    node2->rand = node1;
    node4->rand = node2;
    list.PushBack(node1);
    list.PushBack(node2);
    list.PushBack(node3);
    list.PushBack(node4);

    list.Serialize(output);
    std::cout << "File serialized!" << std::endl;
    fclose(output);

    List newList;
    FILE* input = fopen(pFilePath, "rb");
    std::cout << "File opened for read: " << pFilePath << std::endl;

    newList.Deserialize(input);
    std::cout << "File deserialized!" << std::endl;
    fclose(input);

    int32_t i = 0;
    ListNode* pList = newList.GetHead();
    std::cout << "Read strings: " << newList.GetCount() << std::endl;
    while (pList)
    {
        std::cout << "\tstring " << i << ": " << pList->data.c_str() << std::endl;
        pList = pList->next;
        ++i;
    }

    return 0;
}
