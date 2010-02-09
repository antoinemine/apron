/*
 * OverflowException.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Exception thrown by an abstract transfer function in
 * case of an arithmetic overflow.
 *
 * <p> It indicates that the expected result cannot be expressed nor
 * soundly approximated due to arithmetic limitations.
 *
 * <p> This can happen when using a domain implementation based on
 * machine, fixed-length types (such as machine integers).
 * However, this cannot happen with arbitrary-precision
 * types (such as GMP) or types that can always provide some
 * sound over approximation (such as machine or multi-precision
 * floating-point numbers).
 */
public class OverflowException 
    extends ApronException
{
    public OverflowException()
    {
        super();
    }

    public OverflowException(String msg, Manager man, int fid)
    { 
        super(msg, man, fid); 
    }

}
