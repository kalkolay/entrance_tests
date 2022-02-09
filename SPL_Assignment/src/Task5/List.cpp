#include "List.h"

#include <map>

/// Task 5 : Solution

void List::PushBack(ListNode* node)
{
    if (!count)
    {
        head = node;
        tail = node;
    }
    else
    {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    ++count;
}

List::~List()
{
    auto ptr = head;
    while (ptr)
    {
        ListNode* tmp = ptr->next;
        delete ptr;
        ptr = tmp;
    }
}

void List::Serialize(FILE* file)
{
    fwrite(&count, sizeof(count), 1, file);
    ListNode* ptr = head;
    for (int i = 0; i < count; ++i)
    {
        fwrite(&ptr, sizeof(ptr), 1, file);
        fwrite(&ptr->prev, sizeof(ptr->prev), 1, file);
        fwrite(&ptr->next, sizeof(ptr->next), 1, file);
        fwrite(&ptr->rand, sizeof(ptr->rand), 1, file);
        auto dataSize = ptr->data.size();
        fwrite(&dataSize, sizeof(dataSize), 1, file);
        fprintf( file, "%s", ptr->data.data() );
        ptr = ptr->next;
    }
}

void List::Deserialize(FILE *file)
{
    count = 0;
    int newCount;
    std::map<ListNode*, std::pair<ListNode*, ListNode*>> mp;
    fread(&newCount, sizeof(newCount), 1, file);
    for (int i = 0; i < newCount; ++i)
    {
        ListNode *ptr, *pPtr, *nPtr, *rPtr;
        std::string::size_type dataSize;
        fread(&ptr,  sizeof(ptr),  1, file);
        fread(&pPtr, sizeof(pPtr), 1, file);
        fread(&nPtr, sizeof(nPtr), 1, file);
        fread(&rPtr, sizeof(rPtr), 1, file);

        fread(&dataSize, sizeof(dataSize), 1, file);
        char text[dataSize];

        fread(text, sizeof(char), dataSize, file);
        auto* tmp = new ListNode{nullptr, nullptr, nullptr, text};
        PushBack(tmp);
        mp.insert( std::make_pair( tmp, std::make_pair(ptr, rPtr) ) );
    }

    for (auto& it : mp)
        if (it.second.second)
            for (auto& it2 : mp)
                if (it2.second.first == it.second.second)
                    it.first->rand = it2.first;
}
