#include <iostream>

/// Task 3

// Function to copy 'nBytes' of data from src to dst.
//void myMemcpy(char* dst, const char* src, int nBytes)
//{
//    // Try to be fast and copy a word at a time instead of byte by byte
//    int* wordDst = (int*)dst;
//    int* wordSrc = (int*)src;
//    int numWords = nBytes >> 2;
//    for (int i = 0; i < numWords; i++)
//    {
//        *wordDst++ = *wordSrc++;
//    }
//
//    int numRemaining = nBytes - (numWords << 2);
//    dst = (char*)wordDst;
//    src = (char*)wordSrc;
//    for (int i = 0; i <= numRemaining; i++);
//    {
//        *dst++ = *src++;
//    }
//}

/// Task 3 : Solution

// Function to copy 'nBytes' of data from src to dst.
void myMemcpy(char* dst, const char* src, int nBytes)
{
    // Check input data
    if (!dst || strlen(src) < nBytes)
    {
        std::cerr << "Incorrect input: strlen(src)=" << strlen(src)
                  << " and strlen(dst)=" << strlen(dst) << std::endl;
        return;
    }

    // Address of a char might not be acceptable for int alignment
    int alignment = 0;
    int charSize = sizeof(char);
    while ( (charSize *= 2) <= sizeof(int) )
    {
        ++alignment;
    }

    // Try to be fast and copy a word at a time instead of byte by byte
    int* wordDst = (int*)dst;
    int* wordSrc = (int*)src;
    int numWords = nBytes >> alignment;
    for (int i = 0; i < numWords; i++)
    {
        *wordDst++ = *wordSrc++;
    }

    int numRemaining = nBytes - (numWords << alignment);
    dst = (char*)wordDst;
    src = (char*)wordSrc;
    for (int i = 0; i < numRemaining; i++)
    {
        *dst++ = *src++;
    }
}

/**
 *  Что было исправлено:
 *  - [Syntax]      Убрана точка с запятой в последнем цикле (это предотвращает пустое тело цикла);
 *  - [Algorithm]   Убрано нестрогое неравенство в последнем цикле (нам не нужен крайний случай);
 *  - [Algorithm]   Добавлена проверка на корректность входных данных;
 *  - [Portability] Добавлена поддержка не 32-битного int для других систем;
 *  - [Portability] Исправлены возможные проблемы с выравниванием для char и int на других системах.
 */


int main()
{
    std::cout << "====== Task 3 ======" << std::endl;

    char* dst = new char[42];
    const char* src = "Hello, World!";
    int nBytes = 5;

    myMemcpy(dst, src, nBytes);
    std::cout << "src = \"" << src << "\"; strlen = " << strlen(src) << std::endl;
    std::cout << "dst = \"" << dst << "\"; strlen = " << strlen(dst) << std::endl;

    delete [] dst;
    return 0;
}
