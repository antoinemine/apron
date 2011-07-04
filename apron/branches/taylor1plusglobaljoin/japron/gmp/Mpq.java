/*
 * Mpq.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.math.BigInteger;
import java.io.*;

/**
 * GMP arbitrary precision rationals.
 *
 * <p> This class wraps {@code mpq_t} GMP objects.
 */
public class Mpq
    implements Serializable, Comparable<Mpq>, Cloneable
{
    
    // Internals
    ////////////

    /**
     * Actually, a pointer to a mpq_t object allocated in the C heap. 
     */
    private long ptr;

    private native void init();

    /** Deallocates the underlying mpq_t GMP object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("jgmp"); class_init(); }


    // Canonicalization
    ///////////////////

    /**
     * Puts the fraction into normal form.
     *
     * <p> Removes common factors in the numerator and denominator, 
     * and makes the denominator positive.
     *
     * <p> All arithmetic functions assume and return canonical fractions.
     * However, pure assignment functions (including separate assignments
     * of denominators and numerators, directly or through a
     * {@link gmp.MpzRef} reference) do not canonicalize the assigned
     * object, and so, the function must be called by the user before 
     * arithmetic operations can be performed.
     */
    public native void canonicalize();


    // Constructors
    ///////////////

    /** Constructs a rational initialized to 0/1. */
    public Mpq() 
    { 
        init(); 
    }

    /** Constructs a copy of op. */
    public Mpq(Mpq op) 
    {
        init();
        set(op); 
    }

    /** Constructs a rational equal to the integer op. */
    public Mpq(Mpz op) 
    {
        init(); 
        set(op); 
    }

    /** Constructs a rational equal to the integer op. */
    public Mpq(BigInteger op) 
    {
        init(); 
        set(new Mpz(op)); 
    }

    /** Constructs a rational equal to the integer op. */
    public Mpq(int op) 
    {
        init(); 
        set(op); 
    }

    /**
     * Constructs a rational equal to the double d. 
     *
     * <p> There is no rounding, the conversion is exact.
    */
    public Mpq(double d) 
    { 
        init(); 
        set(d); 
    }

    /** 
     * Constructs a rational from the String s expressed in base b. 
     *
     * <p> The String can have a fraction / symbol.
     * <p> The result is canonicalized. 
     * @see gmp.Mpz#Mpz(String,int)
     */
    public Mpq(String s, int b) 
    {
        init(); 
        set(s, b); 
        if (denRef().sgn()!=0) canonicalize(); 
    }

    /** 
     * Constructs a rational from the String s.
     *
     * <p> The String can have a fraction / symbol.
     * <p> The result is canonicalized. 
     * @see gmp.Mpz#Mpz(String)
     */
    public Mpq(String s) 
    {
        init(); 
        set(s); 
        if (denRef().sgn()!=0) canonicalize(); 
    }

    /**
     * Constructs a rational equal to num/den. 
     *
     * <p> The result is canonicalized.
     */
    public Mpq(Mpz num, Mpz den) 
     {
        init(); 
        set(num, den); 
        if (den.sgn()!=0) canonicalize(); 
    }

     /** 
      * Constructs a rational equal to num/den. 
      *
      * <p> The result is canonicalized. 
      */
    public Mpq(BigInteger num, BigInteger den)
    {
        init(); 
        set(new Mpz(num), new Mpz(den)); 
        if (den.signum()!=0) canonicalize(); 
    }

    /**
     * Constructs a rational equal to num/den. 
     *
     * <p> The result is canonicalized. 
     */
    public Mpq(int num, int den)  
    {
        init();
        set(num, den); 
        if (den!=0) canonicalize(); 
    }

    /**
     * Creates a new Mpq initialized with the value of this.
     */
    public Mpq clone()
        throws CloneNotSupportedException
    {
        return new Mpq(this);
    }


    // Set functions
    ////////////////

    /** Copies the argument into this. */
    public native void set(Mpq op);

    /** Sets this to the integer op. */
    public native void set(Mpz op);

    /** Sets this to the integer op. */
    public void set(BigInteger op)
    {
        set(new Mpz(op)); 
    }

    /** Sets this to the integer op. */
    public void set(int op)
    { 
        set(op,1); 
    }

 
   /** 
     * Sets this to num/den.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void set(Mpz num, Mpz den);

    /** 
     * Sets this to num/den.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public void set(BigInteger num, BigInteger den)
    { 
        set(new Mpz(num), new Mpz(den)); 
    }

    /** 
     * Sets this to num/den.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void set(int num, int den);

    /** 
     * Sets this to the double number.
     * 
     * <p> There is no rounding: the conversion is exact. 
     */
    public native void set(double d);

    /** 
     * Sets this to the String s expressed in base b. 
     *
     * <p> The String can have a fraction / symbol.
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     * @see gmp.Mpz#Mpz(String,int)
     */
    public native void set(String s, int b);

    /** 
     * Sets this to the String s.
     *
     * <p> The String can have a fraction / symbol.
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     * @see gmp.Mpz#Mpz(String)
     */
    public void set(String s) 
    {
        set(s, 0); 
    }


    /** 
     * Changes the numerator of this.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void setNum(Mpz op);

    /** 
     * Changes the numerator of this.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void setNum(int op);

    /** 
     * Changes the numerator of this.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public void setNum(BigInteger op)
    { 
        setNum(new Mpz(op)); 
    }

    /** 
     * Changes the denominator of this.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void setDen(Mpz op);

    /** 
     * Changes the denominator of this.
     *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
     */
    public native void setDen(int op);

    /** 
     * Changes the denominator of this.
      *
     * <p> {@link #canonicalize} must be called manually if the fraction 
     * is not canonical.
    */
    public void setDen(BigInteger op)
    { 
        setDen(new Mpz(op)); 
    }



    // Get functions
    ////////////////

    /** 
     * Returns this as a double. 
     * 
     * <p> If this cannot be represented as a double, the result is truncated.
     * <p> Overflows cause infinities to be returned.
     */
    public native double doubleValue();

    /** 
     * Returns a String representation of this in base b. 
     *
     * @param b must be between 2 and 62.
     */
    public native String toString(int b);

    /** Returns a String representation of this in base 10. */
    public String toString() 
    {
        return toString(10); 
    }

    /** Returns a copy of the numerator of this. */
    public native Mpz getNum();

    /** Returns a copy of the denominator of this. */
    public native Mpz getDen();

    /** Returns a reference to the numerator of this. */
    public native MpzRef numRef();

    /** Returns a reference to the denominator of this. */
    public native MpzRef denRef();


    // Arithmetic functions
    ///////////////////////

    /** Stores arg1+arg2 into this. */
    public native void add(Mpq arg1, Mpq arg2);

    /** Adds arg to this. */
    public void add(Mpq arg) 
    {
        add(this, arg); 
    }


    /** Stores arg1-arg2 into this. */
    public native void sub(Mpq arg1, Mpq arg2);

    /** Subtracts arg from this. */
    public void sub(Mpq arg) 
    {
        sub(this, arg); 
    }

    /** Replaces this with arg-this. */
    public void subRev(Mpq arg) 
    {
        sub(arg, this); 
    }


    /** Stores arg1*arg2 into this. */
    public native void mul(Mpq arg1, Mpq arg2);

    /** Multiply this by arg. */
    public void mul(Mpq arg) 
    {
        mul(this,arg); 
    }

    /** Stores arg1*(2^arg2) into this. */
    public native void mul2exp(Mpq arg1, int arg2);

    /** Multiply this by 2^arg. */
    public void mul2exp(int arg)
    {
        mul2exp(this,arg); 
    }


    /** Stores arg1/arg2 into this. */
    public native void div(Mpq arg1, Mpq arg2);

    /** Divides this by arg. */
    public void div(Mpq arg) 
    {
        div(this, arg); 
    }

    /** Stores arg/this into this. */
    public void divRev(Mpq arg) 
    {
        div(arg, this); 
    }

    /** Stores arg1/(2^arg2) into this. */
    public native void div2exp(Mpq arg1, int arg2);

    /** Divides this by (2^arg). */
    public void div2exp(int arg) 
    {
        div2exp(this,arg); 
    }


    /** Stores -arg into this. */
    public native void neg(Mpq arg);

    /** Replaces this with -this. */
    public void neg() 
    {
        neg(this); 
    }


    /** Stores |arg| into this. */
    public native void abs(Mpq arg);

    /** Replaces this with |this|. */
    public void abs() 
    {
        abs(this); 
    }


    /** Stores 1/arg into this. */
    public native void inv(Mpq arg);

    /** Replaces this with 1/this. */
    public void inv() 
    {
        inv(this); 
    }


    // Miscellaneous functions
    //////////////////////////

    /**
     * Returns the size (in GMP limbs) of the internal representation of 
     * this. 
     */
    public native int size();


    // Comparison functions
    ///////////////////////
    
    /**
     * Returns a positive, null, or negative number when this is respectively
     * strictly greater, equal, or strictly smaller than op.
     */
    public native int cmp(Mpq op);

    /**
     * Returns a positive, null, or negative number when this is respectively
     * strictly greater, equal, or strictly smaller than num/den.
     *
     * <p> den must be positive.
     */
    public native int cmp(int num, int den);


    /**
     * Returns +1, 0, or -1 when this is respectively strictly positive, null,
     * or strictly negative.
     */
    public native int sgn();

    /**
     * Whether this equals op. 
     *
     * <p> This method is faster than calling cmp, as fractions are
     * assumed to be canonical.
     */
    public native boolean isEqual(Mpq op);

    /** Whether x is a Mpq and has the same value as this. */
    public boolean equals(Object x)
    { 
        return (x instanceof Mpq) && isEqual((Mpq)x); 
    }


    /** Compare the value of this and x. */
    public int compareTo(Mpq x) 
    {
        return cmp(x); 
    }


    /** Returns a hash of the value of this. */
    public native int hashCode();


    /** Whether this and x have the same address in memory. */
    public boolean isPhysicalEqual(Mpq x)
    {
        return ptr == x.ptr; 
    }


    // Import and export
    /////////////////////
    
    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        /* format:
           - Mpz numerator
           - Mpz denominator
         */
        out.writeObject(getNum());
        out.writeObject(getDen());
    }
    
    /** Reads back a serialized Mpq. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        Mpz num = (Mpz)(in.readObject());
        Mpz den = (Mpz)(in.readObject());
        init();
        set(num, den);
    }

    /** Fall-back deserialization function: sets to 0. */
    private void readObjectNoData() 
        throws ObjectStreamException
    { 
        init(); 
    }

}
