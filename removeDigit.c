#include <stdio.h>
#include <string.h>
#include <math.h>

/* Detect whether the specific digit given is contained in the
2 numbers chosen for A and B */
int findNumber(char digit, char *number)
{
    int count = 1;
    int i;

    for (i = 0; i < strlen(number); i++)
    {
        // If the number is present in number passed in
        if (number[i] == digit)
        {
            count = 0;
            break;
        }
    }

    return count;
}

void removeDigit(int x, int n, int* a_ptr, int* b_ptr) {
    char strConvert[10];
    char specificDigit = x + '0';
    int a;
    // Validate data
    if ((x <= 9 && x >= 0) && (n >= 10 && n <= pow(10, 8)))
    {
        for (a = 1; a < n; a++)
        {
            /* Converts integer into string then passes into
            findNumber function */
            sprintf(strConvert, "%d", a);
            if (findNumber(specificDigit, strConvert))
            {
                sprintf(strConvert, "%d", n - a);
                if (findNumber(specificDigit, strConvert))
                {
                    *a_ptr = a;
                    *b_ptr = n - a;
                    break;
                }
            }
        }
    }
}