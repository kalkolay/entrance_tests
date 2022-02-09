#include <iostream>

/// Task 1

unsigned long long iterativeFactorial(unsigned int n)
{
    unsigned long long result = 1;
    while (n)
        result *= n--;
    return result;
}

unsigned long long recursiveFactorial(unsigned int n)
{
    return n < 2 ? 1 : n * recursiveFactorial(n - 1);
}

/**
 *  Теоретически, итеративная функция факториала будет работать быстрее, чем рекурсивная, так как
 *  при вызове рекурсивной функции её параметры копируются в стек, а при завершении вызова предыдущие
 *  значения параметров вытягиваются обратно из стека, что приводит к лишним операциям.
 *  На практике же у большинства современных компиляторов в данном случае сработает Tail Call Optimization,
 *  которая по сути оптимизирует рекурсивный алгоритм вычисления факториала в аналогичный итеративный.
 */


int main()
{
    std::cout << "====== Task 1 ======" << std::endl;

    std::cout << "Iterative factorial:\n";
    for (unsigned int i = 0; i < 6; ++i)
        std::cout << "F(" << i << ") = " << iterativeFactorial(i) << std::endl;
    std::cout << "Recursive factorial:\n";
    for (unsigned int i = 0; i < 6; ++i)
        std::cout << "F(" << i << ") = " << recursiveFactorial(i) << std::endl;

    return 0;
}
