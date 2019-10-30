README
//written by phantomx999

Base Number Arithmetic

Allows addition and subtraction with different base numbers
(from base 2 to base 36)

for example:
./bignum_math 4 12 + 13
Result: 31

Here two numbers of 12 and 13 both in base 4 are added and the result is given

If there is invalid input (base is less than 2 or greater than 36, or numbers are outside
of base such as base 2 and numbers are 13 - 11, where 13 is not valid value for base 2,
then output is invalid input with standard usage prompt)

capital and lowercase letters as input are treated the same i.e. a = A in base 16 
for example

negative number input is not handled

DESIGN:
bignum_math.c
bignum_math.h

TO RUN:

compile first
>  gcc bignum_math.c -o bignum_math

run
> ./bignum_math base number1 operation number2

example:
  ./bignum_math 16 1A - CB50B
