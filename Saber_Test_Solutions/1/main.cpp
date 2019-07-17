#include <iostream>

void to_binary(int n)
{
    if (n == 0)
    {
        printf("0\n");
        return;
    }

    int extraZeros = 1;
    unsigned bitMask = 1 << (sizeof(n) * 8 - 1);

    while (bitMask)
    {
        unsigned char bitCur = (n & bitMask) ? 1 : 0;

        if (extraZeros && bitCur)
            extraZeros = 0;

        if (!extraZeros)
            printf("%d", bitCur);

        bitMask >>= 1;
    }
    printf("\n");
}


int main()
{
    to_binary(0);
    to_binary(1);
    to_binary(137);
    to_binary(-1);
    to_binary(-137);
    return 0;
}