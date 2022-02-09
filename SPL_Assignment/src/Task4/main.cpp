#include <iostream>

/// Task 4

void RemoveDups( char *pStr )
{
    char* ptrIn = pStr;
    char* ptrOut = pStr;
    char prevChar = '\0';

    while (*ptrIn != '\0')
    {
        if (*ptrIn == prevChar)
        {
            prevChar = *ptrIn;
            ++ptrIn;
            continue;
        }

        *ptrOut = *ptrIn;
        prevChar = *ptrIn;
        ++ptrOut;
        ++ptrIn;
    }

    *ptrOut = '\0';
}


int main()
{
    std::cout << "====== Task 4 ======" << std::endl;

    const char pString[] = "AAA BBB AAA";
    std::cout << "Before remove:\t" << pString << std::endl;
    RemoveDups( const_cast<char*>(pString) );
    std::cout << "After remove:\t" << pString << std::endl;

    return 0;
}
