#include "newintal.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef struct Intal
{
	char* int_al; //Character array to hold the integer of arbitrary length
	int n; //Size of array
}Intal;

void* intal_create(const char* str)
{
	long int i;
	int alpha_flag = 0;	//check to see if the first character is an alphabet

	if(str == NULL)	//If nul string passed
		return NULL;
	
	Intal *I = (Intal*)malloc(sizeof(Intal));	//Allocating space for the structure
	if(I == NULL)	//Checking if memory is allocated
		return NULL;

	I->n = strlen(str);	//Length of integer stored in I->n
	I->int_al = (char*)malloc((I->n+1) * sizeof(int));	//Allocating space for character array
	if(I->int_al == NULL)
		return NULL;
	
	for(i = 0; i < I->n; i++)	//Checking if the string is valid
	{
		if(!isdigit(str[i]))
		{
			if(i==0)
				alpha_flag = 1;	//First letter is an alphabet
			break;
		}
		I->int_al[i] = str[i];
	}

	if(alpha_flag == 1)
		strcpy(I->int_al, "0");	//Return 0

	else
		I->int_al[i] = '\0';	//Terminate string

	I->n = strlen(I->int_al);

	return (void*)I;
}

void intal_destroy(void* intal)
{
	Intal* I = (Intal*)intal; 
	if(I == NULL)	//No free if Intal is null
		return;
	free(I->int_al);	//freeing the dynamically allocated array
	free(I);	//freeing the structure
}

//Helper function to make intals a required length by adding 0's to the beginning
char* pad_with_0s(char* intal, int n)
{
	if(intal == NULL)
		return NULL;

	char* padded_I = (char*)malloc((n+1) * sizeof(char));
	if(padded_I == NULL)	
		return NULL;

	int i, j;
	for(i=0; i < n - strlen(intal); i++)
		padded_I[i] = '0';

	for(i=i, j=0; j<strlen(intal); i++, j++)
		padded_I[i] = intal[j];
	padded_I[n] = '\0';

	return(padded_I);
}

 // Helper function to get rid of leading zeroes
char* strip_0(char* intal)
{
	if(intal == NULL)
		return NULL;

	if(intal[0] != '0')	//No leading zeroes, so returned as it is
		return intal;
	
	char* stripped_I = (char*)malloc(strlen((intal)+1)*sizeof(char));
	if(stripped_I == NULL)
		return NULL;

	int i=0, j=0;
	while(intal[i] == '0' && i < strlen(intal))
		i++;

	if(i==strlen(intal)) //The whole number is a 0
		strcpy(stripped_I, "0");
	else
	{
		while(i < strlen(intal))
			stripped_I[j++] = intal[i++];
		stripped_I[j] = '\0';
	}
	strcpy(intal, stripped_I);
	free (stripped_I);
	return intal;	
}

char* intal2str(void* intal)
{
	Intal* I = (Intal*)intal;

	if(I == NULL)	//if intal is null, return not a number
	{
		char* Nan = (char*)malloc(3*sizeof(char));
		strcpy(Nan, "NaN");
		return Nan;
	}
	char* string = (char*)malloc((I->n+1) * sizeof(char));
	strcpy(string, I->int_al);
	return string;	//Returning the character array
}

void* intal_increment(void* intal)
{
	Intal* I = (Intal*)intal;
	int i, overflow = 1, occur_9 = 0;
	if(I == NULL)
		return NULL;

	for(i = I->n - 1; i >= 0; i--)
	{
		if(I->int_al[i] != '9')	//If digit is not 9, adds 1 to it and breaks out of the loop
		{
			I->int_al[i]++;
			overflow = 0;	//Overflow set to 0 as there is at least one non-9 value
			break;
		}
		else	//If 9 is encountered, for loop runs till the first non-9 value is found from the end and occur_9 flag is set to 1
			occur_9 = 1;
	}

	if(occur_9 == 1)	//If all the digits after the incremented value is 9, they are all set to 0
	{
		for(i++; i<I->n; i++)
			I->int_al[i] = '0';
	}

	if(overflow == 1) //If intal is only 9's, memory is reallocated
	{
		char* str = (char*)malloc((I->n + 2) * sizeof(char));
		if(str == NULL)
			return NULL;

		str[0] = '1';
		char* temp = intal2str(I);
		strcat(str, temp);	//new string with 1 and I->n 0's is created
		free(temp);
		intal_destroy(I);	//old intal destroyed
		void* newI = intal_create(str); 	//new intal returned
		free(str);
		return newI;
	}	

	return (void*)I;	//returns the old intal
}

