#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// An array of prime numbers
typedef struct PrimesArray
{
    uint64_t length;    // Amount of elements
    uint64_t primes[];  // Elements
} PrimesArray;

// Determine if a given positive integer is prime
static bool is_prime(uint64_t value)
{
    // Special case: value up to 3
    if (value == 0 || value == 1) return false;
    if (value == 2 || value == 3) return true;
    
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

static inline bool sieve_get(uint8_t *sieve, size_t byte, uint8_t bit)
{
    return sieve[byte] & (1 << bit);
}

static inline void sieve_set(uint8_t *sieve, size_t byte, uint8_t bit)
{
    sieve[byte] |= (1 << bit);
}

// Returns an array of prime numbers whithin a range
static PrimesArray *primes_range(uint64_t start, uint64_t end)
{
    // Swap the values when "end" is smaller than "start"
    if (end < start)
    {
        start ^= end;
        end ^= start;
        start ^= end;
    }

    const uint64_t range_len = end - start;
    size_t buffer_size = range_len / 8UL;
    if (range_len % 8 != 0) buffer_size += 1UL;

    // Sieve of Erastothenes
    // This is an array of bitmasks to flag if a number is not prime
    // Each bit represents a number, so a byte represent 8 numbers.
    // If a bit is set, then the corresponding number is not prime.
    uint8_t *sieve = (uint8_t*)calloc(buffer_size, sizeof(uint8_t));

    // Iterate over all values in the range
    for (uint64_t value = start; value <= end; value++)
    {
        // Calculate the position of the value on the sieve
        const uint64_t offset = value - start;
        const size_t byte = offset / 8UL;
        const uint8_t bit = offset % 8UL;

        // Check if the current value was already flagged as not being prime
        if (sieve_get(sieve, byte, bit)) continue;

        // Check if the value is prime
        if (is_prime(value))
        {
            // If the value is prime, flag its multiples as not being prime
            for (uint64_t multiple = value * 2; multiple <= end; multiple += value)
            {
                const uint64_t offset = multiple - start;
                const size_t byte = offset / 8UL;
                const uint8_t bit = offset % 8UL;
                sieve_set(sieve, byte, bit);
            }
        }
    }
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