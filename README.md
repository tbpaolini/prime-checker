## Prime checker

This is a simple command line program for checking whether a number is prime, or getting all primes within a range.

If the program gets a single argument, then it checks if the value is prime:

```shell
./primes 1000003
Is prime
```

If the program gets two arguments, then it prints all primes between the two values:

```shell
./primes 1001000 1001100
1001003 1001017 1001023 1001027 1001041 1001069 1001081 1001087 1001089 1001093
```

For compiling the program, just run `make` in the root directory of the project. If you are on Windows using MinGW, you might need to run `mingw32-make` instead.

Or you can just download the compiled binaries:

* [Executable for Windows](https://github.com/tbpaolini/prime-checker/releases/download/v1.0.0/primes.exe)
* [Executable for Linux](https://github.com/tbpaolini/prime-checker/releases/download/v1.0.0/primes)

## Algorithm

In order to check if a number is prime, I tool advantage of the following properties

* The biggest prime factor of a positive integer, excluding itself, is smaller or equal than its square root.
* With the exception of 2 and 3, all numbers that are prime can be written in the form of `6n ± 1` (with `n` being a positive integer).

Those properties allowed to greatly reduce the amount of divisors to try against the input.

In order to generate a range of prime numbes, I used the [Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes). The sieve initially contains all values in the range, which are tested sequentially. When a value is tested, its multiples are removed from the sieve and not tested (because they certainly cannot be primes).