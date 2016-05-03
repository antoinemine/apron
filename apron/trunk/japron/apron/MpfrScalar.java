/*
 * MpfrScalar.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;
import gmp.*;

/**
 * Class of Scalar objects backed up by a {@link gmp.Mpfr Mpfr} 
 * multi-precision floating-point number.
 */
public class MpfrScalar 
    extends Scalar 
    implements Cloneable, Serializable
{

    /** The (mutable) scalar value. */
    public Mpfr val;


    // Constructors
    ///////////////

    /** Creates a new Mpfr Scalar set to NaN. */
    public MpfrScalar()
    {
        val = new Mpfr();
    }

    /** Creates a new Mpfr Scalar with the given precision, set to NaN. */
    public MpfrScalar(int prec) 
    {
        val = new Mpfr(prec);
    }

    /** Creates a copy of x. */
    public MpfrScalar(MpfrScalar x) 
    { 
        val = new Mpfr(x.val); 
    }

    /** Creates a new Mpfr Scalar holding a copy of x. */
    public MpfrScalar(Mpfr x) 
    { 
        val = new Mpfr(x); 
    }

    /**
     * Creates a new Mpfr Scalar holding a copy of x. 
     *
     * <p> x is rounded to the default Mpfr precision
     * (see {@link gmp.Mpfr#setDefaultPrec gmp.Mpfr.setDefaultPrec})
     * in the direction specified by round.
     */
    public MpfrScalar(double x, int round)
    { 
        val = new Mpfr(x, round); 
    }

    public MpfrScalar copy() 
    { 
        return new MpfrScalar(this); 
    }


    // Set functions
    ////////////////

    /**
     * Copies the value of x into this. 
     *
     * <p> The precision of this is changed to match that of x.
     */
    public void set(MpfrScalar x) 
    {
        val.set(x.val);
    }

    /** Copies the value of x into this.
     *
     * <p> The precision of this is changed to match that of x.
     */
    public void set(Mpfr x) 
    {
        val.set(x);
    }
 
    /**
     * Copies the value of x into this. 
     *
     * <p>this retains its precision and x is rounded. 
     */
    public void set(Mpfr x, int round) 
    {
        val.set(x, round);
    }
 
    /**
     * Copies the value of x into this.
     *
     * <p>this retains its precision and x is rounded. 
     */
    public void set(double x, int round) 
    {
        val.set(x, round);
    }
 
    /** Sets this to x. */
    public void set(int x) 
    {
        val.set(x, Mpfr.RNDN);
    }

   /**
    * Sets the scalar to +oo or -oo. 
    *
    * <p> The result is +oo if sign is strictly positive, -oo
    * if sign is strictly negative, 0 if sign is null.
    */
    public void setInfty(int sign) 
    {
        val.setInf(sign);
    }


    // Get functions
    ////////////////

    /** Returns the underlying Mpfr. */
    public Mpfr get()
    {
        return val;
    }
    
    /**
     * Copies the value of this into r (exact, except if NaN)
     *
     * @return 0 (exact) or 1 (NaN).
     */
    public int toMpq(Mpq r, int round) 
    {
        if (val.isNaN()) { r.set(0); return 1; }
        if (val.isInf()) {
            if (val.sgn()>0) r.set(1,0);
            else r.set(-1,0);
            return 0;
        }
        Mpz x = new Mpz();
        int e = val.MpzExponentValue(x);
        r.set(x);
        r.mul2exp(e);
        return 0;
    }

    /**
     * Copies the value of this into r (exact).
     *
     * @return 0.
     */
    public int toMpfr(Mpfr r, int round) 
    {
        r.setPrec(val.getPrec());
        r.set(val);
        return 0;
    }

    /**
     * Copies the value of this into r[0] (with rounding).
     *
     * @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    public int toDouble(double[] r, int round) 
    {
        r[0] = val.doubleValue(round);
        return val.cmp(r[0]);
    }

    /** Returns a string representation of the value of this. */
    public String toString() 
    {
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
        if (val.isInf()) return val.sgn();
        return 0;
    }

    /** Whether this equals 0. */
    public boolean isZero() 
    {
        return val.cmp(0) == 0; 
    }
    

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(MpfrScalar x) 
    {
        return val.cmp(x.val);
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(int x) 
    {
        return val.cmp(x);
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(Scalar x) 
    {
        if (x instanceof MpfrScalar) return val.cmp(((MpfrScalar)x).val);
        if (x instanceof DoubleScalar) return val.cmp(((DoubleScalar)x).val);
        if (x instanceof MpqScalar) {
            int i1 = isInfty();
            int i2 = x.isInfty();
            if (i1>i2) return 1;
            if (i1<i2) return -1;
            if (i1!=0) return 0;
            return val.cmp(((MpqScalar)x).val);
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(MpfrScalar x) 
    {
        return val.isEqual(x.val);
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(int x) 
    {
        return val.cmp(x) == 0;
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(Scalar x) 
    {
        if (x instanceof MpfrScalar) return val.isEqual(((MpfrScalar)x).val);
        if (x instanceof DoubleScalar) return val.cmp(((DoubleScalar)x).val)==0;
        if (x instanceof MpqScalar) {
            int i1 = isInfty();
            int i2 = x.isInfty();
            if (i1!=i2) return false;
            if (i1!=0) return true;
            return val.cmp(((MpqScalar)x).val)==0;
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /** Returns the sign of this. */
    public int sgn() 
    {
        return val.sgn();
    }


    // Operations
    //////////////

    /** Negates this (exact). */
    public void neg() 
    {
        val.neg(Mpfr.RNDN);
    }

    /** Inverts this (with rounding). */
    public void inv() 
    {
        val.div(1, val, Mpfr.RNDN);
    }

    /** Returns a copy of this. */
    public MpfrScalar clone()
    {
        return copy();
    }
}
