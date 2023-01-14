#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

static bool is_prime(uint64_t value)
{
    // Special cases:
    //  - Divisible by 2 or 3
    //  - Value can not be written in the form of 6n ± 1 (for n integer)
    if ( value % 2 == 0 || value % 3 == 0 || ( (value-1) % 6 != 0 && (value+1) % 6 != 0 ) )
    {
        // In those cases, the value cannot be prime
        return false;
    }

    // Maximum divisor to be tested
    uint64_t max_div = ceil( sqrt(value) );

    // Test the divisors in the form of 6n ± 1
    for (size_t i = 7; i <= max_div; i += 6)
    {
        if (value % (i - 2) == 0) return false;
        if (value % i == 0) return false;
    }
    
    return true;
}

static bool validate_input(char *input)
{
    const size_t length = strlen(input);
    for (size_t i = 0; i < length; i++)
    {
        if (!isdigit(input[i]) && input[i] != ' ') return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    switch (argc)
    {
        case 2:
            if (!validate_input(argv[1]))
            {
                fprintf(stderr, "Error: %s is not a positive integer\n", argv[1]);
                break;
            }

            const uint64_t value = atol(argv[1]);
            
            if (is_prime(value))
            {
                printf("Is prime\n");
            }
            else
            {
                printf("Not prime\n");
            }
            
            break;
        
        case 3:
            break;
    }
}