void* intal_decrement(void* intal)
{
	Intal* I = (Intal*)intal;
	int i, underflow = 1, occur_0 = 0;
	
	if(I == NULL)
		return NULL;

	if(I->int_al[0] == '0')	//no change made if intal is 0
		return (void*)I;

	for(i = I->n - 1; i >= 0; i--)
	{
		if(I->int_al[i] != '0')	//If digit is not 0, subtracts 1 from it and breaks out of the loop
		{
			underflow = 0;	//Underflow set to 0 as there is at least one non-0 value
			break;
		}
		else
			occur_0 = 1;	//If 0 is encountered, for loop runs till the first non-0 value is found from the end and occur_0 flag is set to 1
	}

	if(I->n == 1 && underflow == 1)  // to check if the number is 0 after decrement, 0 is returned
		return (void*)I;
	
	I->int_al[i]--; //First non-zero value decremented

	if(occur_0 == 1)	//If all the digits after the decremented value is 0, they are all set to 9
	{
		for(i++; i<I->n; i++)
			I->int_al[i] = '9';
	}

	if(I->int_al[0] == '0' && I->n > 1) //If there is a leading zero, memory allocated is reduced
	{
		char* str = (char*)malloc((I->n) * sizeof(char));
		if(str == NULL)
			return NULL;
	
		for(i=1; i< I->n; i++)
			str[i-1] = '9';	//new string with I->n-1 9's is created
		str[i-1] = '\0';

		intal_destroy(I);	//old intal is destroyed
		void* newI = intal_create(str);	//new intal is created
		free(str);
		return newI;
	}	

	return (void*)I;
}

int intal_compare(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;
	int i;

	if(I1 == NULL || I2 == NULL)
		return -2;
	
	if(I1->n > I2->n) //if strcmp function returns a positive number, return 1
		return 1;

	else if(I1->n < I2->n)	//if strcmp function returns a negative number return -1
		return -1;
	
	else
	{
		for(i=0; i<I1->n; i++)
		{
			if(I1->int_al[i] > I2->int_al[i])	//if strcmp function returns a positive number, return 1
				return 1;

			else if(I1->int_al[i] < I2->int_al[i])	//if strcmp function returns a negative number return -1
				return -1;
		}
	}
	return 0;	//if intals are equal, return 0
}


char* string_add(char* I1, char* I2)
{
	int length = 0;

	if(strlen(I1) >= strlen(I2))
		length = strlen(I1);

	else
		length = strlen(I2);

	I1 = pad_with_0s(I1, length);	//Making strings of the same length
	I2 = pad_with_0s(I2, length);	//Making strings of the same length

	int i, j, k = length, carry = 0;
	int temp;

	char* sum = (char*)malloc((length+1) * sizeof(char));	//allocating space for the sum
	if(sum == NULL)
		return NULL;
	
	for(i= length-1, j= length-1; i>=0 && j>=0; i--, j--)
	{
		temp = carry + I1[i] - '0' + I2[j] - '0';	
		
		if(temp > 9)	//Checks if the addition of the two numbers results in a carry or not
		{
			temp -= 10;
			carry = 1;
		}
		
		else
			carry = 0;

		sum[--k] = temp + '0';
	}
	sum[length] = '\0';
	free(I1);
	free(I2);
	if(carry == 1)	//If the final addition results in a carry, reallocte space for the sum to store the carry digit
	{
		char* final_sum = (char*)malloc((length+2) * sizeof(char));
		if(final_sum == NULL)
			return NULL;

		final_sum[0] = '1';
		final_sum[1] = '\0';
		strcat(final_sum, sum);
		final_sum[length+1] = '\0';
		free(sum);
		return final_sum;
	}	

	else
		return sum;
}

