
/**************************
 *
 * orginially written by Andy Exley
 * modified by Emery Mizero
 * valid_input(), string_to_integer_array(), bignum_print(), reverse(), subtract(),
 * and part of perform_math() written by phantomx999
 **************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum_math.h"

int* string_to_integer_array(char* str);	//some prototypes
void bignum_print(int* num);				//   ""
int bignum_length(int* num);				//   ""

/*
 * Returns true if the given char is a digit from 0 to 9
 */
bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

/*
 * Returns true if lower alphabetic character
 */
bool is_lower_alphabetic(char c) {
	return c >= 'a' && c <= 'z';
}

/*
 * Returns true if upper alphabetic character
 */
bool is_upper_alphabetic(char c) {
	return c >= 'A' && c <= 'Z';
}

/*
 * Convert a string to an integer
 * returns 0 if it cannot be converted.
 */
int string_to_integer(char* input) {
	int result = 0;
	int length = strlen(input);
    int num_digits = length;
	int sign = 1;
	
	int i = 0;
    int factor = 1;

    if (input[0] == '-') {
		num_digits--;
		sign = -1;
    }

	for (i = 0; i < num_digits; i++, length--) {
		if (!is_digit(input[length-1])) {
			return 0;
		}
		if (i > 0) factor*=10;
		result += (input[length-1] - '0') * factor;
	}

    return sign * result;
}

/*
 * Returns true if the given base is valid.
 * that is: integers between 2 and 36
 */
bool valid_base(int base) {
	if(!(base >= 2 && base <= 36)) { 
		return false; 
	}
	return true;
}

/*
 * Returns true if the given string (char array) is a valid input,
 * that is: digits 0-9, letters A-Z, a-z
 * and it should not violate the given base and should not handle negative numbers
 */
bool valid_input(char* input, int base) {
	int i = 0;
	if (input == NULL) { return false;}		//checks for weird user input such as no input for argv[i].....
	if (!valid_base(base)) { return false; }	//main() already checks for this, but good to have anyways
	int* num = string_to_integer_array(input);	//converts argv[i] string to integer array
	int length = strlen(input);					//gets length of integer array
	
	if (num[i] < 0 || length < 1) { return false;}	//checks if integer array is negative, or more bizarre input from user like len < 1
	
	
	//checks if each array element is not a digit character or alphabetic character
	while (i < length){	
		if(!is_digit(input[i]) && !is_lower_alphabetic(input[i]) && !is_upper_alphabetic(input[i])){
			return false;
		}
		i++;
	}
	
	i = 0;	//reset to 0
	while(i < length){	//now check if each character value is within given base
		if(num[i] >= 0 && num[i] < base){
			i++;
		}
		else{
			return false;
		}
	}
	
	//if elements are within base & all elements are digits/alphabetic characters, return true....
	return true;	
}

/*
 * converts from an array of characters (string) to an array of integers
 */
int* string_to_integer_array(char* str) {
	int* result;
	int i, str_offset = 0;
		result = malloc((strlen(str) + 1) * sizeof(int));
		result[strlen(str)] = -1;
	for(i = str_offset; str[i] != '\0'; i++) {
		if(is_digit(str[i])) { 
			result[i - str_offset] = str[i] - '0';
		} else if (is_lower_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'a' + 10;
		} else if (is_upper_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'A' + 10;
		} else {
			printf("error!\n");
		}
	}
	return result;
}

/*
 * finds the length of a bignum... 
 * simply traverses the bignum until a negative number is found.
 */
int bignum_length(int* num) {
	int len = 0;
	while(num[len] >= 0) { len++; }
	return len;
}

/*
 * Prints out a bignum using digits and lower-case characters
 * Expected behavior: prints characters
 */
void bignum_print(int* num) {
	int i;	
	
	if(num == NULL) { return; }	//if no values to print, accounts for this possible error
	int first_ind = 0;	
	
	/* Handle negative numbers */
	i = bignum_length(num);			
	int len = bignum_length(num);	//length of array to print, same thing as i

	if (len > 1  && num[first_ind] == 0){	//first, check for leading 0's, i.e. 00    
		while(num[first_ind] == 0){		//moves index past leading zeros
			first_ind++;
		}
		if(num[first_ind - 1] == 0 && num[first_ind] == -1){	//if array = {0, 0, 0, -1}, then we have to move back...
			first_ind--;										//...1 index to the last 0 for the upcoming 'for' loop
		}
	}
	for(int j = first_ind; num[j] >= 0; j++) {	//prints array characters until reaches '-1'
		if (num[j] >= 0 && num[j] <= 9){	//ASCII code table, prints out characters '0' to '9'
			printf("%c", num[j] + '0');
		}
		else if(num[j]== 45){		//prints out negative symbol '-'
			printf("%c", num[j]);
		}
		else{
			printf("%c", num[j] + 'W');	//prints out alphabet characters (lowercase)
		}
	}
	
	printf("\n");	
}



