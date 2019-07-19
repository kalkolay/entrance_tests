#include <iostream>
#include <unordered_map>

struct ListNode {
    ListNode *      prev;
    ListNode *      next;
    ListNode *      rand;  // указатель на произвольный элемент данного списка либо NULL
    std::string     data;
};

class List {
public:
    void Serialize(FILE* file)  // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    {
        std::unordered_map<ListNode*, int> ptrToId;

        int idCount = 0;
        for (ListNode* left = head; left; left = left->next)
            ptrToId.insert({left, idCount++});

        for (ListNode* left = head; left; left = left->next)
        {
            fwrite(&ptrToId[left->rand], sizeof(ptrToId[left->rand]), 1, file);
            fputs("\n", file);
            fwrite(&(left->data), sizeof(left->data), 1, file);
            fputs("\n", file);
        }
    }

    void Deserialize(FILE* file)  // загрузка из файла (файл открыт с помощью fopen(path, "rb"))
    {
        std::unordered_map<int, ListNode*> idToPtr;

        int idCount = 0;
        for (ListNode* left = head; left; left = left->next)
            idToPtr.insert({idCount++, left,});

        int id;
        for (ListNode* left = head; left; left = left->next)
        {
            fread(&id, sizeof(id), 1, file);
            fread(&(left->data), sizeof(left->data), 1, file);

            left->rand = idToPtr[id];
        }
    }

    /// For testing
    void addNode()
    {
        std::string s = "data";
        ListNode* newNode = new ListNode;
        newNode->data = s;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = nullptr;
        ListNode* cur = head;
        for (int i = 0; i < rand() % count; i++)
            cur = cur->next;
        newNode->rand = cur;
        tail = newNode;
        count++;
    }

    List()
    {
        head = new ListNode;
        head->next = nullptr;
        head->prev = nullptr;
        head->data = "data";
        head->rand = nullptr;
        tail = head;
        count = 1;
    }

    ~List() = default;

private:
    ListNode *      head;
    ListNode *      tail;
    int       count;
};


int main()
{
    List myList;
    myList.addNode();
    myList.addNode();
    FILE* file = fopen("test.bin", "wb");
    myList.Serialize(file);
    std::cout << "Serialization completed.\n";
    file = fopen("test.bin", "rb");
    myList.Deserialize(file);
    std::cout << "Deserialization completed.\n";
    return 0;
}