//Wrapper function for add
void* intal_add(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;
	
	if(I1 == NULL || I2 == NULL)
		return NULL;

	char* string1 = intal2str(I1);
	char* string2 = intal2str(I2);

	char* sum = string_add(string1, string2);
	free(string1);
	free(string2);
	Intal* Isum = intal_create(sum);
	free(sum);
	return (void*)Isum;
}

char* string_diff(char* I1, char* I2)
{
	int length = 0;	
	if(strlen(I1) >= strlen(I2))
		length = strlen(I1);

	else
		length = strlen(I2);

	I1 = pad_with_0s(I1, length);	//Making strings of the same length
	I2 = pad_with_0s(I2, length);	//Making strings of the same length

	int i, j, k = length;
	int temp;

	char* diff = (char*)malloc((length+1) * sizeof(char*));
	if(diff == NULL)
		return NULL;

	for(i = length-1, j = length-1; i>=0 && j >= 0; i--, j--)
	{
		if(I1[i] < I2[j])	//if first digit smaller than second digit, borrow required
		{
			k = i-1;
			while(I1[k] == '0')
			{
				I1[k] = '9';
				k--;
			}
			I1[k]--;
			temp = 10 + (I1[i] - '0') - (I2[j] - '0');	//the borrow value is added to the smaller digit 
		}

		else
			temp = (I1[i] - '0') - (I2[j] - '0');
		diff[i] = temp + '0';
	}
	diff[length] = '\0';
	free(I1);
	free(I2);
	return(diff);
}

//Wrapper function for difference
void* intal_diff(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;

	if(I1 == NULL || I2 == NULL)
		return NULL;

	char* string1;
	char* string2;
	int check = intal_compare(I1, I2);
	if(check > 0)	//Assigning strings based on which intal is greater
	{
		string1 = intal2str(I1);
		string2 = intal2str(I2);
	}

	else if(check < 0)
	{
		string1 = intal2str(I2);
		string2 = intal2str(I1);
	}

	else
		return (void*)intal_create("0");

	char* diff = string_diff(string1, string2);
	diff = strip_0(diff);
	Intal* Idiff = intal_create(diff);
	free(diff);
	free(string1);
	free(string2);
	return (void*)Idiff;
}

//Helper function to pad zeroes at the end
char* multiply_by_10(char* intal, int mul)
{
	int i;	
	char* new = (char*)malloc((strlen(intal)+mul+1)*sizeof(char));
	if(new == NULL)
		return NULL;

	strcpy(new, intal);

	for(i=strlen(intal); i<strlen(intal)+mul; i++)
		new[i] = '0';
	new[i] = '\0';
	return new;
}

