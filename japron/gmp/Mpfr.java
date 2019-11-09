/*
 * Mpfr.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.math.BigInteger;
import java.io.*;

/**
 * MPFR multi-precision floating-point numbers.
 *
 * <p> This class wraps {@code mpfr_t} MPFR objects.
 *
 * <p> The precision of each number is set at construction time and not
 * changed when the number is modified (except by {@link #setPrec} or
 * {@link #set(Mpfr)}).
 * Thus, most operators entail rounding. Such operators take as (last) 
 * argument a rounding direction that can be {@link #RNDN}, {@link #RNDZ}, 
 * {@link #RNDU}, or {@link #RNDD}).
 *
 * <p> Most operators return an integer that tells whether the stored result is
 * exact (zero returned), over-approximated (positive number returned), or
 * under-approximated (negative number returned).
 *
 * <p> In addition, the class has some global (static) exception flags that
 * can be set by methods to denote IEEE 754 exceptions, and then
 * inspected and reset by the user: overflow, underflow, NaN, inexact, 
 * and erange (invalid range) flags (these do not cause Java exceptions 
 * to be thrown).
 */
public class Mpfr
    implements Serializable, Comparable<Mpfr>, Cloneable
{
    
    // Internals
    ////////////

    /**
     * Actually, a pointer to a mpfr_t object allocated in the C heap. 
     */
    private long ptr;

    private native void init();
    private native void init(int prec);
    private native void init_set(Mpfr v, int round);
    private native void init_set(int v, int round);
    private native void init_set(Mpz v, int round);
    private native void init_set(double v, int round);
    private native void init_set(Mpq v, int round);
    private native void init_set(String s, int b, int round);

    /** Deallocates the underlying mpfr_t MPFR object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("jgmp"); class_init(); }


    // Constructors
    ///////////////

    /**
     * Constructs a new number with the default precision. 
     *
     * <p> It is set to NaN. 
     */
    public Mpfr() 
    {
        init();
    }

    /**
     * Constructs a new number with exactly prec bits of precision. 
     *
     * <p> It is set to NaN. 
     */
    public Mpfr(int prec) 
    {
        init(prec); 
    }

    /** Constructs a copy of v, with the same precision as v. */
    public Mpfr(Mpfr v) 
    {
        init(v.getPrec()); set(v, RNDN); 
    }

    /** Constructs a copy of v, rounded to the default precision. */
    public Mpfr(Mpfr v, int round) 
    {
        init_set(v,round); 
    }

    /**
     * Constructs a float with the default precision equal to an integer 
     * (with rounding). 
     */
    public Mpfr(int v, int round) 
    { 
        init_set(v, round); 
    }

    /** 
     * Constructs a float with the default precision equal to an integer 
     * (with rounding). 
     */
    public Mpfr(BigInteger v, int round) 
    {
        init_set(new Mpz(v), round); 
    }

    /**
     * Constructs a float with the default precision equal to an integer 
     * (with rounding). 
     */
    public Mpfr(Mpz v, int round) 
    {
        init_set(v, round); 
    }

    /**
     * Constructs a float with the default precision equal to a double 
     * (with rounding). 
     */
    public Mpfr(double v, int round) 
    { 
        init_set(v, round); 
    }

    /**
     * Constructs a float with the default precision equal to a fraction 
     * (with rounding). 
     */
    public Mpfr(Mpq v, int round) 
    {
        init_set(v, round); 
    }

    /**
     * Constructs a float with the default precision equal to the number 
     * represented in the String s in base b (with rounding). 
     */
    public Mpfr(String s, int b, int round) { init_set(s, b, round); }

    /**
     * Constructs a float with the default precision equal to the number 
     * represented in the decimal String s (with rounding). 
     */
    public Mpfr(String s, int round) { init_set(s, 10, round); }


    /**
     * Creates a new Mpfr initialized with the value of this.
     */
    public Mpfr clone()
        throws CloneNotSupportedException
    {
        return new Mpfr(this);
    }

    
    // Precision functions
    //////////////////////

    /** 
     * Sets the default precision to exactly prec bits. 
     *
     * <p> This only affects objects created after the call.
     * <p> The default is 53 bits initially (i.e., same as double
     * precision IEEE floats).
     */
    static public native void setDefaultPrec(int prec);

    /** Returns the default precision, in bits. */
    static public native int getDefaultPrec();

    /**
     * Changes the precision of this to exactly prec bits. 
     *
     * <p> The value of this is lost and set to NaN.
     */
    public native void setPrec(int prec);

    /** Returns the precision of this, in bits. */
    public native int getPrec();


    // Rounding
    ///////////

    /** Rounds to nearest. */
    static public final int RNDN = 0;
    
    /** Rounds toward zero. */
    static public final int RNDZ = 1;
    
    /** Rounds toward plus infinity. */
    static public final int RNDU = 2;
    
    /** Rounds toward minus infinity. */
    static public final int RNDD = 3;


    // Assignment functions
    ///////////////////////

    /** Copies v into this (with rounding). */
    public native int set(Mpfr v, int round);

    /**
     * Copies v into this, changing the precision of this to match that of v.
     */
    public void set(Mpfr v)
    { 
        if (ptr!=v.ptr) setPrec(v.getPrec()); 
        set(v, RNDN); 
    }

    /** Sets this to an integer (with rounding). */
    public native int set(int v, int round);

    /** Sets this to a v*2^e (with rounding). */
    public native int set(int v, int e, int round);

    /** Sets this to an integer (with rounding). */
    public int set(BigInteger v, int round) 
    {
        return set(new Mpz(v), round); 
    }

    /** Sets this to an integer (with rounding). */
    public native int set(Mpz v, int round);

    /** Sets this to a double (with rounding). */
    public native int set(double v, int round);

    /** Sets this to a fraction (with rounding). */
    public native int set(Mpq v, int round);

    /**
     * Sets this to the number represented in the String s in base b
     * (with rounding). 
     */
    public native void set(String s, int b, int round);

    /**
     * Sets this to the number represented in the decimal String s
     * (with rounding). 
    */
    public void set(String s, int round) { set(s, 10, round); }


    /** Sets this to a signed infinity. */
    public native void setInf(int sign);

    /** Sets this to NaN. */
    public native void setNaN();


    // Get functions
    ////////////////

    /** 
     * Returns this as an integer (with rounding).
     *
     * @see #fitsInt(int)
     */
    public native int intValue(int round);

    /** 
     * Returns this as a double (with rounding). 
     */
    public native double doubleValue(int round);


    /**
     * Converts the integer to a mantissa and exponent.
     * 
     * <p> The mantissa m returned is such that 0.5 &lt;= |m| &lt; 1,
     * rounded to fit double.
     * <p> The exponent is stored into r[0].
     */
    public native double doubleExponentValue(int[] r, int round);


    /** Converts this to an integer (with rounding) and returns it. */
    public native Mpz MpzValue(int round);

    /**
     * Converts exactly this to the form v*2^e, where v is an integer.
     *
     * <p> v is stored into r while e is returned.
    */
    public native int MpzExponentValue(Mpz r);

    /** 
     * Returns a String representation (with rounding). 
     *
     * <p> b is the base, between 2 and 62.
     * <p> n is the number of digits to output, or 0 to denote sufficient
     * precision so that it is possible to recover the original value 
     * when parsing using rounding to nearest.
     */
    public native String toString(int b, int n, int round);

    /**
     * Returns a String representation of this in base 10 
     * (rounding to nearest). 
     */
    public String toString() 
    {
        return toString(10, 0, RNDN); 
    }

    /** Whether this converted to integer (with rounding) fits in an int. */
    public native boolean fitsInt(int round);


    // Basic arithmetic functions
    /////////////////////////////

    /** Sets this to op1+op2. */
    public native int add(Mpfr op1, Mpfr op2, int round);
   
    /** Sets this to op1+op2. */
    public native int add(Mpfr op1, int op2, int round);
   
    /** Sets this to op1+op2. */
    public native int add(Mpfr op1, double op2, int round);
   
    /** Sets this to op1+op2. */
    public native int add(Mpfr op1, Mpz op2, int round);
   
    /** Sets this to op1+op2. */
    public native int add(Mpfr op1, Mpq op2, int round);
   
    /** Adds op to this. */
    public int add(Mpfr op, int round) 
    {
        return add(this, op, round); 
    }
   
    /** Adds op to this. */
    public int add(int op, int round) 
    { 
        return add(this, op, round); 
    }
   
    /** Adds op to this. */
    public int add(double op, int round) 
    {
        return add(this, op, round); 
    }
   
    /** Adds op to this. */
    public int add(Mpz op, int round) 
    {
        return add(this, op, round); 
    }
   
    /** Adds op to this. */
    public int add(Mpq op, int round) 
    {
        return add(this, op, round); 
    }

 
    /** Sets this to op1-op2. */
    public native int sub(Mpfr op1, Mpfr op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(Mpfr op1, int op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(Mpfr op1, double op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(Mpfr op1, Mpz op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(Mpfr op1, Mpq op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(int op1, Mpfr op2, int round);
   
    /** Sets this to op1-op2. */
    public native int sub(double op1, Mpfr op2, int round);
   
    /** Subtracts op from this. */
    public int sub(Mpfr op, int round) 
    {
        return sub(this, op, round); 
    }
   
    /** Subtracts op from this. */
    public int sub(int op, int round) 
    {
        return sub(this, op, round); 
    }
   
    /** Subtracts op from this. */
    public int sub(double op, int round) 
    {
        return sub(this, op, round); 
    }
   
    /** Subtracts op from this. */
    public int sub(Mpz op, int round) 
    {
        return sub(this, op, round); 
    }
   
    /** Subtracts op from this. */
    public int sub(Mpq op, int round) 
    {
        return sub(this, op, round); 
    }

    /** Stores op-this into this. */
    public int subRev(int op, int round) 
    {
        return sub(op, this, round); 
    }
   
    /** Stores op-this into this. */
    public int subRev(double op, int round) 
    {
        return sub(op, this, round); 
    }
   

    /** Sets this to op1*op2. */
    public native int mul(Mpfr op1, Mpfr op2, int round);
   
    /** Sets this to op1*op2. */
    public native int mul(Mpfr op1, int op2, int round);
   
    /** Sets this to op1*op2. */
    public native int mul(Mpfr op1, double op2, int round);
   
    /** Sets this to op1*op2. */
    public native int mul(Mpfr op1, Mpz op2, int round);
   
    /** Sets this to op1*op2. */
    public native int mul(Mpfr op1, Mpq op2, int round);
   
    /** Multiplies op by this. */
    public int mul(Mpfr op, int round) 
    {
        return mul(this, op, round); 
    }
   
    /** Multiplies op by this. */
    public int mul(int op, int round) 
    {
        return mul(this, op, round); 
    }
   
    /** Multiplies op by this. */
    public int mul(double op, int round) 
    {
        return mul(this, op, round);
    }
   
    /** Multiplies op by this. */
    public int mul(Mpz op, int round) 
    {
        return mul(this, op, round); 
    }
   
    /** Multiplies op by this. */
    public int mul(Mpq op, int round) 
    {
        return mul(this, op, round); 
    }

 
    /** Sets this to op1/op2. */
    public native int div(Mpfr op1, Mpfr op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(Mpfr op1, int op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(Mpfr op1, double op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(Mpfr op1, Mpz op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(Mpfr op1, Mpq op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(int op1, Mpfr op2, int round);
   
    /** Sets this to op1/op2. */
    public native int div(double op1, Mpfr op2, int round);
   
    /** Divides this by op. */
    public int div(Mpfr op, int round) 
    {
        return div(this, op, round); 
    }
   
    /** Divides this by op. */
    public int div(int op, int round) 
    {
        return div(this, op, round); 
    }
   
    /** Divides this by op. */
    public int div(double op, int round) 
    {
        return div(this, op, round); 
    }
   
    /** Divides this by op. */
    public int div(Mpz op, int round) 
    {
        return div(this, op, round); 
    }
   
    /** Divides this by op. */
    public int div(Mpq op, int round) 
    {
        return div(this, op, round); 
    }

    /** Stores op/this into this. */
    public int divRev(int op, int round) 
    {
        return div(op, this, round); 
    }
   
    /** Stores op/this into this. */
    public int divRev(double op, int round) 
    {
        return div(op, this, round); 
    }
   

    /** Sets this to the square root of op. */
    public native int sqrt(Mpfr op, int round);

    /** Takes the square root of this. */
    public int sqrt(int round) 
    {
        return sqrt(this, round); 
    }

    /** Sets this to the square root of op. */
    public native int sqrt(int op, int round);

    /** Sets this to the inverted square root of op. */
    public native int recSqrt(Mpfr op, int round);

    /** Takes the inverted square root of this. */
    public int recSqrt(int round)
    {
        return recSqrt(this, round); 
    }


    /** Sets this to the cubic root of op. */
    public native int cbrt(Mpfr op, int round);

    /** Takes the cubic root of this. */
    public int cbrt(int round) 
    {
        return cbrt(this, round); 
    }


    /**
     * Sets this to the k-th root of op. 
     *
     * <p> k must be positive. 
     */
    public native int root(Mpfr op, int k, int round);

    /** 
     * Takes the k-th root of this. 
     *
     * <p> k must be positive. 
     */
    public int root(int k, int round) 
    {
        return root(this, k, round); 
    }

    
    /** Stores op1^op2 into this. */
    public native int pow(Mpfr op1, Mpfr op2, int round);

    /** Stores op1^op2 into this. */
    public native int pow(Mpfr op1, int op2, int round);

    /** Stores op1^op2 into this. */
    public native int pow(Mpfr op1, Mpz op2, int round);


    /** Sets this to -op. */
    public native int neg(Mpfr op, int round);

    /** Negates this. */
    public int neg(int round) { return neg(this, round); }


    /** Sets this to |op|. */
    public native int abs(Mpfr op, int round);

    /** Takes the absolute value of this. */
    public int abs(int round) { return abs(this, round); }


    /** Sets this to max(0, op1-op2). */
    public native int dim(Mpfr op1, Mpfr op2, int round);


    /** Sets this to op1*(2^op2). */
    public native int mul2exp(Mpfr op1, int op2, int round);

    /** Multiplies this by 2^op. */
    public int mul2exp(int op, int round) 
    {
        return mul2exp(this, op, round); 
    }


    /** Sets this to op1/(2^op2). */
    public native int div2exp(Mpfr op1, int op2, int round);

    /** Divides this by 2^op. */
    public int div2exp(int op, int round) 
    {
        return div2exp(this, op, round); 
    }


    // Comparison functions
    ///////////////////////

    /**
     * Compares this to op.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp(Mpfr op);

    /**
     * Compares this to op.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp(int op);

    /**
     * Compares this to op.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp(double op);

    /**
     * Compares this to op.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp(Mpz op);

    /**
     * Compares this to op.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp(Mpq op);


    /**
     * Compares this to op*2^e.
     *
     * <p> Returns a strictly positive, null, or negative number when this is
     * respectively strictly greater, equal, or strictly smalled than op*2^e.
     * <p> A NaN causes erange to be set and 0 to be returned.
     */
    public native int cmp2exp(int op, int e);

    /** Compares |this| to |op|. */
    public native int cmpAbs(Mpfr op);


    /** Returns true if this is a NaN. */
    public native boolean isNaN();

    /** Returns true if this is an infinity. */
    public native boolean isInf();

    /** Returns true if this is a regular number (non-infinity or NaN). */
    public native boolean isNumber();

    /** Returns true if this is +0 or -0. */
    public native boolean isZero();


    /** 
     * Returns a positive value if this is positive, negative if this is
     * negative, and null if this is zero or NaN.
     */
    public native int sgn();


    /**
     * Whether this &gt; op. 
     *
     * <p> false if any operand is NaN. 
     */
    public native boolean isGreater(Mpfr op);

    /** Whether this &gt;= op. 
     *
     * <p> false if any operand is NaN. 
     */
    public native boolean isGreaterEqual(Mpfr op);

    /** Whether this &lt; op. 
     *
     * <p> false if any operand is NaN. 
     */
    public native boolean isLess(Mpfr op);

    /** Whether this &lt;= op. 
     *
     * <p> false if any operand is NaN. 
     */
    public native boolean isLessEqual(Mpfr op);

    /** Whether this &lt; op or this &gt; op. 
     *
     * <p> false if any operand is NaN. 
     */
    public native boolean isLessGreater(Mpfr op);

    /** Whether this == op.  
     *
     * <p> false if any operand is NaN: we have NaN!=NaN. 
     */
    public native boolean isEqual(Mpfr op);

    /** Whether this or op is a NaN. */
    public native boolean isUnordered(Mpfr op);

     /**
      * Whether x is a Mpfr and has the same value as this. 
      * 
      * <p> This compares representations, and so, NaN is assumed equal to 
      * NaN (unlike {@link #isEqual} that follows the IEEE 754 convention).
      */
    public boolean equals(Object x)
    { 
        if (x instanceof Mpfr) {
            Mpfr y = (Mpfr) x;
            return isEqual(y) || (isNaN() && y.isNaN());
        }
        else return false;
    }

    /**
     * Compare the value of this and x.
     *
     * @see #equals(Object)
     */
    public int compareTo(Mpfr x) 
    {
        if (isNaN()) {
            if (x.isNaN()) return 0;
            else return 1;
        }
        if (x.isNaN()) return -1;
        return cmp(x); 
    }

    /** Returns a hash of the value of this. */
    public native int hashCode();


    /** Whether this and x have the same address in memory. */
    public boolean isPhysicalEqual(Mpfr x)
    {
        return ptr == x.ptr; 
    }


    // Special functions
    ////////////////////

    /** Sets this to the natural logarithm of op. */
    public native int log(Mpfr op, int round);
 
    /** Sets this to the logarithm in base 2 of op. */
    public native int log2(Mpfr op, int round);
 
    /** Sets this to the logarithm in base 10 of op. */
    public native int log10(Mpfr op, int round);


    /** Sets this to e^op. */
    public native int exp(Mpfr op, int round);
 
    /** Sets this to 2^op. */
    public native int exp2(Mpfr op, int round);
 
    /** Sets this to 10^op. */
    public native int exp10(Mpfr op, int round);


    /** Sets this to the cosine of op. */
    public native int cos(Mpfr op, int round);

    /** Sets this to the sine of op. */
    public native int sin(Mpfr op, int round);

    /** Sets this to the tangent of op. */
    public native int tan(Mpfr op, int round);

    /** Sets this to the secant of op. */
    public native int sec(Mpfr op, int round);

    /** Sets this to the cosecant op. */
    public native int csc(Mpfr op, int round);

    /** Sets this to the cotangent of op. */
    public native int cot(Mpfr op, int round);

    /** Sets this to the sine of op, and c to its cosine. */
    public native int sinCos(Mpfr c, Mpfr op, int round);

    /** Sets this to the arc-cosine of op. */
    public native int acos(Mpfr op, int round);

    /** Sets this to the arc-sine of op. */
    public native int asin(Mpfr op, int round);

    /** Sets this to the arc-tangent of op. */
    public native int atan(Mpfr op, int round);

    /** Sets this to the arc-tangent2 of y and x. */
    public native int atan2(Mpfr y, Mpfr x, int round);

    /** Sets this to the hyperbolic cosine of op. */
    public native int cosh(Mpfr op, int round);

    /** Sets this to the hyperbolic sine of op. */
    public native int sinh(Mpfr op, int round);

    /** Sets this to the hyperbolic sine of op, and c to its hyperbolic cosine. */
    public native int sinhCosh(Mpfr c, Mpfr op, int round);

    /** Sets this to the hyperbolic secant of op. */
    public native int sech(Mpfr op, int round);

    /** Sets this to the hyperbolic cosecant op. */
    public native int csch(Mpfr op, int round);

    /** Sets this to the hyperbolic cotangent of op. */
    public native int coth(Mpfr op, int round);

     /** Sets this to the hyperbolic arc-cosine of op. */
    public native int acosh(Mpfr op, int round);

    /** Sets this to the hyperbolic arc-sine of op. */
    public native int asinh(Mpfr op, int round);

    /** Sets this to the hyperbolic arc-tangent of op. */
    public native int atanh(Mpfr op, int round);

    
    /**
     * Sets this to i!. 
     *
     * <p> i must be positive. 
     */
    public native int fac(int i, int round);


    /** Sets this to log(1+op). */
    public native int log1p(Mpfr op, int round);

    /** Sets this to exp(op-1). */
    public native int expm1(Mpfr op, int round);

    /** Sets this to the exponential integral of op. */
    public native int eint(Mpfr op, int round);

    /** Sets this to the real part of the dilogarithm of op. */
    public native int li2(Mpfr op, int round);

    /** Sets this to the value of the Gamma function at op. */
    public native int gamma(Mpfr op, int round);

    /** Sets this to the logarithm of the value of the Gamma function at op. */
    public native int lngamma(Mpfr op, int round);

    /** 
     * Sets this to the logarithm of the absolute value of the Gamma 
     * function at op. 
     *
     * <p> Also stores the sign of the value into sign[0].
     */
    public native int lgamma(int[] sign, Mpfr op, int round);

    /** Sets this to the value of the Riemann Zeta function at op. */
    public native int zeta(Mpfr op, int round);

    /** 
     * Sets this to the value of the Riemann Zeta function at op. 
     *
     * <p> op must be positive.
    */
    public native int zeta(int op, int round);

    /** Sets this to the value of the error function at op. */
    public native int erf(Mpfr op, int round);

    /** Sets this to the value of the complementary error function at op. */
    public native int erfc(Mpfr op, int round);


    /**
     * Sets this to the value of the first kind Bessel function of order 0
     *  at op. 
     */
    public native int j0(Mpfr op, int round);

    /**
     * Sets this to the value of the first kind Bessel function of order 1
     * at op. 
     */
    public native int j1(Mpfr op, int round);

    /** 
     * Sets this to the value of the first kind Bessel function of order n
     * at op. 
     */
    public native int jn(int n, Mpfr op, int round);

    /**
     * Sets this to the value of the second kind Bessel function of order 0
     *  at op. 
     */
    public native int y0(Mpfr op, int round);

    /** 
     * Sets this to the value of the second kind Bessel function of order 1
     * at op. 
     */
    public native int y1(Mpfr op, int round);

    /**
     * Sets this to the value of the second kind Bessel function of order n
     * at op. 
     */
    public native int yn(int n, Mpfr op, int round);


    /** Sets this to (op1*op2)+op3. */
    public native int fma(Mpfr op1, Mpfr op2, Mpfr op3, int round);

    /** Sets this to (op1*op2)-op3. */
    public native int fms(Mpfr op1, Mpfr op2, Mpfr op3, int round);


    /** Sets this to the arithmetic-geometric mean of op1 and op2. */
    public native int agm(Mpfr op1, Mpfr op2, int round);


    /** Sets this to the Euclidean norm of x and y. */
    public native int hypot(Mpfr y, Mpfr x, int round);


    /** Sets this to the constant log(2). */
    public native int constLog2(int round);

    /** Sets this to the constant pi. */
    public native int constPi(int round);

    /** Sets this to Euler's constant. */
    public native int constEuler(int round);

    /** Sets this to Catalan's constant. */
    public native int constCatalan(int round);


    /** 
     * Free various caches used internally to compute functions 
     * and constants.
     */
    static public native void freeCache();

    /** Stores into this the sum of an array or Mpfr (correctly rounded). */
    public native int sum(Mpfr[] ar, int round);


    // Integer and remainder related functions
    //////////////////////////////////////////

    /** 
     * Stores into this, op rounded to the nearest representable
     * integer for the given rounding mode. 
     */
    public native int rint(Mpfr op, int round);

    /** Stores into this, op rounded to the next representable integer. */
    public native int ceil(Mpfr op);

    /** Stores into this, op rounded to the previous representable integer. */
    public native int floor(Mpfr op);

    /** Stores into this, op rounded to the nearest representable integer. */
    public native int round(Mpfr op);

    /** Stores into this, op truncated to a representable integer. */
    public native int trunc(Mpfr op);

    /** Stores into this, the integer following op, then rounded. */
    public native int rint_ceil(Mpfr op, int round);

    /** Stores into this, the integer preceding op, then rounded. */
    public native int rint_floor(Mpfr op, int round);

    /** Stores into this, the integer nearest op, then rounded. */
    public native int rint_round(Mpfr op, int round);

    /** Stores into this, op truncated to an integer, then rounded. */
    public native int rint_trunc(Mpfr op, int round);


    /**
     * Stores into this, the fractional part of op with the same sign as op,
     * and rounded.
     */
    public native int frac(Mpfr op, int round);

    /** Sets this to the integral part of op, and f to its fractional part. */
    public native int modf(Mpfr f, Mpfr op, int round);

    /** Sets this to x-y*trunc(x/y). */
    public native int fmod(Mpfr x, Mpfr y, int round);

    /** Sets this to x-y*round(x/y). */
    public native int remainder(Mpfr x, Mpfr y, int round);

    /** 
     * Sets this to x-y*round(x/y). 
     *
     * <p> Stores into q[0] the low bits of the (signed) quotient round(x/y).
     */
    public native int remquo(int[] q, Mpfr x, Mpfr y, int round);


    /** Whether this is an integer. */
    public native boolean isInteger();


    // Rounding related functions
    /////////////////////////////


    /** Rounds this to prec bits and changes its precision. */
    public native int precRound(int prec, int round);

    /**
     * Whether this can be rounded without loss of precision.
     *
     * <p> Assuming that this approximates some number x in the direction 
     * round1 with error at most 2^(expb-err), where expb is the exponent of b,
     * returns true if x can be correctly rounded to prec bits in the
     * direction round2. Does not modify this.
     */
    public native boolean canRound(int err, int round1, int round2, int prec);

    /** Returns the name of the rounding mode. */
    static public native String toStringRndMode(int round);


    // Miscellaneous functions
    //////////////////////////


    /** 
     * Sets this to the next representable float following x 
     * in the direction of y. 
     */
    public native void nextToward(Mpfr op);

    /** Sets this to the representable float following x. */
    public native void nextAbove();

    /** Sets this to the representable float preceding x. */
    public native void nextBelow();


    /** Sets this to the minimum of op1 and op2. */
    public native int min(Mpfr op1, Mpfr op2, int round);

    /** Sets this to the maximum of op1 and op2. */
    public native int max(Mpfr op1, Mpfr op2, int round);

    /** Returns the exponent of this. */
    public native int getExp();

    /** Sets the exponent of this to e. */
    public native boolean setExp(int e);

    /** Returns true if this has its sign bit set. */
    public native boolean signBit();

    /** Sets this to op (rounded) and then sets the bit-sign of this to s. */
    public native int setSign(Mpfr op, boolean s, int round);

    /** 
     * Sets this to op1 (rounded), and then sets the bit-sign of this to
     * that of op2.
     */
    public native int copySign(Mpfr op1, Mpfr op2, int round);

    /** Returns the MPFR version. */
    static public native String getVersion();

    /** Returns the ids of the patches applied to the MPFR library. */
    static public native String getPatches();


    // Exception related functions
    //////////////////////////////


    /**
     *  Returns the current smallest exponent allowed for a floating-point 
     * variable. 
     */
    static public native long getEmin();

    /** 
     * Returns the current largest exponent allowed for a floating-point 
     * variable. 
     */
    static public native long getEmax();

    /**
     * Sets the current smallest exponent allowed for a for a floating-point 
     * variable.
     *
     * <p> Returns true if the change was successful.
     */
    static public native boolean setEmin(long exp);
 
    /** 
     * Sets the current largest exponent allowed for a for a floating-point 
     * variable.
     *
     * <p> Returns true if the change was successful.
     */
    static public native boolean setEmax(long exp);

    /** Returns the smallest value allowed in {@link #setEmin}. */
    static public native long getEminMin();

    /** Returns the largest value allowed in {@link #setEmin}. */
    static public native long getEminMax();

    /** Returns the smallest value allowed in {@link #setEmax}. */
    static public native long getEmaxMin();

    /** Returns the largest value allowed in {@link #setEmax}. */
    static public native long getEmaxMax();

    /** Forces this to be in the current range of acceptable values. */
    public native int checkRange(int t, int round);

    /** Rounds this to emulate subnormal arithmetic. */
    public native int subNormalize(int t, int round);

    /** Clears the underflow flag. */
    static public native void clearUnderflow();

    /** Clears the overflow flag. */
    static public native void clearOverflow();

    /** Clears the invalid flag. */
    static public native void clearNaNFlag();

    /** Clears the inexact flag. */
    static public native void clearInexFlag();

    /** Clears the erange (invalid range) flag. */
    static public native void clearERangeFlag();


    /** Sets the underflow flag. */
    static public native void setUnderflow();

    /** Sets the overflow flag. */
    static public native void setOverflow();

    /** Sets the invalid flag. */
    static public native void setNaNFlag();

    /** Sets the inexact flag. */
    static public native void setInexFlag();

    /** Sets the erange (invalid range) flag. */
    static public native void setERangeFlag();


    /** Returns the underflow flag. */
    static public native boolean isUnderflow();

    /** Returns the overflow flag. */
    static public native boolean isOverflow();

    /** Returns the invalid flag. */
    static public native boolean isNaNFlag();

    /** Returns the inexact flag. */
    static public native boolean isInexFlag();

    /** Returns the erange (invalid range) flag. */
    static public native boolean isERangeFlag();


    /** Stores into this a uniformly distributed random number in [0, 1]. */
    public native int random(RandState r);


    // Import and export
    /////////////////////
    
    /**
     * Serializes this. 
     *
     * <p> Not only the value, but also the precision is stored.
     */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        /* format:
           - precision
           - string in base 16
        */
        out.writeInt(getPrec());
        out.writeObject(toString(16, 0, RNDN));
    }
    
    /**
     * Reads back a serialized Mpfr. 
     *
     * <p> Not only the value, but also the precision is restored.
     */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        init(in.readInt());
        set((String)(in.readObject()), 16, RNDN);
    }

    /**
     * Fall-back deserialization function: sets to NaN with the default
     * precision. 
     */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init(); 
    }

}
