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

// Position of a bit in an array of bytes
typedef struct BitPos
{
    size_t byte;    // Index of the byte on the array
    uint8_t bit;    // Index of the bit on the byte
} BitPos;

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

// Get the position of a bit in an array of bytes
static inline BitPos bit_get_pos(uint64_t value, uint64_t offset)
{
    const uint64_t index = value - offset;
    const size_t byte = index / 8UL;
    const uint8_t bit = index % 8UL;
    return (BitPos){byte, bit};
}

// Get whether a flag on the Sieve of Eratosthenes is set
static inline bool sieve_get(uint8_t *sieve, size_t byte, uint8_t bit)
{
    return sieve[byte] & (1 << bit);
}

// Set a flag on the Sieve of Eratosthenes
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

    // Calculate the buffer size for storing which numbers in the range are prime
    const uint64_t range_len = end - start;
    size_t buffer_size = (range_len / 8UL) + 1UL;

    // Sieve of Eratosthenes
    // This is an array of bitmasks to flag if a number is not prime
    // Each bit represents a number, so a byte represent 8 numbers.
    // If a bit is set, then the corresponding number is not prime.
    uint8_t *restrict sieve = (uint8_t*)calloc(buffer_size, sizeof(uint8_t));

    if (!sieve)
    {
        fprintf(stderr, "Error: No enough memory\n");
        abort();
    }
    
    BitPos pos; // Position of the bit on the sieve
    uint64_t prime_count = 0;   // Count how many numbers are prime

    // Iterate over all values in the range
    for (uint64_t value = start; value <= end; value++)
    {
        // Calculate the position of the value on the sieve
        pos = bit_get_pos(value, start);

        // Check if the current value was already flagged as not being prime
        if (sieve_get(sieve, pos.byte, pos.bit)) continue;

        // Check if the value is prime
        if (is_prime(value))
        {
            // Increment the prime counter
            prime_count++;          
        }
        else
        {
            // Flag the value as not being prime
            pos = bit_get_pos(value, start);
            sieve_set(sieve, pos.byte, pos.bit);
        }

        // Special case: value is 0 or 1
        if (value <= 1) continue;

        // Flag the value's multiples as not being prime
        // (except if the value is 0 or 1)
        for (uint64_t multiple = value * 2; multiple <= end; multiple += value)
        {
            pos = bit_get_pos(multiple, start);
            sieve_set(sieve, pos.byte, pos.bit);
        }
    }

    // Alocate array to store the prime numbers in the range
    PrimesArray *output = (PrimesArray*)malloc( sizeof(PrimesArray) + (prime_count * sizeof(uint64_t)) );

    if (!output)
    {
        fprintf(stderr, "Error: No enough memory\n");
        abort();
    }

    output->length = prime_count;
    size_t prime_id = 0;

    // Populate the array
    for (uint64_t value = start; value <= end; value++)
    {
        pos = bit_get_pos(value, start);
        if (!sieve_get(sieve, pos.byte, pos.bit))
        {
            output->primes[prime_id++] = value;
        }
    }

    // The sieve is no longer needed
    free(sieve);

    // Return a pointer to the array
    return output;
}

// Check if a string only contains digits
static void validate_input(char *input)
{
    const size_t length = strlen(input);
    for (size_t i = 0; i < length; i++)
    {
        if (!isdigit(input[i]))
        {
            fprintf(stderr, "Error: '%s' is not a positive integer or zero\n", input);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv)
{
    switch (argc)
    {
        // If the program got only one argument, check if the argument is prime
        case 2:
            // Check if the argument is valid and convert it to integer
            validate_input(argv[1]);
            const uint64_t value = atol(argv[1]);
            
            // Check if the value is prime
            if (is_prime(value))
            {
                printf("Is prime\n");
            }
            else
            {
                printf("Not prime\n");
            }
            
            break;
        
        // If the program got two arguments, print the primes in the range between the arguments
        case 3:
            // Check if both arguments are positive integers or zero
            validate_input(argv[1]);
            validate_input(argv[2]);

            // Convert the range to integers
            const uint64_t start = atol(argv[1]);
            const uint64_t end = atol(argv[2]);

            // Get the primes in the range
            PrimesArray *restrict output = primes_range(start, end);

            // Print the primes
            for (size_t i = 0; i < output->length; i++)
            {
                printf("%lu ", output->primes[i]);
            }
            printf("\n");

            free(output);

            break;
    }

    return 0;
}