/*
 * Coeff.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Abstract class of constant coefficients that can appear in expressions.
 *
 * <p> A coefficient can be either a {@link apron.Scalar} real singleton or
 * an {@link apron.Interval} set of real values.
 * The Coeff class provides methods to manipulate coefficients independently
 * from the underlying representation.
 */
abstract public class Coeff 
    implements Cloneable, Serializable
{

    // Constructors
    ///////////////

    /** Creates a 0 coefficient as a double Scalar. */
    public static Coeff create()
    { return Scalar.create(); }

    /** Creates a copy of the coefficient, of the same kind. */
    public abstract Coeff copy();


    // Get functions
    ////////////////

    /** Returns a reference to the lower bound of this. */
    abstract public Scalar inf();

    /** Returns a reference to the upper bound of this. */
    abstract public Scalar sup();

    /** Returns a string representation of this. */
    abstract public String toString();

    /** Returns a hash of the value of this. */
    abstract public int hashCode();


    // Tests
    ////////

    /** Whether this represents the singleton 0. */
    abstract public boolean isZero();

    /** Whether this represents a singleton. */
    abstract public boolean isScalar();

    /** Whether this represents the singleton i. */
    abstract public boolean isEqual(int i);
   
    /** Whether this equals x. */
    abstract public boolean isEqual(Coeff x);
   
    /**
     * Compare this and x for inclusion. 
     *
     * @see apron.Interval#cmp(Interval)
     */
    abstract public int cmp(Coeff x);
   

    // Operations
    //////////////

    /** Negates this. */
    abstract public void neg();

    /** Whether x is a Coeff has the same value. */
    public boolean equals(Object x)
    {
        return (x instanceof Coeff) && (isEqual((Coeff)x));
    }
}
