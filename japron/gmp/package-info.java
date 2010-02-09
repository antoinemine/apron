/*
 * package-info.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

/**
 * Java binding for the GNU Multiple Precision Arithmetic Library (GMP)
 * and the Multiple Precision Floating-Point Reliable Library (MPFR).
 *
 * <p> This includes arbitrary-precision integers {@link gmp.Mpz} and
 * rationals {@link gmp.Mpq} (growing as needed), and multi-precision
 * floating-points numbers {@link gmp.Mpfr} that extend the IEEE-754 standard
 * to an arbitrary, user-chosen precision level. 
 *
 * <p> Unlike Java's {@code BigIntegers}, all {@link gmp.Mpz}, {@link gmp.Mpq}, 
 * and {@link gmp.Mpfr} objects are mutable.
 * 
 * <p> Most arithmetic operators are in three-address mode: the this object
 * is the destination while the operator arguments are unchanged
 * (with the exception of the division and extended gcd, where extra
 * results are returned in the first arguments).
 * It is possible to pass the same object (including this) as source and
 * destination of an operation. However, when an operator modifies several
 * objects, these must be distinct.
 *
 * <p> The binding is based on GMP 4.3.2 and MPFR 2.4.1.
 *
 * @see <a href="http://gmplib.org/">http://gmplib.org/</a>
 * @see <a href="http://www.mpfr.org/">http://www.mpfr.org/</a>
 * @author Antoine Miné
 * @version 0.1
 */
package gmp;