/*
 *	Helper for reversing the result that we built backward.
 *  see add(...) below
 */
void reverse(int* num) {
	int i, len = bignum_length(num);
	for(i = 0; i < len/2; i++) {
		int temp = num[i];
		num[i] = num[len-i-1];
		num[len-i-1] = temp;
	}
}


/*
 * used to add two numbers with the same sign
 */
int* add(int* input1, int* input2, int base) {		
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2) + 2;
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int r = 0;
	int carry = 0;
	int sign = input1[len1];
    int num1, num2;

	len1--;
	len2--;

	while (len1 >= 0 || len2 >= 0) {
        if (len1 >= 0) {
            num1 = input1[len1];
        } else {
            num1 = 0;
        }

        if (len2 >= 0) {
            num2 = input2[len2];
        } else {
            num2 = 0;
        }
		result[r] = (num1 + num2 + carry) % base;
		carry = (num1 + num2 + carry) / base;
		len1--;
		len2--;
		r++;
	}
	if (carry > 0) {
        result[r] = carry; 
        r++; 
    }
	result[r] = sign;
	reverse(result);
	return result;
}


/*
 subtract number2 from number1 of user input base
 */
 
int* subtract(int* input1, int* input2, int base){			//a separate subtraction function
	int len1 = bignum_length(input1);						//length of 1st array
	int len2 = bignum_length(input2);						//length of 2nd array
	int resultlength = ((len1 > len2)? len1 : len2) + 2;	//finds length of result array	
	int* result;											//answer goes in this array
	int positive_answer = true;								//boolean to find if answer >= 0
	
	
	if(len1 > len2){	//for input1 > input2, subtraction yields a positive answer
		positive_answer = true;
	}
	else if(len2 > len1){	//for input2 > input1 since input2 has longer length, subtraction yields negative answer
		positive_answer = false;
	}
	else{	//input1 and input2 are = lengths, so first compare if input2[i] > input1[i].  If not, simply subtract and get positive number
		int ind = 0;	//index
		int greater_than = false;	// is > ? 
		int less_than = false;		// < ?
		
		while (ind < len1 && greater_than == false && less_than == false){		//traverses/compares arrays to see which one is smaller/larger	
				if (input1[ind] > input2[ind] && less_than == false){			//if 1st array is larger, positive answer
					greater_than = true;
				}
				else if (input1[ind] < input2[ind]){							//if 2nd array is larger, negative answer
					less_than = true;
				}
				ind++;
		}
		
		if (less_than == true){		
			positive_answer = false;
		}	
	}
	
	int sign = input1[len1];		//puts -1 at end of array
	len1--;							//start at end of arrays, decrement down to 0th index
	len2--;
	int num1 = 0;					//current element in 1st array
	int num2 = 0;					//  ""    ""      in 2nd array
	int index = 0;					
	
	if(positive_answer == false){	//for negative answers
		result = (int*) malloc (sizeof(int) * resultlength); //will need length of larger array, plus -1, plus '-' negative sign (hence resultlength == lenX + 2)
	}
	else{	//positive answer
		result = (int*) malloc (sizeof(int) * (resultlength -1));	//need length of larger array, plus -1, so 1 index smaller since we dont need '-' negative sign
	}
	
	if(positive_answer == true){		//performs subtraction for 1st array > 2nd array
		while(len1 >= 0){				//continues until we go through all of 1st array
			num1 = input1[len1];		//store current element to be subtracted
			if (len2 >= 0){				//traverses 2nd array
				num2 = input2[len2];	//stores current element that subtracts
			}						
			else{						//if we already went through all of 2nd array, assign 0's for remainder values
				num2 = 0;
			}
			
			if(num1 >= num2){					//i.e. 8-5, 8-8...just subtract normally
				result[index] = num1 - num2;	//put it in result array	
			}
			else if (num1 < num2){				//i.e. 4-5, 3-8...have to carry over 
				int j =1;						
				while(input1[len1 - j] == 0){	//goes to next digit value, continues to go to higher digit values until > 0
					j++;
				}
				input1[len1 - j]--;							//subtracts 1 from this digit value to be carried down
				j--;	
												//now goes back down back to original index
				while(j != 0){	//continues to traverse back to original index
					if(input1[len1 - j] == 0){				//for any digit values along the way == 0, add base then minus 1
						input1[len1 - j] = base - 1;
					}
					j--;									
				} 
				result[index] = num1 + base - num2;			//once we arrive at original index, value has been carried down 
															//so we add base and subtract stored 2nd array element
			}												//example: 1001 - 999 = 002 = 2  
															//(leading 0's dealt with in bignum_print)
															//put value in result array
			index++;		//go to next result index		
			len1--;			//go down arrays until < 0 index
			len2--;	
		}
	}
	else{						//performs subtraction when 1st array < 2nd array....negative answer
		index = 0;
		while(len2 >= 0){			//same as earlier subtract algorithm, but flips array2 and array 1
			num2 = input2[len2];		//i.e. a1 = 999, a2 = 1000, subtract a2 - a1 = 1 and then negate it to = -1
			if(len1 >= 0){
				num1 = input1[len1];
			}
			else{
				num1 = 0;
			}
			
			if(num2 >= num1){
				result[index] = num2 - num1;
			}
			else if (num2 < num1){
				int j =1;
				while(input2[len2 - j] == 0){				
					j++;									
				}
				input2[len2 - j]--;
				j--;
				while(j != 0){
					if(input2[len2 - j] == 0){
						input2[len2 - j] = base - 1;
					}
					j--;
				} 
				result[index] = num2 + base - num1;
			}
			index++;
			len1--;
			len2--;	
		}
		result[index] = 45;		//45 = '-' character in ASCII, put this at end of array before reversing
		index++;
	}
	
	result[index] = sign; 	//append -1 to answer array
	reverse(result);		//reverse order answer
	
	if(result[0] == 45){		//for special case scenario: i.e. answer = -002, here you have to get rid of leading zeros
		if(result[1] == 0){		//yet you still want to keep '-' sign to keep it negative answer
			int count_zeros = 1;				//checks if answer is negative, then checks for leading 0's right after
			while(result[count_zeros] == 0){	//counts number of leading 0's and moves to first non zero element
				count_zeros++;
			}
			count_zeros--;						//one too many leading 0's counted, go back one
			int temps_length = bignum_length(result) - count_zeros + 1;	//length of new array	
			
			int* temps = (int*) malloc(sizeof(int) * temps_length);	//allocate memory for new temp array = answer - leading 0 count + -1 index							
			temps[0] = result[0]; //put in negative sign first
			int i = 1;
			int j = count_zeros + 1;
			while(result[j] >= 0){ 	//go through rest of new array
				temps[i] = result[j];	//set new array to answer array  minus any leading 0's
				i++;
				j++; 
			}
			temps[i] = -1;	//end temp array with -1
			
			realloc(result,temps_length);	//allocate memory of answer array to the temp array
			
			for(int x = 0; x < temps_length; x++){	//copy elements from temp array to answer array
				result[x] = temps[x];
			}
		}
	}
	
	return result;	//subtraction answer array returned

} 
 
