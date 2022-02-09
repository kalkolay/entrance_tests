#include <iostream>

#include "ReverseAfter.h"

/// Task 2 : Solution

void Reverse( Node** head )
{
    if (!head || !*head)
        return;

    Node* list = *head;
    Node* newList = nullptr;
    while (list)
    {
        Node* tmp = list;
        list = list->next;
        tmp->next = newList;
        newList = tmp;
    }
    *head = newList;
}

void ReverseAfter( struct Node* head, int val )
{
    if (!head)
        return;

    while (head->next && head->next->val != val)
        head = head->next;
    Reverse(&head->next);
}

void Print( Node* head )
{
    while (head)
    {
        std::cout << head->val << ' ';
        head = head->next;
    }
    std::cout << std::endl;
}


int main()
{
    std::cout << "====== Task 2 ======" << std::endl;

    Node n[6] =
    {
        {nullptr, 1},
        {nullptr, 2},
        {nullptr, 3},
        {nullptr, 4},
        {nullptr, 5},
        {nullptr, 6}
    };
    n[0].next = &n[1];
    n[1].next = &n[2];
    n[2].next = &n[3];
    n[3].next = &n[4];
    n[4].next = &n[5];

    Node* head = &n[0];
    std::cout << "Before reverse:\t";
    Print(head);

    ReverseAfter(head, 4);
    std::cout << "After reverse:\t";
    Print(head);

    return 0;
}
