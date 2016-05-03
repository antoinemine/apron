/*
 * Interval.java
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
 * Class of intervals of reals, denoted by a lower and an upper 
 * {@link apron.Scalar} bound.
 *
 * <p> Intervals are a specialisation of the more general {@link apron.Coeff}
 * class.
 *
 * <p> Intervals may be unbounded (one or both bound(s) can be infinite).
 * An interval can also be empty (the canonical empty interval is [1, -1]).
 */
public class Interval 
    extends Coeff 
    implements Cloneable, Serializable
{

    /** (Mutable) lower bound. */
    public Scalar inf;

    /** (Mutable) upper bound. */
    public Scalar sup;


    // Constructors
    ///////////////

    /** Creates a new interval set to [0, 0], with double bounds. */
    public Interval()
    {
        inf = new DoubleScalar(0); 
        sup = new DoubleScalar(0);
    }

    /** 
     * Creates a new interval [i, s].
     *
     * <p> The bounds are not copied, but only referenced.
     */
    public Interval(Scalar i, Scalar s)
    { 
        inf = i; 
        sup = s; 
    }

    /** 
     * Creates a copy of the interval i. 
     *
     * <p> Both scalar bounds are copied.
     */
    public Interval(Interval i)
    { 
        inf = i.inf.copy(); 
        sup = i.sup.copy();
    }

    /** Creates an interval [i, s] with Mpq bounds. */
    public Interval(int i, int s)
    { 
        inf = new MpqScalar(i); 
        sup = new MpqScalar(s); 
    }

    /** Creates an interval [inum/iden, snum/sden] with Mpq bounds. */
    public Interval(int inum, int iden, int snum, int sden)
    {
        inf = new MpqScalar(inum, iden);
        sup = new MpqScalar(snum, sden); 
    }

    /** Creates an interval [i, s] with Mpq bounds. */
    public Interval(BigInteger i, BigInteger s)
    { 
        inf = new MpqScalar(i); 
        sup = new MpqScalar(s); 
    }

    /** Creates an interval [inum/iden, snum/sden] with Mpq bounds. */
    public Interval(BigInteger inum, BigInteger iden, BigInteger snum, BigInteger sden)
    { 
        inf = new MpqScalar(inum, iden); 
        sup = new MpqScalar(snum, sden); 
    }

    /** Creates an interval [i, s] with Mpq bounds. */
    public Interval(Mpz i, Mpz s)
    { 
        inf = new MpqScalar(i); 
        sup = new MpqScalar(s); 
    }

    /** Creates an interval [inum/iden, snum/sden] with Mpq bounds. */
    public Interval(Mpz inum, Mpz iden, Mpz snum, Mpz sden)
    { 
        inf = new MpqScalar(inum, iden); 
        sup = new MpqScalar(snum, sden); 
    }

    /** Creates an interval [i, s] with Mpq bounds (copied). */
    public Interval(Mpq i, Mpq s)
    { 
        inf = new MpqScalar(i); 
        sup = new MpqScalar(s);
    }

    /** Creates an interval [i, s] with double bounds. */
    public Interval(double i, double s)
    { 
        inf = new DoubleScalar(i);
        sup = new DoubleScalar(s); 
    }

    /** Creates an interval [i, s] with Mpfr bounds (copied). */
    public Interval(Mpfr i, Mpfr s)
    { 
        inf = new MpfrScalar(i); 
        sup = new MpfrScalar(s); 
    }

    public Interval copy()
    { 
        return new Interval(this); 
    }
   

    // Set functions
    ////////////////

    /** Sets the lower bound. */
    public void setInf(Scalar s)
    {
        inf = s;
    }

    /** Sets the upper bound. */
    public void setSup(Scalar s)
    {
        sup = s;
    }

    /** Sets the interval to [-oo, +oo], without changing the bound types. */
    public void setTop()
    { 
        inf.setInfty(-1);
        sup.setInfty(1);
    }

    /** Sets the interval to [1, -1], without changing the bound types. */
    public void setBottom()
    { 
        inf.set(1);
        sup.set(-1);
    }


    // Tests
    ////////

    /** Whether this represents [-oo, +oo]. */
    public boolean isTop() 
    { 
        return (inf.isInfty() < 0) && (sup.isInfty() > 0); 
    }

    /** Whether this represents an empty interval. */
    public boolean isBottom() 
    { 
        return inf.cmp(sup) > 0; 
    }

    /** Whether this represents the singleton [0, 0]. */
    public boolean isZero()
    { 
        return inf.isEqual(0) && sup.isEqual(0); 
    }

    /** Whether this is included in x. */
    public boolean isLeq(Interval x)
    {
        int sinf = inf.cmp(x.inf);
        int ssup = sup.cmp(x.sup);
        return (sinf>=0 && ssup<=0) || isBottom();
    }

    /** 
     * Compares this and x for inclusion.
     *
     * @return 0 is this = x, 
     * -1 if this is included in x,
     * +1 if this contains x,
     * -2 if the lower bound of this is smaller than that of x,
     * +2 if the lower bound of this is greater than that of x.
     */
    public int cmp(Interval x) 
    {
        int sinf = inf.cmp(x.inf);
        int ssup = sup.cmp(x.sup);
        if (sinf==0 && ssup==0) return 0;
        if (sinf>=0 && ssup<=0) return -1;
        if (sinf<=0 && ssup>=0) return 1;
        boolean b1 = isBottom();
        boolean b2 = x.isBottom();
        if (b1 && b2) return 0;
        if (b1) return -1;
        if (b2) return 1;
        if (sinf > 0) return 2;
        return -2;
    }

    /** Whether this and x are equal. */
    public boolean isEqual(Interval x)
    {
        return 
            (inf.isEqual(x.inf) && sup.isEqual(x.sup)) ||
            (isBottom() && x.isBottom());
    }

    /** Whether this equals the singleton i. */
    public boolean isEqual(int i)
    {
        return inf.isEqual(i) && sup.isEqual(i); 
    }

    /** Whether this represents a singleton. */
    public boolean isScalar()
    { 
        return inf.isEqual(sup); 
    }

    /** Whether this equals x. */
    public boolean isEqual(Coeff x) 
    {
        if (x instanceof Interval) return isEqual((Interval)x);
        if (x instanceof Scalar) {
            Scalar xx = (Scalar)x;
            return inf.isEqual(xx) && sup.isEqual(xx);
        }
        throw new IllegalArgumentException("unknown Coeff subtype");
    }
   
    /**
     * Compares this and x for inclusion.
     *
     * @see #cmp(Interval)
     */
    public int cmp(Coeff x) 
    {
        if (x instanceof Interval) return cmp((Interval)x);
        if (x instanceof Scalar) {
            Scalar xx = (Scalar)x;
            return cmp(new Interval(xx, xx));
        }
        throw new IllegalArgumentException("unknown Coeff subtype");
    }


    // Operations
    /////////////

    /** Negates x. */
    public void neg() 
    {
        Scalar x = inf; inf = sup; sup = x;
        inf.neg();
        sup.neg();
    }


    // Get functions
    ////////////////

    /** Returns a reference to the lower bound of this. */
    public Scalar inf()
    {
        return inf; 
    }

    /** Returns a reference to the upper bound of this. */
    public Scalar sup()
    {
        return sup; 
    }


    /** Returns a string representation of the interval. */
    public String toString()
    { 
        return "[" + inf + "," + sup + "]"; 
    }

    /** Returns a hash made from both bounds. */
    public int hashCode()
    {
        return inf.hashCode() ^ sup.hashCode(); 
    }

    /** Returns a copy of this. */
    public Interval clone()
    {
        return copy();
    }
}
