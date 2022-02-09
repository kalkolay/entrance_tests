#pragma once

/// Task 2

struct Node
{
    struct Node* next;
    int val;
};

void ReverseAfter( struct Node* head, int val );