char* string_multiply(char* I1, char* I2)
{
	if(strcmp(I1, "0") == 0 || strcmp(I2, "0") == 0) // Returns 0 for any 0 multiplication
	{
		char* z = (char*)malloc(2*sizeof(char));
		if(z == NULL)
			return NULL;

		z[0] = '0';
		z[1]= '\0';
		return z;
	}

	if(strcmp(I1,"1") == 0) // Returns number for multiplication by 1
	{
		char* newI2 = (char*)malloc((strlen(I2)+1)*sizeof(char));
		if(newI2 == NULL)
			return NULL;
		
		strcpy(newI2, I2);
		return newI2;
	}

	else if(strcmp(I2,"1") == 0) // Returns number for multiplication by 1
	{
		char* newI1 = (char*)malloc((strlen(I1)+1)*sizeof(char));
		if(newI1 == NULL)
			return NULL;
		
		strcpy(newI1, I1);
		return newI1;		
	}
	
	//If both are non-zero and not 1
	int length;

	if(strlen(I1) >= strlen(I2))	//Making numbers the same size
		length = strlen(I1);

	else
		length = strlen(I2);

	I1 = pad_with_0s(I1, length);
	I2 = pad_with_0s(I2, length);

	int temp;
	char* prod;
	if(length == 1) //Multiplication of single digits
	{
		temp = (I1[0] - '0') * (I2[0] - '0');
		if(temp > 9)
		{
			prod = (char*)malloc(3*sizeof(char));
			if(prod == NULL)
				return NULL;

			prod[0] = temp / 10 + '0';
			prod[1] = (temp % 10) + '0';
			prod[2] = '\0';
		}

		else
		{
			prod = (char*)malloc(2*sizeof(char));
			if(prod == NULL)
				return NULL;
			
			prod[0] = temp + '0';
			prod[1] = '\0';
		}
	
		free(I1);
		free(I2);
		return prod;
	}

	int fh = (int)ceil((float)(length)/ 2);	//length of first half
	int lh = length - fh;	//length of second half
	int i, j;

	char* I1_L = (char*)malloc((fh+1) *sizeof(char));	//First half of I1
	char* I1_R = (char*)malloc((lh+1) *sizeof(char));	//Second half of I1
	char* I2_L = (char*)malloc((fh+1) *sizeof(char));	//First half of I2
	char* I2_R = (char*)malloc((lh+1) *sizeof(char));	//Second half of I2

	for(i=lh-1, j=lh-1; i>=0; i--, j--)
		I1_L[j] = I1[i]; 
	I1_L[lh] = '\0';

	
	for(i=length-1, j=fh-1; i>=length-lh-1 && j>=0; i--, j--)
		I1_R[j] = I1[i]; 
	
	I1_R[fh] = '\0';

	for(i=lh-1, j=lh-1; i>=0; i--, j--)
		I2_L[j] = I2[i]; 
	I2_L[lh] = '\0';


	for(i=length-1, j=fh-1; i>=length-lh-1 && j>=0; i--, j--)
		I2_R[j] = I2[i]; 
	I2_R[fh] = '\0';
	
	char* P1 = string_multiply(I1_L, I2_L);	//Multiplication of first halves
	char* P2 = string_multiply(I1_R, I2_R);	//Multiplication of second halves
	char* temp4 = string_add(I1_L, I1_R);	//Addition of halves of first string
	char* temp5 = string_add(I2_L, I2_R);	//Addition of halves of second string
	char* P3 = string_multiply(temp4, temp5);	//Multiplication of added results
	char* F = multiply_by_10(P1, 2*fh);	//Power of first term
	char* temp1 = string_add(P1, P2);
	char* temp2 = string_diff(P3, temp1);
	char* M = multiply_by_10(temp2, fh);	//Power of middle term
	char* temp3 = string_add(F,M);	
	prod = string_add(P2, temp3);	//Addition of all terms

	free(temp3);
	free(M);
	free(temp1);
	free(temp2);
	free(F);
	free(P3);
	free(temp5);
	free(temp4);
	free(P2);	
	free(P1);
	free(I1_L);
	free(I1_R);
	free(I2_L);
	free(I2_R);
	free(I1);
	free(I2);

	return prod;
}

//Wrapper function for multiply
void* intal_multiply(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;
	
	if(I1 == NULL || I2 == NULL)
		return NULL;

	char* string1 = intal2str(I1);
	char* string2 = intal2str(I2);

	char* prod = string_multiply(string1, string2);
	prod = strip_0(prod);
	Intal* Iprod = intal_create(prod);
	free(prod);
	free(string1);
	free(string2);

	return (void*)Iprod;
}

