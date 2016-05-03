/*
 * Texpr0DimNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of variable leaves in {@link apron.Texpr0Node} level 0
 * expression trees.
 *
 * <p> A variable is represented as a dimension (i.e., integer starting
 * from 0).
 */
public class Texpr0DimNode 
    extends Texpr0Node
    implements Cloneable, Serializable
{

    /** Dimension. */
    public int dim;


    // Operations
    /////////////

    /** Creates a new dimension node, for dimension d. */
    public Texpr0DimNode(int d)
    { 
        dim = d; 
    }

    public Texpr0Node shallowCopy()
    {
        return new Texpr0DimNode(dim); 
    }

    public Texpr0Node deepCopy()
    {
        return new Texpr0DimNode(dim); 
    }

    public Texpr1Node toTexpr1Node(Environment e)
    {
        return new Texpr1VarNode(e.varOfDim(dim));
    }

    public String toString(Var[] names)
    { 
        if (names==null) return "x" + dim;
        else return names[dim].toString();
    }

    public int getDimension()
    {
        return dim;
    }

    public void setDimension(int d)
    {
        dim  = d;
    }


    public int getDepth()
    { 
        return 0;
    }

    public int getSize()
    {
        return 0;
    }

    public int maxDim()
    {
        return dim + 1;
    }

    public boolean hasDim(int var)
    {
        return var==dim;
    }

    protected void fillDims(boolean[] ar)
    {
        ar[dim] = true;
    }

    public boolean isIntervalCst()
    {
        return false;
    }

    public boolean isIntervalLinear()
    {
        return true;
    }

    public boolean isIntervalPolynomial()
    {
        return true;
    }

    public boolean isIntervalPolyfrac()
    {
        return true;
    }

    public boolean isScalar()
    {
        return true;
    }

    public Texpr0Node substitute(int d, Texpr0Node t)
    {
        if (dim==d) return t;
        else return this;
    }

    public void addDimensions(Dimchange c)
    {
        dim = c.afterAdd(dim);
    }

    /**
     * Removes some dimensions and shifts coefficients accordingly. 
     *
     * Removed variables are replaced with [-oo,+oo] constants.
     */
    public Texpr0Node removeDimensions(Dimchange c)
    {
        dim = c.afterRemove(dim);
        if (dim==-1) {
            Interval i = new Interval();
            i.setTop();
            return new Texpr0CstNode(i);
        }
        return this;
    }

    public void permuteDimensions(Dimperm p)
    {
       dim = p.getElem(dim);
    }

    public int hashCode()
    {
        return dim;
    }

    public boolean isEqual(Texpr0Node t)
    {
        return 
            (t instanceof Texpr0DimNode) &&
            dim==((Texpr0DimNode)t).dim;
    }

    public int getPrecedence()
    {
        return 3;
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr0DimNode) && (isEqual((Texpr0DimNode)x));
    }
}
