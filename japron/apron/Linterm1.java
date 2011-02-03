/*
 * Linterm1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of terms in level 1 linear expressions and constraints.
 *
 * <p> A Linterm1 is a pair composed of a {@link apron.Coeff} coefficient
 * and a variable denoted by name.
 * It represents a single term in a {@link apron.Linexpr1} or 
 * {@link apron.Lincons1}.
 */
public class Linterm1
    implements Cloneable, Serializable
{
    /** Variable name. */
    public String var;

    /** Variable coefficient. */
    public Coeff coeff;

    /** 
     * Creates a new linear term.
     *
     * <p> The coefficient is referenced, not copied.
     */
    public Linterm1(String v, Coeff c)
    { 
        var = v;
        coeff = c; 
    }

    /**
     * Creates a copy of the linear term.
     *
     * <p> The coefficient is copied.
     */
    public Linterm1(Linterm1 t)
    {
        var = t.var;
        coeff = t.coeff.copy();
    }

    /**
     * Returns a String representation of the linear term.
     */    
    public String toString()
    {
        return coeff.toString() + var;
    }

    /** Returns a copy of this. */
    public Linterm1 clone()
        throws CloneNotSupportedException
    {
        return new Linterm1(var, coeff);
    }

     /** Whether x and this are structurally equal. */
    public boolean isEqual(Linterm1 x)
    {
        return var.equals(x.var) && coeff.isEqual(x.coeff);
    }

    /** Whether x is a Linterm1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Linterm1) && (isEqual((Linterm1)x));
    }

    public String getVariable()
    {
        return var;
    }

    public Coeff getCoefficient()
    {
        return coeff;
    }

    public void setVariable(String s)
    {
        var = s;
    }

    public void setCoeffcient(Coeff c)
    {
        coeff = c;
    }

}
