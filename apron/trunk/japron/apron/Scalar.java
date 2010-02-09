/*
 * Scalar.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;
import gmp.*;

/**
 * Abstract class of coefficients that represent scalar real numbers.
 *
 * <p> A Scalar object may be an arbitrary-precision fraction 
 * (stored into a {@link apron.MpqScalar}), a double (stored in a 
 * {@link apron.DoubleScalar}), or an arbitrary-precision float 
 * (stored in a {@link apron.MpfrScalar}).
 * The Scalar class provides methods to manipulate them independently
 * from the underlying representation.
 * It is also a specialisation of the more general {@link apron.Coeff}
 * class to coefficients that are singleton.
 *
 * <p> All kinds of Scalar can also represent +oo and -oo.
 */
abstract public class Scalar 
    extends Coeff 
    implements Cloneable, Serializable
{

    // Constructors
    ///////////////

    /** Creates a double Scalar initialized to 0. */
    public static Scalar create() 
    {
        return new DoubleScalar(); 
    }

    /** Creates a copy of the Scalar, of the same kind. */
    public abstract Scalar copy();


    // Set functions
    ////////////////

   /**
    * Sets the scalar to +oo or -oo. 
    *
    * <p> The result is +oo if sign is strictly positive, -oo
    * if sign is strictly negative, 0 if sign is null.
    */
    abstract public void setInfty(int sign);

    /** Sets the scalar to an integer, without changing its kind. */
    abstract public void set(int i);


    // Get functions
    ////////////////

    /**
     * Copies the value of this into r.
     *
     * @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    abstract public int toMpq(Mpq r, int round);

    /**
     * Copies the value of this into r.
     *
     * @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    abstract public int toMpfr(Mpfr r, int round);

    /**
     * Copies the value of this into r[0].
     *
     * @return -1, 0, 1 if the result is lower, exact, or greater than this.
     */
    abstract public int toDouble(double[] r, int round);

    /** Returns a string representation of the Scalar value. */
    abstract public String toString();

    /** Returns a hash of the value of this. */
    abstract public int hashCode();

    /** Returns a reference to the lower bound of this, i.e., this itself. */
    public Scalar inf()
    { 
        return this; 
    }

    /** Returns a reference to the upper bound of this, i.e., this itself. */
    public Scalar sup()
    {
        return this; 
    }


    // Tests
    ////////

    /** Returns -1, 0 or 1 if this is respectively -oo, finite, or +oo. */
    abstract public int isInfty();

    /** Whether this equals 0. */
    abstract public boolean isZero();

    /** Returns the sign of this. */
    abstract public int sgn();

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    abstract public int cmp(Scalar x);

    /**
     * Returns -1, 0, 1 if this is respectively strictly smaller, equal, or
     * strictly greater than x.
     */
    abstract public int cmp(int x);

    /** Whether this and x have the same value. */
    abstract public boolean isEqual(Scalar x);

    /** Whether this and x have the same value. */
    abstract public boolean isEqual(int x);

    /**
     * Whether this represents a singleton. 
     *
     * <p> Always true. 
     */
    public boolean isScalar()
    {
        return true; 
    }

    /** Whether this equals x. */
    public boolean isEqual(Coeff x) 
    {
        if (x instanceof Scalar) return isEqual((Scalar)x);
        if (x instanceof Interval) {
            Interval xx = (Interval)x;
            return xx.inf.isEqual(this) && xx.sup.isEqual(this);
        }
        throw new IllegalArgumentException("unknown Coeff subtype");
    }
   
    /** Compare this and x for inclusion. */
    public int cmp(Coeff x) 
    {
        if (x instanceof Scalar) return cmp((Scalar)x);
        if (x instanceof Interval)
            return ((Interval)x).cmp(new Interval(this, this));
        throw new IllegalArgumentException("unknown Coeff subtype");
    }


    // Operations
    //////////////

    /** Negates this. */
    abstract public void neg();

    /** Inverts this. */
    abstract public void inv();

}
