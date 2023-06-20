/* fmod function */
/* Author Robert R. Hall (hall@crach.cts.com) */

/* Id: fmod.c,v 1.4 1994/06/24 11:43:40 ceriel Exp */

#include "core/pch.h"

#if 0 // Currently unused, may be used in the future

#ifdef ACK_MATH_LIBRARY

#include "modules/stdlib/src/thirdparty_math/localmath.h"

double
(fmod)(double x, double y)
{				/* compute fmod(x, y) */
	double t;
	int n, neg;
	int ychar, xchar;

    	if (y == 0.0) {
		errno = EDOM;
		return 0.0;
    	}
    				/* fmod(finite, finite) */
	if (y < 0.0) y = -y;
	if (x < 0.0) x = -x, neg = 1;
	else neg = 0;
	t = frexp(y, &ychar);
				/* substract |y| until |x| < |y| */

	t = frexp(x, &xchar);
	for (n = xchar - ychar; 0 <= n; --n) {
				/* try to substract |y|*2^n */
	    t = ldexp(y, n);
	    if (t <= x) x -= t;
	}
	return (neg ? -x : x);
}

#endif // ACK_MATH_LIBRARY

#endif // 0
