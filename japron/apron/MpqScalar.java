/*
 * MpqScalar.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;
import java.math.BigInteger;
import gmp.*;

/**
 * Class of Scalar objects backed up by a {@link gmp.Mpq Mpq} multi-precision
 * rational.
 *
 * <p> Note: infinities are represented by using a null denominator.
 * This may cause problems if manipulated by {@link gmp.Mpq Mpq} functions.
 */
public class MpqScalar 
    extends Scalar 
    implements Cloneable, Serializable
{

    /** The (mutable) scalar value. */
    public Mpq val;


    // Constructors
    ///////////////

    /** Creates a new Mpq Scalar set to 0. */
    public MpqScalar()
    { 
        val = new Mpq();
    }

    /** Creates a copy of x. */
    public MpqScalar(MpqScalar x) 
    { 
        val = new Mpq(x.val); 
    }

    /** Creates a new Mpq Scalar holding a copy of x. */
    public MpqScalar(Mpq x)
    { 
        val = new Mpq(x); 
    }

    /** Creates a new Mpq Scalar holding a copy of x. */
    public MpqScalar(Mpz x) 
    { 
        val = new Mpq(x); 
    }

    /** Creates a new Mpq Scalar initialized to x. */
    public MpqScalar(int x) 
    { 
        val = new Mpq(x); 
    }

    /** Creates a new Mpq Scalar initialized to the fraction num/den. */
    public MpqScalar(int num, int den) 
    { 
        val = new Mpq(num, den); 
    }

    /** Creates a new Mpq Scalar initialized to the fraction num/den. */
    public MpqScalar(Mpz num, Mpz den) 
    { 
        val = new Mpq(num, den); 
    }

    /** Creates a new Mpq Scalar initialized to x. */
    public MpqScalar(BigInteger x) 
    { 
        val = new Mpq(x); 
    }

    /** Creates a new Mpq Scalar initialized to the fraction num/den. */
    public MpqScalar(BigInteger num, BigInteger den) 
    { 
        val = new Mpq(num, den); 
    }
    
    public MpqScalar copy()
    { 
        return new MpqScalar(this); 
    }


    // Set functions
    ////////////////

    /** Copies the value of x into this. */
    public void set(MpqScalar x) 
    {
        val.set(x.val);
    }

    /** Copies the value of x into this. */
    public void set(Mpq x) 
    {
        val.set(x);
    }

    /** Copies the value of x into this. */
    public void set(Mpz x) 
    {
        val.set(x);
    }

    /** Sets this to x. */
    public void set(int x) 
    {
        val.set(x);
    }

    /** Sets this to the fraction num/den. */
    public void set(int num, int den) 
    {
        val.set(num, den); 
        if (den!=0) val.canonicalize();
    }

    /** Sets this to x. */
    public void set(BigInteger x) 
    {
        val.set(x);
    }

    /** Sets this to the fraction num/den. */
    public void set(BigInteger num, BigInteger den) 
    {
        val.set(num, den);
        if (den.signum()!=0) val.canonicalize();
    }

   /**
    * Sets the scalar to +oo or -oo. 
    *
    * <p> The result is +oo if sign is strictly positive, -oo
    * if sign is strictly negative, 0 if sign is null.
    */
    public void setInfty(int sign) 
    {
        if (sign>0) val.set(1, 0);
        else if (sign<0) val.set(-1, 0);
        else val.set(0);
    }


    // Get functions
    ////////////////


    /** Returns the underlying Mpq. */
    public Mpq get()
    {
        return val;
    }
    
    /** 
     * Copies the value of this into r (exact).
     * 
     * @return 0.
     */
    public int toMpq(Mpq r, int round) 
    {
        int i = isInfty();
        if (i>0) r.set(1,0);
        else if (i<0) r.set(-1,0);
        else r.set(val);
        return 0;
    }

    /**
     * Copies the value of this into r (with rounding).
     *
     * @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    public int toMpfr(Mpfr r, int round) 
    {
        int i = isInfty();
        if (i!=0) { r.setInf(i); return 0; }
        return r.set(val, round);
    }

    /**
     * Copies the value of this into r[0] (with rounding).
     *
     *  @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    public int toDouble(double[] r, int round) 
    {
        int i = isInfty();
        if (i>0) { r[0] = Double.POSITIVE_INFINITY; return 0; }
        if (i<0) { r[0] = Double.NEGATIVE_INFINITY; return 0; }
        Mpfr x = new Mpfr(53);
        int e = x.set(val, round);
        r[0] = x.doubleValue(round);
        return e;
    }

    /** Returns a string representation of the value of this. */
    public String toString() 
    {
        int i = isInfty();
        if (i>0) return "+oo";
        if (i<0) return "-oo";
        return val.toString();
    }

    /** Returns a hash of the value of this. */
    public int hashCode() 
    {
        return val.hashCode();
    }


    // Tests
    ////////

    /** Returns -1, 0 or 1 if this is respectively -oo, finite, or +oo. */
    public int isInfty() 
    {
        if (val.getDen().sgn()==0) return val.sgn();
        return 0;
    }

    /** Whether this equals 0. */
    public boolean isZero() 
    {
        return val.sgn() == 0; 
    } 

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(MpqScalar x) 
    {
        int i1 = isInfty();
        int i2 = x.isInfty();
        if (i1>i2) return 1;
        if (i1<i2) return -1;
        if (i1!=0) return 0;
        return val.cmp(x.val);
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(Scalar x) 
    {
        int i1 = isInfty();
        int i2 = x.isInfty();
        if (i1>i2) return 1;
        if (i1<i2) return -1;
        if (i1!=0) return 0;
        if (x instanceof MpqScalar) return val.cmp(((MpqScalar)x).val);
        if (x instanceof MpfrScalar) return -((MpfrScalar)x).val.cmp(val);
        if (x instanceof DoubleScalar) {
            // converts x to Mpfr exactly, and compare to this
            Mpfr xx = new Mpfr(53);
            xx.set(((DoubleScalar) x).val, Mpfr.RNDN);
            return -xx.cmp(val);
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     *  strictly greater than x.
     */
    public int cmp(int x) 
    {
        int i = isInfty();
        if (i!=0) return i;
        return val.cmp(x,1);
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(MpqScalar x) 
    {
        int i1 = isInfty();
        int i2 = x.isInfty();
        if (i1!=i2) return false;
        if (i1!=0) return true;
        return val.isEqual(x.val);
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(Scalar x) 
    {
        int i1 = isInfty();
        int i2 = x.isInfty();
        if (i1!=i2) return false;
        if (i1!=0) return true;
        if (x instanceof MpqScalar) return val.isEqual(((MpqScalar)x).val);
        if (x instanceof MpfrScalar) return ((MpfrScalar)x).val.cmp(val)==0;
        if (x instanceof DoubleScalar) {
            // converts x to Mpfr exactly, and compare to this
            Mpfr xx = new Mpfr(53);
            xx.set(((DoubleScalar) x).val, Mpfr.RNDN);
            return xx.cmp(val)==0;
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(int x) 
    {
        if (isInfty()!=0) return false;
        return val.cmp(x,1) == 0;
    }


    /** Returns the sign of this. */
    public int sgn() 
    {
        return val.sgn();
    }

    // Operations
    /////////////

    /** Negates this (exact). */
    public void neg()  
    {
        val.neg();
    }

    /** Inverts this (exact). */
    public void inv() 
    {
        val.inv();
    }

    /** Returns a copy of this. */
    public MpqScalar clone()
    {
        return copy();
    }
}
