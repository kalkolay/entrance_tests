#include <iostream>

void RemoveDups(char* pStr)
{
    char* pivot = pStr;
    const char* cur = pStr;

    while (*cur)
    {
        if (pivot != cur)
            *pivot = *cur;
        *pivot++;

        while ( !(*cur ^ *(cur + 1)) )
            *cur++;
        *cur++;
    }
    *pivot = 0;
}


int main()
{
    char str[] = "AAA BBB AAA";
    RemoveDups(str);
    std::cout << str << std::endl;
    return 0;
}