#include <iostream>
#include <fstream>
#include <vector>

struct ListNode {
    ListNode *      prev;
    ListNode *      next;
    ListNode *      rand;  // указатель на произвольный элемент данного списка либо NULL
    std::string     data;

    int        id;
};

class List {
public:
    void Serialize(std::ostream& output)
    {
        int id = 0;
        for (ListNode* left = head; left; left = left->next)
            left->id = id++;

        output << count << std::endl;

        for (ListNode* left = head; left; left = left->next)
        {
            if (left->rand != nullptr)
                output << left->rand->id << std::endl;
            else
                output << "NULL"         << std::endl;
            output << left->data     << std::endl;
        }
    }

    void Deserialize(std::istream& input)
    {
        int streamCount;
        input >> streamCount;

        std::vector<ListNode*> vec;
        vec.reserve(streamCount);

        for (int i = 0; i < streamCount; i++)
        {
            ListNode* oldTail = tail;
            tail = new ListNode;
            tail->prev = oldTail;
            tail->next = nullptr;

            !head ? head = tail : oldTail->next = tail;
            vec.push_back(tail);
        }

        std::string id;
        for (int i = 0; i < streamCount; i++)
        {
            input >> id;
            input >> vec[i]->data;
            if (id != "NULL")
                vec[i]->rand = vec[std::stoi(id)];
            else
                vec[i]->rand = nullptr;
            vec[i]->id = i;
        }

        count += streamCount;
    }

    /// For testing
    void addNode()
    {
        std::string s = "data";
        ListNode* newNode = new ListNode;
        newNode->data = s;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->id   = count;
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
        head->id   = 0;
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
    std::ofstream out("test.txt");
    myList.Serialize(out);
    std::cout << "Serialization completed.\n";
    std::ifstream in("test.txt");
    myList.Deserialize(in);
    std::cout << "Deserialization completed.\n";
    return 0;
}
