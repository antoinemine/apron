/*
 * Dimension.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of dimension specifications for level 0 objects.
 *
 * <p> A level 0 object ({@link apron.Abstract0}, etc.) has a set of
 * integer-valued and a set of real-valued dimensions.
 * A Dimension objects indicates how many dimensions of each kind.
 * It is simply a pair of positive integers: intDim and realDim.
 *
 * <p> In level 0 objects, integer-valued variables are numbered
 * from 0 to intDim-1, and real-valued variables are numbered from
 * intDim to intDim+realDim-1.
 */
public class Dimension
    implements Cloneable, Serializable
{

    /** Number of integer-valued dimensions. */
    public int intDim;

    /** Number of real-valued dimensions. */
    public int realDim;

    /** Constructs a new Dimension object. */
    public Dimension(int intdim, int realdim)
    {
        intDim = intdim;
        realDim = realDim;
    }

    /** Returns a String representation of the dimension. */
    public String toString()
    {
        return "( i: " + intDim + ", r: " + realDim + " )";
    }

    /** Whether x is equal to this. */
    public boolean isEqual(Dimension x)
    {
        return intDim==x.intDim && realDim==x.realDim;
    }

    /** Whether x is a Dimension and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Dimension) && (isEqual((Dimension)x));
    }

    public int getIntegerDimension()
    {
        return intDim;
    }

    public int getRealDimension()
    {
        return realDim;
    }

    public void setIntegerDimension(int d)
    {
        intDim = d;
    }

    public void setRealDimension(int d)
    {
        realDim = d;
    }

}
