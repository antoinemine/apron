/*
 * Mpz.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.math.BigInteger;
import java.io.*;

/**
 * GMP arbitrary precision integers.
 *
 * <p> This class wraps {@code mpz_t} GMP objects.
 */
public class Mpz 
    implements Serializable, Comparable<Mpz>, Cloneable
{
    
    // Internals
    ////////////

    /** 
     * Actually, a pointer to a mpz_t object allocated in the C heap. 
     */
    private long ptr;

    private native void init();
    private native void init_set(Mpz v);
    private native void init_set(int i);
    private native void init_set(double d);
    private native void init_set(String s, int b);

    /**
     * Sets the value of this given a magnitude and a sign.
     *
     * <p> Used during deserialization.
     * <p> mag is the absolute value encoded in a byte array in 
     * big endian order.
     **/
    public native void set(int sign, byte[] mag);

    /** 
     * Returns the magnitude of this.
     *
     * <p> User during serialization.
     * <p> The magnitude is the absolute value, and it is returned
     * as a byte array stored in big-endian order.
     */
    public native byte[] byteArrayValue();

    /** Deallocates the underlying mpz_t GMP object. */
    protected native void finalize();

    private static native void class_init();
    
    static { System.loadLibrary("jgmp"); class_init(); }


    // Constructors
    ///////////////

    /** Constructs an uninitialized number. */
    public Mpz()
    {
        init(); 
    }
    
    /** Constructs a copy of its argument. */
    public Mpz(Mpz v)
    {
        init_set(v); 
    }

    /** Constructs a number initialized with an integer. */
    public Mpz(int i) 
    { 
        init_set(i); 
    }

    /** 
     * Constructs a number initialized with a double.
     *
     * <p>The value is truncated to an integer. 
    */
    public Mpz(double d) 
    {
        init_set(d); 
    }

    /** Constructs a number initialized from a Java big integer. */
    public Mpz(BigInteger b)
    { 
        init(); 
        set(b.signum(), b.abs().toByteArray()); 
    }

    /** 
     * Constructs a number initialized from a string representation in base b.
     *
     * @param b must be either 0, or between 2 and 62.
     * When b is 0, the leading characters are used to determine the base
     * (0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal,
     * decimal otherwise).
     */
    public Mpz(String s, int b) 
    {
        init_set(s, b); 
    }

    /** Constructs a number initialized from a string representation. 
     *
     * <p> The leading characters are used to determine whether base:
     * 0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, and
     * decimal otherwise.
     */
    public Mpz(String s) 
    { 
        init_set(s, 0); 
    }

    /**
     * Constructs a number from a unsigned stored in a big-endian byte array 
     * and a sign. 
     */
    public Mpz(int sign, byte[] mag) 
    { 
        init(); 
        set(sign, mag); 
    }

    /**
     * Creates a new Mpz initialized with the value of this.
     */
    public Mpz clone()
        throws CloneNotSupportedException
    {
        return new Mpz(this);
    }

    // Set functions
    ////////////////

    /** Copies the argument into this. */
    public native void set(Mpz v);

    /** Sets this to an integer. */
    public native void set(int i);

    /** 
     * Sets this to a double. 
     * 
     * <p> The value is truncated. 
     */
    public native void set(double d);

    /** Sets this to a Java big integer. */
    public void set(BigInteger b)
    { 
        set(b.signum(), b.abs().toByteArray()); 
    }

    /** 
     * Sets this to the String s expressed in base b. 
     *
     * @see #Mpz(String,int)
     */
    public native void set(String s, int b);

    /** 
     * Sets this to the String s.
     *
     * @see #Mpz(String)
     */
    public void set(String s)
    {
        set(s, 0);
    }


    // Get functions
    ////////////////

    /** 
     * Returns this as an integer.   
     *
     * <p> The result is truncated if it does not fit in an integer.
     * @see #fitsInt()
     */
    public native int intValue();

    /** 
     * Returns this as a double. 
     *
     * <p> If this cannot be represented as a double, the result is truncated.
     * <p> Overflows cause infinities to be returned.
     */
    public native double doubleValue();

    /**
     * Converts the integer to a mantissa and exponent.
     * 
     * <p> The mantissa m returned is such that 0.5 &lt;= |m| &lt; 1,
     * truncated to fit double.
     * <p> The exponent is stored into r[0].
     */
    public native double doubleExponentValue(int[] r);

    /** Returns this as a Java big integer. */
    public BigInteger bigIntegerValue()
    { return new BigInteger(sgn(), byteArrayValue()); }

    /** 
     * Returns a String representation of this in base b. 
     *
     * @param b must be between 2 and 62.
     */
    public native String toString(int b);

    /** Returns a String representation this in base 10. */
    public String toString() 
    {
        return toString(10); 
    }


    // Arithmetic functions
    ///////////////////////

    /** Stores arg1+arg2 into this. */
    public native void add(Mpz arg1, Mpz arg2);

    /** Stores arg1+arg2 into this. */
    public native void add(Mpz arg1, int arg2);

    /** Adds arg to this. */
    public void add(Mpz arg) 
    {
        add(this, arg); 
    }

    /** Adds arg to this. */
    public void add(int arg) 
    {
        add(this, arg); 
    }


    /** Stores arg1-arg2 into this. */
    public native void sub(Mpz arg1, Mpz arg2);

    /** Stores arg1-arg2 into this. */
    public native void sub(Mpz arg1, int arg2);

    /** Stores arg1-arg2 into this. */
    public native void sub(int arg1, Mpz arg2);

    /** Subtracts arg from this. */
    public void sub(Mpz arg) 
    {
        sub(this, arg); 
    }

    /** Subtracts arg from this. */
    public void sub(int arg) 
    {
        sub(this, arg); 
    }

    /** Replaces this with arg-this. */
    public void subRev(int arg)
    {
        sub(arg, this); 
    }


    /** Stores arg1*arg2 into this. */
    public native void mul(Mpz arg1, Mpz arg2);

    /** Stores arg1*arg2 into this. */
    public native void mul(Mpz arg1, int arg2);

    /** Multiplies this by arg. */
    public void mul(Mpz arg) 
    {
        mul(this, arg); 
    }

    /** Multiplies this by arg. */
    public void mul(int arg) 
    {
        mul(this, arg); 
    }


    /** Stores this+arg1*arg2 into this. */
    public native void addMul(Mpz arg1, Mpz arg2);

    /** Stores this+arg1*arg2 into this. */
    public native void addMul(Mpz arg1, int arg2);

    /** Stores this-arg1*arg2 into this. */
    public native void subMul(Mpz arg1, Mpz arg2);

    /** Stores this-arg1*arg2 into this. */
    public native void subMul(Mpz arg1, int arg2);


    /**
     * Stores arg1*2^arg2 into this. 
     *
     * <p> arg2 must be positive. 
     */
    public native void mul2exp(Mpz arg1, int arg2);

    /**
     * Multiplies this by 2^arg.
     *
     * <p> arg must be positive. 
     */
    public void mul2exp(int arg) 
    {
        mul2exp(this, arg); 
    }


    /** Stores -arg into this. */
    public native void neg(Mpz arg);

    /** Replaces this with -this. */
    public void neg()
    {
        neg(this); 
    }


    /** Stores |arg| into this. */
    public native void abs(Mpz arg);

    /** Replaces this with |this|. */
    public void abs() 
    {
        abs(this); 
    }


    // Division functions
    /////////////////////

    /** Stores the quotient of n/d rounded towards +oo into this. */ 
    public native void cdivQ(Mpz n, Mpz d) throws ArithmeticException;

    /** Divides this by d and rounds towards +oo. */ 
    public void cdivQ(Mpz d) throws ArithmeticException
    {
        cdivQ(this, d); 
    }

    /** Stores the remainder of n/d rounded towards +oo into this. */ 
    public native void cdivR(Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards +oo into this and the
     * remainder into r.
     */ 
    public native void cdivQR(Mpz r, Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards +oo into this, 
     * and returns the remainder.
     *
     * <p> d must be positive.
     */ 
    public native int cdivQ(Mpz n, int d) throws ArithmeticException;

    /**
     * Divides this by d, rounds towards +oo, and returns the remainder.
     *
     * <p> d must be positive.
     */ 
    public int cdivQ(int d) throws ArithmeticException
    { 
        return cdivQ(this, d); 
    }

    /**
     * Stores the remainder of n/d rounded towards +oo into this, 
     * and returns the remainder.
     *
     * <p> d must be positive.
     */ 
    public native int cdivR(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards +oo into this, the
     * remainder into r, and returns the remainder. 
     *
     * <p> d must be positive.
     */ 
    public native int cdivQR(Mpz r, Mpz n, int d) throws ArithmeticException;

    /**
     * Returns the remainder of of this/d rounded towards +oo.
     * 
     * <p> this is unchanged. 
     * <p> d must be positive.
     */
    public native int cdivR(int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/(2^d) rounded towards +oo into this. 
     *
     * <p> d must be positive.
     */ 
    public native void cdivQ2exp(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the remainder of n/(2^d) rounded towards +oo into this. 
     *
     * <p> d must be positive.
     */ 
    public native void cdivR2exp(Mpz n, int d) throws ArithmeticException;
    

    /**
     * Stores the quotient of n/d rounded towards -oo into this. 
     */ 
    public native void fdivQ(Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Divides this by d and rounds towards -oo. 
     */ 
    public void fdivQ(Mpz d) throws ArithmeticException
    {
        fdivQ(this, d); 
    }

    /**
     * Stores the remainder of n/d rounded towards -oo into this. 
     */ 
    public native void fdivR(Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards -oo into this and the
     * remainder into r.
    */ 
    public native void fdivQR(Mpz r, Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards -oo into this, 
     * and returns the remainder. 
     *
     * <p> d must be positive.
    */ 
    public native int fdivQ(Mpz n, int d) throws ArithmeticException;

    /**
     * Divides this by d, rounds towards -oo, and returns the remainder. 
     *
     * <p> d must be positive.
    */ 
    public int fdivQ(int d) throws ArithmeticException
    {
        return fdivQ(this, d); 
    }

    /**
     * Stores the remainder of n/d rounded towards -oo into this, 
     * and returns the remainder. 
     *
     * <p> d must be positive.
    */ 
    public native int fdivR(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards -oo into this, the
     * remainder into r, and returns the remainder.
     *
     * <p> d must be positive.
    */ 
    public native int fdivQR(Mpz r, Mpz n, int d) throws ArithmeticException;

    /**
     * Returns the remainder of of this/d rounded towards -oo.
     *
     * <p> this is unchanged. 
     * <p> d must be positive.
    */
    public native int fdivR(int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/(2^d) rounded towards -oo into this. 
     */ 
    public native void fdivQ2exp(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the remainder of n/(2^d) rounded towards -oo into this. 
     *
     * <p> d must be positive.
     */ 
    public native void fdivR2exp(Mpz n, int d) throws ArithmeticException;
    

    /**
     * Stores the quotient of n/d rounded towards 0 into this. 
     */ 
    public native void tdivQ(Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Divides this by d and rounds towards 0. 
     */ 
    public void tdivQ(Mpz d) throws ArithmeticException
    {
        tdivQ(this, d); 
    }

    /**
     * Stores the remainder of n/d rounded towards 0 into this. 
     */ 
    public native void tdivR(Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards 0 into this and the
     * remainder into r.
    */ 
    public native void tdivQR(Mpz r, Mpz n, Mpz d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards 0 into this, 
     * and returns the remainder. 
     *
     * <p> d must be positive.
     */ 
    public native int tdivQ(Mpz n, int d) throws ArithmeticException;

    /**
     * Divides this by d, rounds towards 0, and returns the remainder. 
     *
     * <p> d must be positive.
     */ 
    public int tdivQ(int d) throws ArithmeticException
    { 
        return tdivQ(this, d); 
    }

    /**
     * Stores the remainder of n/d rounded towards 0 into this, 
     * and returns the remainder. 
     *
     * <p> d must be positive.
    */ 
    public native int tdivR(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/d rounded towards 0 into this, the
     * remainder into r, and returns the remainder. 
     *
     * <p> d must be positive.
     */ 
    public native int tdivQR(Mpz r, Mpz n, int d) throws ArithmeticException;

    /** 
     * Returns the remainder of of this/d rounded towards 0.
     *
     * <p> this is unchanged.
     * <p> d must be positive.
     */
    public native int tdivR(int d) throws ArithmeticException;

    /**
     * Stores the quotient of n/(2^d) rounded towards 0 into this.
     *
     * <p> d must be positive.
     */ 
    public native void tdivQ2exp(Mpz n, int d) throws ArithmeticException;

    /**
     * Stores the remainder of n/(2^d) rounded towards 0 into this. 
     *
     * <p> d must be positive.
     */ 
    public native void tdivR2exp(Mpz n, int d) throws ArithmeticException;
    

    /** Stores n mod |d| into this. */
    public native void mod(Mpz n, Mpz d) throws ArithmeticException;

    /** Replaces this with this mod |d|. */
    public void mod(Mpz d) throws ArithmeticException
    { 
        mod(this, d); 
    }

    /** Stores n mod |d| into this and returns it. */
    public native int mod(Mpz n, int d) throws ArithmeticException;

   /** Replaces this with this mod |d|. */
    public int mod(int d) throws ArithmeticException
    {
        return mod(this, d); 
    }


    /** Stores n/d into this, assuming that d divides n. */
    public native void divExact(Mpz n, Mpz d) throws ArithmeticException;

    /** Divides this by d, assuming that d divides this. */
    public void divExact(Mpz d) throws ArithmeticException
    { 
        divExact(this, d); 
    }

    /** Stores n/d into this, assuming that d divides n. d must be positive. */
    public native void divExact(Mpz n, int d) throws ArithmeticException;

    /** Divides this by d, assuming that d divides this. d must be positive. */
    public void divExact(int d) throws ArithmeticException
    {
        divExact(this, d); 
    }


    /**
     * Returns true if d divides this. 
     *
     * <p> Only 0 is divisible by 0. 
     */
    public native boolean isDivisible(Mpz d);

    /**
     * Returns true if d divides this. 
     *
     * <p> d must be positive. 
     * <p> Only 0 is divisible by 0. 
     */
    public native boolean isDivisible(int d);

    /**
     * Returns true if 2^d divides this. 
     *
     * <p> d must be positive. 
     */
    public native boolean isDivisible2exp(int d);


    /**
     * Returns true if this equals c modulo d.
     *
     * <p> When d=0, returns true if this equals c. 
     */
    public native boolean isCongruent(Mpz c, Mpz d);

    /**
     * Returns true if this equals c modulo d. 
     *
     * <p> d must be positive. 
     * <p> When d=0, returns true if this equals c. 
    */
    public native boolean isCongruent(int c, int d);

    /**
     * Returns true if this equals c modulo 2^d.  
     *
     * <p> d must be positive. 
     */
    public native boolean isCongruent2exp(Mpz c, int d);


    // Exponentiation functions
    ///////////////////////////

    /**
     * Stores b^e mod m into this.
     *
     * <p> Throws an ArithmeticException if e &lt; 0 and b has no inverse modulo m.
     */
    public native void pow(Mpz b, Mpz e, Mpz m) throws ArithmeticException;

    /**
     * Stores b^e mod m into this. 
     *
     * <p> e must be positive. 
     */
    public native void pow(Mpz b, int e, Mpz m) throws ArithmeticException;

    /**
     * Stores b^e into this. 
     *
     * <p> e must be positive. 
     * <p> 0^0 is 1. 
     */
    public native void pow(Mpz b, int e);
    
    /** 
     * Takes the e-th power of this. 
     *
     * <p> e must be positive. 
     * <p> 0^0 is 1. 
     */
    public void pow(int e)
    { 
        pow(this, e); 
    }

    /**
     * Stores b^e into this. 
     *
     * <p> b and e must be positive. 
     * <p> 0^0 is 1. 
     */
    public native void pow(int b, int e);


    // Root extraction functions
    ////////////////////////////

    /**
     * Stores the n-th root of op into this, truncated.
     * 
     * <p> Returns true if the computation was exact. 
     * <p> n must be positive.
     */
    public native boolean root(Mpz op, int n);

    /** 
     * Takes the n-th root of this, truncated.
     *
     * <p> Returns true if the computation was exact. 
     * <p> n must be positive.
    */
    public boolean root(int n)
    { return root(this, n); }

    /** 
     * Stores the n-th root of op into this, truncated, and the remainder
     * op-this^n into r. 
     *
     * <p> n must be positive.
     */
    public native void root(Mpz r, Mpz op, int n);


    /** Stores the truncated square root of op into this. */
    public native void sqrt(Mpz op);

    /** Takes the truncated square root of this. */
    public void sqrt()
    { 
        sqrt(this); 
    }

    /** 
     * Stores the square root of op into this, truncated, and the remainder
     * op-this^2 into r.
     */
    public native void sqrt(Mpz r, Mpz op);


    /**
     * Returns true if this has the form a^b for some a and b &gt; 1. 
     *
     * <p> 0 and 1 are perfect powers. 
     */
    public native boolean isPerfectPower();
    
    
    /**
     * Returns true if this has the form a^2 for some a. 
     *
     * <p> 0 and 1 are perfect squares. 
     */
    public native boolean isPerfectSquare();


    // Number theoretic functions
    /////////////////////////////

    /**
     * Returns 2 if this is definitely prime, 1 if it is probably prime,
     * and 0 if it is definitely composite.
     *
     * <p> r is the number of tests; high numbers increasing the certainty.
     * A value between 5 and 10 is reasonable.
    */
    public native int isProbabPrime(int r);

    /**
     * Stores into this the next prime greater than op.
     * 
     * <p> This uses a probabilistic algorithm so that the chance of getting a
     * composite are non-null but extremely small.
     */
    public native void nextPrime(Mpz op);


    /**
     * Stores into this the greatest common divisor of op1 and op2.
     *
     * <p> The result is always positive, even for negative inputs.
     */
    public native void gcd(Mpz op1, Mpz op2);
    
    /**
     * Returns the greatest common divisor of op1 and op2. 
     *
     * <p> this is unchanged. 
     * <p> op2 must be strictly positive.
    */
    public native int gcd(Mpz op1, int op2);

    /**
     * Stores the greatest common divisor of op1 and op2 into this.
     *
     * <p> Sets s and t to values such that s*op1+t*op2 = gcd and 
     * |s| &lt;= |op2|, |t| &lt;= |op1|.
     */
    public native void gcd(Mpz s, Mpz t, Mpz op1, Mpz op2);

    /** Stores into this the least common multiple of op1 and op2. */
    public native void lcm(Mpz op1, Mpz op2);

    /** 
     * Stores into this the least common multiple of op1 and op2. 
     *
     * <p> op2 must be positive. 
    */
    public native void lcm(Mpz op1, int op2);

    
    /** 
     * Stores the inverse of op1 modulo op2 into this.
     *
     * <p> Returns true if this inverse exists. 
     * Otherwise, returns false; this is undefined.
    */
    public native boolean invert(Mpz op1, Mpz op2);


    /**
     * Returns the Jacobi symbol of a and b.
     *
     * <p> b must be odd. 
     */
    static public native int jacobi(Mpz b, Mpz p);


    /**
     * Returns the Legendre symbol. 
     *
     * <p> p must be an odd positive prime. 
     */
    static public native int legendre(Mpz a, Mpz p);


    /** Returns the Jacobi symbol with the Kronecker extension. */
    static public native int kronecker(Mpz a, Mpz b);

    /** Returns the Jacobi symbol with the Kronecker extension. */
    static public native int kronecker(Mpz a, int b);

    /** Returns the Jacobi symbol with the Kronecker extension. */
    static public native int kronecker(int a, Mpz b);

    
    /**
     * Stores into this op with all occurrences of factor f removed.
     *
     * <p> Returns the number of removed factors.
     */
    public native int remove(Mpz op, Mpz f);

    /**
     * Removes from this all occurrences of factor f.
     *
     * <p> Returns the number of removed factors.
     */
    public int remove(Mpz f)
    { 
        return remove(this, f); 
    }


    /**
     * Stores op! into this.
     *
     * <p> op must be positive. 
     */
    public native void fac(int op);


    /** 
     * Stores into this the binomial coefficient (n k). 
     *
     * <p>k must be positive. 
     */
    public native void bin(Mpz n, int k);

    /** 
     * Stores into this the binomial coefficient (n k). 
     *
     * <p> n and k must be positive.
     */
    public native void bin(int n, int k);


    /** 
     * Stores into this the n-th Fibonacci number. 
     *
     * <p> n must be positive. 
     */
    public native void fib(int n);

    /** 
     * Stores into this the n-th Fibonacci number, and into r the n-1-th
     * Fibonacci number. 
     *
     * <p> n must be positive. 
    */
    public native void fib(Mpz r, int n);


    /**
     * Stores into this the n-th Lucas number.
     *
     * <p> n must be positive. 
     */
    public native void lucnum(int n);

    /** 
     * Stores into this the n-th Lucas number, and into r the n-1-th
     * Lucas number. 
     *
     * <p> n must be positive. 
     */
    public native void lucnum(Mpz r, int n);


    // Comparison functions
    ///////////////////////
    
    /** 
     * Returns a positive, null, or negative number when this is respectively
     * strictly greater, equal, or strictly smaller than op.
     */
    public native int cmp(Mpz op);

    /**
     * Returns a positive, null, or negative number when this is respectively
     * strictly greater, equal, or strictly smaller than op.
     */
    public native int cmp(int op);

    /**
     * Returns a positive, null, or negative number when this is respectively
     * strictly greater, equal, or strictly smaller than op.
     */
    public native int cmp(double op);


    /**
     * Returns a positive, null, or negative number when |this| is respectively
     * strictly greater, equal, or strictly smaller than |op|.
     */
    public native int cmpAbs(Mpz op);

    /**
     * Returns a positive, null, or negative number when |this| is respectively
     *  strictly greater, equal, or strictly smaller than |op|.
     */
    public native int cmpAbs(int op);

    /**
     * Returns a positive, null, or negative number when |this| is respectively
     * strictly greater, equal, or strictly smaller than |op|.
     */
    public native int cmpAbs(double op);


    /**
     * Returns +1, 0, or -1 when this is respectively strictly positive, null,
     * or strictly negative.
     */
    public native int sgn();

    /** Whether this and x have the same value. */
    public boolean isEqual(Mpz x)
    { 
        return cmp(x)==0; 
    }

    /** Whether x is a Mpz and has the same value as this. */
    public boolean equals(Object x)
    { 
        return (x instanceof Mpz) && cmp((Mpz)x)==0; 
    }


    /** Compare the value of this and x. */
    public int compareTo(Mpz x) 
    {
        return cmp(x); 
    }


    /** Returns a hash of the value of this. */
    public native int hashCode();


    /** Whether this and x have the same address in memory. */
    public boolean isPhysicalEqual(Mpz x)
    { 
        return ptr == x.ptr; 
    }


    // Logical and bit manipulation functions
    //////////////////////////////////////////

    /** 
     * This value can be returned by bit-searching or bit-counting operations. 
     */
    static public final int infinite = -1;

    /** Stores the bitwise and of op1 and op2 into this. */
    public native void and(Mpz op1, Mpz op2);

    /** Stores the bitwise and of this and op into this. */
    public void and(Mpz op) 
    {
        and(this,op); 
    }


    /** Stores the bitwise or of op1 and op2 into this. */
    public native void or(Mpz op1, Mpz op2);

    /** Stores the bitwise or of this and op into this. */
    public void or(Mpz op) 
    {
        or(this,op); 
    }


    /** Stores the bitwise xor of op1 and op2 into this. */
    public native void xor(Mpz op1, Mpz op2);

    /** Stores the bitwise xor of this and op into this. */
    public void xor(Mpz op) 
    {
        xor(this,op); 
    }


    /** Stores the bitwise complement of op into this. */
    public native void not(Mpz op);

    /** Stores the bitwise complement of this into this. */
    public void not() 
    {
        not(this); 
    }


    /**
     * Returns the number of bits set to 1 in this.
     *
     * <p> Returns {@link #infinite} if this is negative.
     */
    public native int popCount();


    /** 
     * Returns the Hamming distance between this and op.
     *
     * <p> Returns {@link #infinite} if this and op have different signs.
     */
    public native int hamDist(Mpz op);


    /**
     * Returns the position of the first bit set to 0 at or after position 
     * pos.
     *
     * <p> pos must be positive. 
     * <p> May return {@link #infinite}, if this is negative.
     */
    public native int scan0(int pos);

    /**
     * Returns the position  of the first bit set to 1
     * at or after position pos. 
     *
     * <p> pos must be positive. 
     * <p> May return {@link #infinite}, if this is positive.
     */
    public native int scan1(int pos);


    /**
     * Sets the pos-th bit to 1 in this. 
     *
     * <p> pos must be positive. 
     */
    public native void setBit(int pos);

    /**
     * Sets the pos-th bit to 0 in this. 
     *
     * <p> pos must be positive. 
     */
    public native void clrBit(int pos);

    /** 
     * Inverts the pos-th bit in this. 
     *
     * <p> pos must be positive. 
     */
    public native void notBit(int pos);

    /** 
     * Returns true if the pos-th bit in this is 1. 
     *
     * <p> pos must be positive. 
     */
    public native boolean tstBit(int pos);


    // Miscellaneous functions
    //////////////////////////

    /** Whether this can fit in an int. */
    public native boolean fitsInt();

    /** Whether this is odd. */
    public native boolean isOdd();

    /** Whether this is even. */
    public native boolean isEven();

    /** Returns the number of digits necessary to represent |this| in base b. */
    public native int sizeInBase(int b);

    /** 
     * Returns the size (in GMP limbs) of the internal representation of 
     * this. 
     */
    public native int size();


    // Import and export
    /////////////////////
    
    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        /* format: 
           - length (int)
           - sign (byte)  
           - absolute value, in big-endian byte order
        */
        byte[] v = byteArrayValue();
        out.writeInt(v.length);
        out.writeByte(sgn());
        out.write(v); 
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        int len = in.readInt();
        byte sign = in.readByte();
        byte[] v = new byte[len];
        in.readFully(v);
        init();
        set(sign, v);
    }

    /** Fall-back deserialization function: sets to 0. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init_set(0); 
    }


    // Random number functions
    //////////////////////////


    /**
     * Stores into this a uniformly distributed random number with n bits 
     * (i.e., in 0..2^n-1). 
     *
     * <p> n must be positive.
     */
    public native void randomBits(RandState r, int n);

    /** Stores into this a uniformly distributed random number in 0..n-1. */
    public native void random(RandState r, Mpz n);

    /**
     * Stores into this a random number with n bits, and long strings of 
     * zeros and ones in its binary representation.
     *
     * <p> Useful to trigger corner-case bugs when testing.
     */
    public native void rRandomBits(RandState r, int n);

}
