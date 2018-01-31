#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "externs.h"
#include "cephes.h"
#include "tm_stm32f4_sdram.h"

void CumulativeSums(int n)
{
	int32_t S, sup, inf, z, zrev, k;
	float sum1, sum2, p_value;

	S = 0;
	sup = 0;
	inf = 0;
	for ( k=0; k<n; k++ ) {
		TM_SDRAM_Read8(k) ? S++ : S--;
		if ( S > sup )
			sup++;
		if ( S < inf )
			inf--;
		z = (sup > -inf) ? sup : -inf;
		zrev = (sup-S > S-inf) ? sup-S : S-inf;
	}
	
	// forward
	sum1 = 0.0;
	for ( k=(-n/z+1)/4; k<=(n/z-1)/4; k++ ) {
		sum1 += cephes_normal(((4*k+1)*z)/sqrt(n));
		sum1 -= cephes_normal(((4*k-1)*z)/sqrt(n));
	}
	sum2 = 0.0;
	for ( k=(-n/z-3)/4; k<=(n/z-1)/4; k++ ) {
		sum2 += cephes_normal(((4*k+3)*z)/sqrt(n));
		sum2 -= cephes_normal(((4*k+1)*z)/sqrt(n));
	}

	p_value = 1.0 - sum1 + sum2;
	
	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
	{
		//pval out of range
	}

	//decyzja forward

	// backwards
	sum1 = 0.0;
	for ( k=(-n/zrev+1)/4; k<=(n/zrev-1)/4; k++ ) {
		sum1 += cephes_normal(((4*k+1)*zrev)/sqrt(n));
		sum1 -= cephes_normal(((4*k-1)*zrev)/sqrt(n));
	}
	sum2 = 0.0;
	for ( k=(-n/zrev-3)/4; k<=(n/zrev-1)/4; k++ ) {
		sum2 += cephes_normal(((4*k+3)*zrev)/sqrt(n));
		sum2 -= cephes_normal(((4*k+1)*zrev)/sqrt(n));
	}
	p_value = 1.0 - sum1 + sum2;

	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
	{
		//pval out of range
	}

	//decyzja backwardds
}