int* perform_math(int* input1, int* input2, char op, int base) {

	/* 
	 * this code initializes result to be large enough to hold all necessary digits.
     */	

	//first check for leading 0's for input1
	
	int z = 0;			//indexes
	int y = 0;
	int* temp_array1;	//temporary arrays for input1, 2
	int* temp_array2;
		
	if(input1[0] == 0){			//if leading 0 input
		while(input1[z] == 0){	//go past leading 0's, i.e. 0000...4     ....until > 0 or -1 is reached
			z++;
		}
		if(input1[z] > 0){		//not 0 for value, i.e. 000023...  2 here is reached
			y = z;				//store current index value
			int count = 1;		//counts length of temp array
			while(input1[z] >= 0){	//find length of temp_array1
				z++;
				count++;
			} 
			int x = 0;													//temp index x
			temp_array1 = (int*) malloc (sizeof(int) * count);			//allocate based on array length minus leading zeros
			while(input1[y] >= 0){										//traverses rest of array past leading 0's
				temp_array1[x] = input1[y];								//assigns temp array remainder of actual array
				x++;
				y++;
			}
			temp_array1[x] = -1; 										//appends -1 to temp array
		//	realloc(input1, count);
			int r = 0;
			for(r = 0; r < count; r++){
				input1[r] = temp_array1[r];
			}		
		}
		else if ((input1[z - 1] == 0) && (input1[z] == -1)){		// when value is 0 but has leading 0's, i.e. 0000 = 0
			temp_array1 = (int*) malloc (sizeof(int) * 2);			//temp array = {0, -1} , copy input array to this array
			temp_array1[0] = 0;										
			temp_array1[1] = -1;
		//	realloc(input1, 2);
			int r = 0;
			for(r = 0; r < 2; r++){				//copy input array to have temp elements
				input1[r] = temp_array1[r];
			}
		}
	}
	
	//now check for leading 0's for input2, same exact algorithms...
	
	if (input2[0] == 0){ 
		int z = 0;
		int y = 0;
		while(input2[z] == 0){	//get rid of leading 0's, i.e. 0000...4
			z++;
		}
		if(input2[z] > 0){		//not 0 for value, i.e. 000023...
			y = z;
			int count = 1;
			while(input2[z] >= 0){	//find length of temp_array2
				z++;
				count++;
			} 
			int x = 0;
			temp_array2 = (int*) malloc (sizeof(int) * count);
			while(input2[y] >= 0){
				temp_array2[x] = input2[y];
				x++;
				y++;
			}
			temp_array2[x] = -1; 		//appends -1 to temp array
		//	realloc(input2, count);
			int r = 0;
			for(r = 0; r < count; r++){
				input2[r] = temp_array2[r];
			}
		}
		else if ((input2[z - 1] == 0) && (input2[z] == -1)){		//value is 0, i.e. 0000 = 0
			temp_array2 = (int*) malloc (sizeof(int) * 2);
			temp_array2[0] = 0;
			temp_array2[1] = -1;	
		//	realloc(input2, 2);
			int r = 0;
			for(r = 0; r < 2; r++){
				input2[r] = temp_array2[r];
			}
		}
	}

    int len1 = bignum_length(input1);		//lengths of 1 and 2 arrays
    int len2 = bignum_length(input2);
    int resultlength = 0;					//initialize result array length
    int* result;							//answer array
    
    if (op != '>' && op != '<' && op != '='){				//when operator is either + or - only
		resultlength = ((len1 > len2)? len1 : len2) + 2;	//result length must be answer array plus -1 index, plus carry over from addition or possibly '-' negative sign from subtraction
		result = (int*) malloc (sizeof(int) * resultlength); //allocate this size memory
	}
	else{
		resultlength = 6;	//for comparison operators ( <, =, > ).....for characters {f a l s e -1} , length = 6
		result = (int*) malloc (sizeof(int) * resultlength); 	//or {t r u e -1} length = 5
	}
	

	z= 0;	//reset 0
	y =0;
	for(int i = 0; i < resultlength; i++){		//initialize answer array to 0's
		result[i] = 0;
	}
	result[resultlength - 1] = -1; 			//..except last index to -1
 
 
 	//does addition on arrays
	if(op == '+') {
		return add(input1, input2, base);
	}
	
	//does subtraction on arrays (calls subtraction function)
	if(op == '-') {
		return subtract(input1, input2, base);
	}
	
	//checks input1 > input2 ???
	if(op == '>'){
		int i = 0;							//index
		int greater_than = false;			//input1 > input2
		int less_than = false;				//input1 < input2
		if(len1 == len2){					//same length arrays, have to now compare each index of arrays
			int index = 0;					//index
			while (index < len1 && greater_than == false && less_than == false){	//index still hasnt reached end of array and input1 vs. input2  > or < has not been determined yet
				if (input1[index] > input2[index] && less_than == false){	//if input1 > input2
					greater_than = true;
				}
				else if (input1[index] < input2[index]){	//input1 < input2
					less_than = true;
				}
				index++;									
			}
			
		}
		else if(len1 != len2){					//different length arrays
			if (len1 < len2){					//longer length array is > than since they are in same base
				greater_than = false;
			}
			else{
				greater_than = true;
			}
		}
		
		if(greater_than == false){		//result array prints out "false" string in bignum_print for  in1 > in2 
			i = 0;
			result[0] = 15;
			result[1] = 10;
			result[2] = 21;
			result[3] = 28;
			result[4] = 14;
			result[5] = -1;
			return result;
		}
		else{							//result array prints out "true"
			result[0] = 29;
			result[1] = 27;
			result[2] = 30;
			result[3] = 14;
			result[4] = -1;
			return result;
		}
	}
	
	if(op == '<'){ //nearly exact same code as '>' op condition, but with reverse comparison sign used
		int i = 0;
		int less_than = false;
		int greater_than = false;
	
	//compare algorithm '<', if positive arrays have same length, traverse each index and compare values
	//else if positive arrays have different lengths, then longer array is always > than 
		if(len1 == len2){
			int index = 0;
			while (index < len1 && greater_than == false && less_than == false){
				if (input1[index] > input2[index] && less_than == false){
					greater_than = true;
				}
				else if (input1[index] < input2[index]){
					less_than = true;
				}
				index++;
			}
		}
		else if(len1 != len2){
			
			if(len1 > len2){
				less_than = false;
			}									
			else{
				less_than = true;
			}
		}
		
		if(less_than == false){
			i = 0;
			result[0] = 15;
			result[1] = 10;
			result[2] = 21;
			result[3] = 28;
			result[4] = 14;
			result[5] = -1;
			return result;
		}
		else{
			result[0] = 29;
			result[1] = 27;
			result[2] = 30;
			result[3] = 14;
			result[4] = -1;
			return result;
		}		
	}
	
	if(op == '='){		//input1 == input2 ????
		int i = 0;		//index
		int equal = 1;	// = to is true
		while((i < len1 || i < len2) && equal == true){	//go through arrays and compare each index	
			if(input1[i] == input2[i]){		
				i++;
				equal = true;
			}
			else{					//if certain index has different values, than = is false
				equal = false;
			}	
		}
		if(equal == false){		//print false
			i = 0;
			result[0] = 15;
			result[1] = 10;
			result[2] = 21;
			result[3] = 28;
			result[4] = 14;
			result[5] = -1;
			return result;
		}
		else{					//print true
			result[0] = 29;
			result[1] = 27;
			result[2] = 30;
			result[3] = 14;
			result[4] = -1;
			return result;
		}
	}
	

	return result;
}

