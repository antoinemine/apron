/*
 * Linterm0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of terms in level 0 linear expressions and constraints.
 *
 * <p> A Linterm0 is a pair composed of a {@link apron.Coeff} coefficient
 * and a variable denoted by dimension (i.e., integer starting 
 * from 0).
 * It represents a single term in a {@link apron.Linexpr0} or 
 * {@link apron.Lincons0}.
 */
public class Linterm0
    implements Cloneable, Serializable
{
    /** Dimension index (positive). */
    public int dim;

    /** Variable coefficient. */
    public Coeff coeff;

    /** 
     * Creates a new linear term.
     *
     * <p>The coefficient is referenced, not copied.
     */
    public Linterm0(int d, Coeff c)
    { 
        if (d<0) throw new IllegalArgumentException("negative dimension");
        dim = d; 
        coeff = c; 
    }

    /**
     * Creates a copy of the linear term.
     *
     * <p>The coefficient is copied.
     */
    public Linterm0(Linterm0 t)
    {
        dim = t.dim;
        coeff = t.coeff.copy();
    }

    /**
     * Returns a String representation of the linear term.
     * 
     * <p> Dimension i is denoted by names[i].
     */
    public String toString(Var[] names)
    {
        if (names==null) return coeff.toString() + "x" + dim;
        else return coeff.toString() + names[dim].toString();
    }

    public String toString(String[] names)
    {
        if (names==null) return coeff.toString() + "x" + dim;
        else return coeff.toString() + names[dim];
    }

    /**
     * Returns a String representation of the linear term.
     *
     * <p> Dimension i is denoted as xi.
     */    
    public String toString()
    {
        return toString((Var[])null);
    }

    /** Returns a copy of this. */
    public Linterm0 clone()
    {
        return new Linterm0(dim, coeff);
    }

     /** Whether x and this are structurally equal. */
    public boolean isEqual(Linterm0 x)
    {
        return dim==x.dim && coeff.isEqual(x.coeff);
    }

    /** Whether x is a Linterm0 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Linterm0) && (isEqual((Linterm0)x));
    }

    public int getDimension()
    {
        return dim;
    }

    public Coeff getCoefficient()
    {
        return coeff;
    }

    public void setDimension(int d)
    {
        if (d<0) throw new IllegalArgumentException("negative dimension");
        dim = d;
    }

    public void setCoeffcient(Coeff c)
    {
        coeff = c;
    }
}