char* string_divide(char* I1, char* I2)
{
	if(!strcmp(I2,"1"))	//Divide by 1 returns the number
		return I1;

	if(!strcmp(I2, "0"))	//Divide by 0 returns null
		return NULL;

	char* quotient = (char*)malloc((strlen(I1)+1)*sizeof(char));
	char* temp = (char*)malloc((strlen(I1)+1)*sizeof(char));	//to store temporary dividend

	strncpy(temp, I1, strlen(I2));	//copy the first strlen(I)2 elements of I1 into the temp dividend
	if(strcmp(I2, temp)>0)	//If the value of the temp dividend is less than I2, copy one more character
	{
		temp[strlen(I2)] = I1[strlen(I2)];
		temp[strlen(I2)+1] = '\0';	//terminate the string
	}
	else
		temp[strlen(I2)] = '\0';

	int i = 0; int j = strlen(temp);
	char* Dividend = (char*)malloc(j+1 * sizeof(char));

	while(j<=strlen(I1))
	{
		strcpy(Dividend, temp);
		quotient[i] = '0';

		while(strcmp(Dividend, I2) >= 0 || strlen(Dividend) > strlen(I2))	//
		{
			if(strlen(Dividend)<strlen(I2))
				break;

			char* tempDividend = string_diff(Dividend, I2);
			tempDividend = strip_0(tempDividend);
			strcpy(Dividend, tempDividend);
			free(tempDividend);
			quotient[i]++;
		}

		i++;
		strcpy(temp, Dividend);
		int k = strlen(Dividend);
		temp[k] = I1[j];	//Taking the next part of dividend
		k++;
		j++;
		temp[k] = '\0'; //Terminating temp
		if(temp[0] == '0')
		{
			temp = strip_0(temp);
			k = strlen(temp);
		}

		while(strcmp(temp, I2) < 0 && strlen(temp)<=strlen(I2)&&j<strlen(I1))
		{
			temp[k] = I1[j];
			k++;
			quotient[++i] ='0';	//Quotient is zero because dividend is less than divisor
			temp[k] = '\0';
			j++;
		}
		temp[k] = '\0';
	}
	free(Dividend);
	free(temp);
	quotient[i] = '\0';
	return quotient;
}

//Wrapper function for divide
void* intal_divide(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;
	
	if(I1 == NULL || I2 == NULL)
		return NULL;

	int equal_check = intal_compare(I1, I2);
	if(equal_check == 0)
		return (void*)intal_create("1");

	char* string1 = intal2str(I1);
	char* string2 = intal2str(I2);

	char* quo = string_divide(string1, string2);
	if(quo)
		quo = strip_0(quo);
	Intal* Iquo = intal_create(quo);
	if(quo)
	{
		if(strcmp(quo, string1)!=0)
			free(string1);
		free(quo);
	}
	free(string2);
	return (void*)Iquo;
}

char* string_pow(char* I1, char* I2)
{
	if(strcmp("0", I1)==0)	//Returns 0 for any 0^n
	{
		char *Zero = (char*)malloc(1*sizeof(char));
		strcpy(Zero, "0");
		return Zero;
	}
	if(strcmp("0", I2) == 0)	//Returns 1 for any n^0
	{
		char *One = (char*)malloc(1*sizeof(char));
		strcpy(One, "1");
		return One;
	}
	char *exp = (char*)malloc(10000*sizeof(char));
	strcpy(exp, I1);
	int i ;
	int power = atoi(I2);
	for(i=1; i<power; i++)
	{
		char* tempexp = string_multiply(I1, exp);
		if(tempexp[0] == '0')
			tempexp = strip_0(tempexp);

		strcpy(exp, tempexp);
		free(tempexp);
	}
	return exp;
}

//Wrapper function for power
void* intal_pow(void* intal1, void* intal2)
{
	Intal* I1 = (Intal*)intal1;
	Intal* I2 = (Intal*)intal2;
	
	if(I1 == NULL || I2 == NULL)
		return NULL;
	
	char* string1 = intal2str(I1);
	char* string2 = intal2str(I2);

	char* exp = string_pow(string1, string2);

	if(exp[0] =='0')
		exp = strip_0(exp);

	Intal* Iexp = intal_create(exp);

	free(exp);
	free(string1);
	free(string2);
	return (void*)Iexp;
}