/*
 * Print to "stderr" and exit program
 */
void print_usage(char* name) {
	fprintf(stderr, "----------------------------------------------------\n");
	fprintf(stderr, "Usage: %s base input1 operation input2\n", name);
	fprintf(stderr, "base must be number between 2 and 36, inclusive\n");
	fprintf(stderr, "input1 and input2 are arbitrary-length integers\n");
	fprintf(stderr, "Two operations are allowed '+' and '-'\n");
	fprintf(stderr, "----------------------------------------------------\n");
	exit(1);
}


/*
 * MAIN: Run the program and tests functions.
 * sample command: ./bignum_math 4 12 + 13
 * Result: 31
 */
int main(int argc, char** argv) {

	int input_base;		//2 to 36 

    int* input1;		//1st array
    int* input2;		//2nd array
    int* result;		//answer array

	if(argc != 5) { 			//checks for valid # of arguments
		print_usage(argv[0]); 	//print_usage is standard error message
	}

	input_base = string_to_integer(argv[1]);	//creates base integer

	if(!valid_base(input_base)) { 							//checks for valid input
		fprintf(stderr, "Invalid base: %s\n", argv[1]);		//shows where input is an error
		print_usage(argv[0]);
	}
	

	if(!valid_input(argv[2], input_base)) { 				//1st array valid input?
		fprintf(stderr, "Invalid input1: %s\n", argv[2]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[4], input_base)) { 				//2nd array valid input
		fprintf(stderr, "Invalid input2: %s\n", argv[4]);
		print_usage(argv[0]);
	}

        char op = argv[3][0];	//check for valid operator + = - < >
	if(op != '-' && op != '+' && op != '<' && op != '>' && op != '=') {
		fprintf(stderr, "Invalid operation: %s\n", argv[3]);
		print_usage(argv[0]);
	}

	input1 = string_to_integer_array(argv[2]);	//make 1st array into integers
    input2 = string_to_integer_array(argv[4]);	//2nd array to integers

    result = perform_math(input1, input2, argv[3][0], input_base);	//perform arithmetic on arrays and obtain answer array

    printf("Result: ");
    bignum_print(result);		//print out answer array
	printf("\n");

	exit(0);
}
