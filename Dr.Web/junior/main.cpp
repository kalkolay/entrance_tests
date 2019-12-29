#include <iostream>
#include <cstring>
#include <chrono>

/* Write a function with given signature that efficiently trims all whitespaces in the end of C-like string. */

void TrimRight( char *s )
{
    if (s != nullptr)
    {
        char *ptr = s + strlen(s);
        while ( *(--ptr) == ' ' );
        *(++ptr) = '\0';
    }
    else
        throw std::invalid_argument("null pointer exception");
}


int main()
{
	using ms = std::chrono::milliseconds;
    char somestr[] = "abc                                        ";
    std::cout << "FIRST: " << somestr << "END" << std::endl;
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 1e8; ++i)
    {
        try
        {
            TrimRight(somestr);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    std::cout << "SECOND: " << somestr << "END" << std::endl;
    auto end = std::chrono::steady_clock::now();
    auto timeElapsed = end - start;
    std::cout << "Calculation time: " << std::chrono::duration_cast<ms>(timeElapsed).count() << "ms" << std::endl;

    char *errstr = nullptr;  // calling TrimRight() of this string would return "null pointer exception"!
    try
    {
        TrimRight(errstr);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}