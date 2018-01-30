/** \file itoa.c
 * \brief itoa() function.
 * \details itoa() function to convert integer to string.
 * \author Freddie Chopin
 * \date 2009-07-13
 */

/******************************************************************************
* project: 750-750
* chip: STM32F103VB
* compiler: arm-none-eabi-gcc 4.4.1
*
* prefix: (none)
*
* available global functions:
* 	char* itoa(int value, char* buffer, int base)
*
* available local functions:
* 	static void __reverse(char* begin,char* end);
******************************************************************************/

/*
+=============================================================================+
| includes
+=============================================================================+
*/

#include <stdint.h>

#include "itoa.h"

/*
+=============================================================================+
| module variables
+=============================================================================+
*/

/*
+=============================================================================+
| local functions' declarations
+=============================================================================+
*/

static void __reverse(char* begin,char* end);

/*
+=============================================================================+
| global functions
+=============================================================================+
*/

/*------------------------------------------------------------------------*//**
* \brief itoa() function.
* \details Converts signed integer to an char array. Valid 'base' in [2;16].
* Only base == 10 values are treated as signed
*
* \param [in] value the value which will be converted
* \param [out] buffer the output buffer
* \param [in] base the base of conversion
* \return pointer to \em buffer
*//*-------------------------------------------------------------------------*/

char* itoa(int value, char* buffer, int base)
{
	static const char digits[]="0123456789abcdef";

	char* buffer_copy=buffer;
	int32_t sign=0;
	int32_t quot,rem;

	if ((base>=2)&&(base<=16))				// is the base valid?
	{
		if (base == 10 && (sign = value) < 0)// negative value and base == 10? store the copy (sign)
			value = -value;					// make it positive

		do
		{
			quot=value/base;				// calculate quotient and remainder
			rem=value%base;
			*buffer++ = digits[rem];		// append the remainder to the string
		} while ((value=quot));				// loop while there is something to convert

		if (sign<0)							// was the value negative?
			*buffer++='-';					// append the sign

		__reverse(buffer_copy,buffer-1);		// reverse the string
	}

	*buffer='\0';
	return buffer_copy;
}

