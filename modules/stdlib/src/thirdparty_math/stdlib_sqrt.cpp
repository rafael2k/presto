/*
 * (c) copyright 1988 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 *
 * Author: Ceriel J.H. Jacobs
 */
/* Id: sqrt.c,v 1.4 1994/06/24 11:44:18 ceriel Exp */

#include "core/pch.h"

#ifdef ACK_MATH_LIBRARY

#include "modules/stdlib/src/thirdparty_math/localmath.h"

#ifndef HAVE_SQRT

#define NITER	5

double
op_sqrt(double x)
{
	int exponent;
	double val;

	if (op_isnan(x)) {
		set_errno(EDOM);
		return x;
	}
	if (x <= 0) {
		if (x < 0) set_errno(EDOM);
		return 0;
	}

	if (x > DBL_MAX) return x;	/* for infinity */

	val = op_frexp(x, &exponent);
	if (exponent & 1) {
		exponent--;
		val *= 2;
	}
	val = op_ldexp(val + 1.0, exponent/2 - 1);
	/* was: val = (val + 1.0)/2.0; val = ldexp(val, exponent/2); */
	for (exponent = NITER - 1; exponent >= 0; exponent--) {
		val = (val + x / val) / 2.0;
	}
	return val;
}

#endif

#endif // ACK_MATH_LIBRARY
