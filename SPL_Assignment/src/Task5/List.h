#pragma once

#include <iostream>
#include <string>

/// Task 5

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка либо NULL
    std::string data;
};

class List {
public:
    void Serialize   (FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize (FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

    ~List();

    void PushBack  (ListNode* node);

    ListNode* GetHead ()       { return head;  }
    ListNode* GetTail ()       { return tail;  }
    int32_t GetCount  () const { return count; }

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count      = 0;
};
