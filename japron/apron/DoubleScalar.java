/*
 * DoubleScalar.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;
import gmp.*;

/**
 * Class of Scalar objects backed up by a double-precision
 * floating-point number.
 */
public class DoubleScalar 
    extends Scalar 
    implements Cloneable, Serializable
{

    /** The (mutable) scalar value. */
    public double val;


    // Constructors
    ///////////////

    /** Creates a new double Scalar set to 0. */
    public DoubleScalar() 
    {
        val = 0.;
    }

    /** Creates a copy of x. */
    public DoubleScalar(DoubleScalar x) 
    { 
        val = x.val;
    }

    /** Creates a new double Scalar initialized to x. */
    public DoubleScalar(double x) 
    { 
        val = x; 
    }

    public DoubleScalar copy() 
    {
        return new DoubleScalar(this); 
    }
    

    // Set functions
    ////////////////

    /** Copies the value of x into this. */
    public void set(DoubleScalar x)
    {
        val = x.val;
    }

    /** Copies the value of x into this. */
    public void set(double x) 
    {
        val = x;
    }

    /** Sets this to x. */
    public void set(int x) 
    {
        val = x;
    }

   /**
    * Sets the scalar to +oo or -oo. 
    *
    * <p> The result is +oo if sign is strictly positive, -oo
    * if sign is strictly negative, 0 if sign is null.
    */
    public void setInfty(int sign) 
    {
        if (sign>0) val = Double.POSITIVE_INFINITY;
        else if (sign<0) val = Double.NEGATIVE_INFINITY;
        else val = 0;
    }


    // Get functions
    ////////////////
 

    /** Returns the underlying double. */
    public double get()
    {
        return val;
    }

   /**
     * Copies the value of this into r (exact, except if NaN).
     *
     * @return 0 (exact) or 1 (NaN).
     */
    public int toMpq(Mpq r, int round)
    {
        if (Double.isNaN(val)) { r.set(0); return 1; }
        if (val==Double.POSITIVE_INFINITY) r.set(1,0);
        else if (val==Double.NEGATIVE_INFINITY) r.set(-1,0);
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
        return r.set(val,round);
    }

    /**
     * Copies the value of this into r (exact).
     *
     * @return 0.
     */
    public int toDouble(double[] r, int round) 
    {
        r[0] = val;
        return 0;
    }

    /** Returns a string representation of the value of this. */
    public String toString()
    {
        return Double.toString(val);
    }

    /** Returns a hash of the value of this. */
    public int hashCode() 
    {
        return (new Double(val)).hashCode();
    }


    // Tests
    ////////

    /** Returns -1, 0 or 1 if this is respectively -oo, finite, or +oo. */
    public int isInfty() 
    {
        if (Double.isInfinite(val)) return val > 0 ? 1 : -1;
        return 0;
    }

    /** Whether this equals 0. */
    public boolean isZero() 
    {
        return val == 0.;
    } 

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(DoubleScalar x) 
    {
        return (val==x.val) ? 0 : (val < x.val) ? -1 : 1;
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(int x)
    {
        return (val==x) ? 0 : (val < x) ? -1 : 1;
    }

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    public int cmp(Scalar x) 
    {
        if (x instanceof DoubleScalar) {
            double y = ((DoubleScalar)x).val;
            return (val==y) ? 0 : (val < y) ? -1 : 1;
        }
        if (x instanceof MpfrScalar) {
            Mpfr xx = ((MpfrScalar) x).val;
            return -xx.cmp(val);
        }
        if (x instanceof MpqScalar) {
            int i1 = isInfty();
            int i2 = x.isInfty();
            if (i1>i2) return 1;
            if (i1<i2) return -1;
            if (i1!=0) return 0;
            Mpq v = new Mpq(val);
            return v.cmp(((MpqScalar)x).val);
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(DoubleScalar x) 
    {
        return val == x.val;
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(int x) 
    {
        return val == x;
    }

    /** Whether this and x have the same value. */
    public boolean isEqual(Scalar x) 
    {
        if (x instanceof DoubleScalar) return val == ((DoubleScalar)x).val;
        if (x instanceof MpfrScalar) {
            Mpfr xx = ((MpfrScalar) x).val;
            return xx.cmp(val) == 0;
        }
        if (x instanceof MpqScalar) {
            int i1 = isInfty();
            int i2 = x.isInfty();
            if (i1!=i2) return false;
            if (i1!=0) return true;
            Mpq v = new Mpq(val);
            return v.isEqual(((MpqScalar)x).val);
        }
        throw new IllegalArgumentException("unknown Scalar subtype");
    }

    /** Returns the sign of this. */
    public int sgn() 
    {
        return val > 0 ? 1 : val < 0 ? -1 : 0;
    }


    // Operations
    //////////////

    /** Negates this (exact). */
    public void neg()  
    {
        val = -val;
    }

    /** Inverts this (with rounding). */
    public void inv() 
    {
        val = 1./val;
    }

    /** Returns a copy of this. */
    public DoubleScalar clone()
    {
        return copy();
    }
}