/**************************************************
 *
 *    ftoa - converts float to string
 *
 ***************************************************
 *
 *    This is a simple implemetation with rigid
 *    parameters:
 *            - Buffer must be 8 chars long
 *            - 3 digits precision max
 *            - absolute range is -524,287 to 524,287
 *            - resolution (epsilon) is 0.125 and
 *              always rounds down
 **************************************************/
 void ftoa(float Value, char* Buffer)
 {
     union
     {
         float f;

         struct
         {
             unsigned int    mantissa_lo : 16;
             unsigned int    mantissa_hi : 7;
             unsigned int     exponent : 8;
             unsigned int     sign : 1;
         };
     } helper;

     unsigned long mantissa;
     signed char exponent;
     unsigned int int_part;
     char frac_part[3];
     int i, count = 0;

     helper.f = Value;
     //mantissa is LS 23 bits
     mantissa = helper.mantissa_lo;
     mantissa += ((unsigned long) helper.mantissa_hi << 16);
     //add the 24th bit to get 1.mmmm^eeee format
     mantissa += 0x00800000;
     //exponent is biased by 127
     exponent = (signed char) helper.exponent - 127;

     //too big to shove into 8 chars
     if (exponent > 18)
     {
         Buffer[0] = 'I';
         Buffer[1] = 'n';
         Buffer[2] = 'f';
         Buffer[3] = '\0';
         return;
     }

     //too small to resolve (resolution of 1/8)
     if (exponent < -3)
     {
         Buffer[0] = '0';
         Buffer[1] = '\0';
         return;
     }

     count = 0;

     //add negative sign (if applicable)
     if (helper.sign)
     {
         Buffer[0] = '-';
         count++;
     }

     //get the integer part
     int_part = mantissa >> (23 - exponent);
     //convert to string
     itoa(int_part, &Buffer[count], 10);

     //find the end of the integer
     for (i = 0; i < 8; i++)
         if (Buffer[i] == '\0')
         {
             count = i;
             break;
         }

     //not enough room in the buffer for the frac part
     if (count > 5)
         return;

     //add the decimal point
     Buffer[count++] = '.';

     //use switch to resolve the fractional part
     switch (0x7 & (mantissa  >> (20 - exponent)))
     {
         case 0:
             frac_part[0] = '0';
             frac_part[1] = '0';
             frac_part[2] = '0';
             break;
         case 1:
             frac_part[0] = '1';
             frac_part[1] = '2';
             frac_part[2] = '5';
             break;
         case 2:
             frac_part[0] = '2';
             frac_part[1] = '5';
             frac_part[2] = '0';
             break;
         case 3:
             frac_part[0] = '3';
             frac_part[1] = '7';
             frac_part[2] = '5';
             break;
         case 4:
             frac_part[0] = '5';
             frac_part[1] = '0';
             frac_part[2] = '0';
             break;
         case 5:
             frac_part[0] = '6';
             frac_part[1] = '2';
             frac_part[2] = '5';
             break;
         case 6:
             frac_part[0] = '7';
             frac_part[1] = '5';
             frac_part[2] = '0';
             break;
         case 7:
             frac_part[0] = '8';
             frac_part[1] = '7';
             frac_part[2] = '5';
             break;
     }

     //add the fractional part to the output string
     for (i = 0; i < 3; i++)
         if (count < 7)
             Buffer[count++] = frac_part[i];

     //make sure the output is terminated
     Buffer[count] = '\0';
 }

 char * dtoa(char *s, double n) {
     // handle special cases
     if (isnan(n)) {
         strcpy(s, "nan");
     } else if (isinf(n)) {
         strcpy(s, "inf");
     } else if (n == 0.0) {
         strcpy(s, "0");
     } else {
         int digit, m, m1;
         char *c = s;
         int neg = (n < 0);
         if (neg)
             n = -n;
         // calculate magnitude
         m = log10(n);
         int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
         if (neg)
             *(c++) = '-';
         // set up for scientific notation
         if (useExp) {
             if (m < 0)
                m -= 1.0;
             n = n / pow(10.0, m);
             m1 = m;
             m = 0;
         }
         if (m < 1.0) {
             m = 0;
         }
         // convert the number
         while (n > PRECISION || m >= 0) {
             double weight = pow(10.0, m);
             if (weight > 0 && !isinf(weight)) {
                 digit = floor(n / weight);
                 n -= (digit * weight);
                 *(c++) = '0' + digit;
             }
             if (m == 0 && n > 0)
                 *(c++) = '.';
             m--;
         }
         if (useExp) {
             // convert the exponent
             int i, j;
             *(c++) = 'e';
             if (m1 > 0) {
                 *(c++) = '+';
             } else {
                 *(c++) = '-';
                 m1 = -m1;
             }
             m = 0;
             while (m1 > 0) {
                 *(c++) = '0' + m1 % 10;
                 m1 /= 10;
                 m++;
             }
             c -= m;
             for (i = 0, j = m-1; i<j; i++, j--) {
                 // swap without temporary
                 c[i] ^= c[j];
                 c[j] ^= c[i];
                 c[i] ^= c[j];
             }
             c += m;
         }
         *(c) = '\0';
     }
     return s;
 }


/*
+=============================================================================+
| local functions
+=============================================================================+
*/

/*------------------------------------------------------------------------*//**
* \brief Reverse order in string.
* \details Reverses the order of the elements in string.
*
* \param [in,out] begin pointer to the beginning of the string
* \param [in,out] end pointer to the end of the string
*//*-------------------------------------------------------------------------*/

static void __reverse(char* begin,char* end)
{
	char temp;

	while (end>begin)
	{
		temp=*end;
		*end--=*begin;
		*begin++=temp;
	}
}

/******************************************************************************
* END OF FILE
******************************************************************************/
