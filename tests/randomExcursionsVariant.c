#include <stdio.h> 
#include <math.h> 
#include <string.h>
#include <stdlib.h>
#include "externs.h"
#include "cephes.h"
#include "tm_stm32f4_sdram.h"

void RandomExcursionsVariant(int n)
{
	int32_t i, p, J, x, constraint, count, *S_k;
	int32_t stateX[18] = { -9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	float p_value;
	
	J = 0;
	TM_SDRAM_Write2ndVector(0, 2*(int)TM_SDRAM_Read8(0) - 1);
	for ( i=1; i<n; i++ ) {
		TM_SDRAM_Write2ndVector(i, TM_SDRAM_Read2ndVector(i-1) + 2*TM_SDRAM_Read8(i) - 1);
		if ( TM_SDRAM_Read2ndVector(i) == 0 )
			J++;
	}
	if ( TM_SDRAM_Read2ndVector(n-1) != 0 )
		J++;

	constraint = (int)MAX(0.005*pow(n, 0.5), 500);
	if (J < constraint) {
		//insufficient nr of cycles
	}
	else {
		for ( p=0; p<=17; p++ ) {
			x = stateX[p];
			count = 0;
			for ( i=0; i<n; i++ )
				if ( TM_SDRAM_Read2ndVector(i) == x )
					count++;
			p_value = erfc(fabs(count-J)/(sqrt(2.0*J*(4.0*fabs(x)-2))));

			if ( isNegative(p_value) || isGreaterThanOne(p_value) )
			{
				//pval out of range
			}
			//decyzja
		}
	}
	free(S_k);
}
