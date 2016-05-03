/*
 * Texpr0CstNode.java
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
 * Class of constant leaves in {@link apron.Texpr0Node} level 0
 * expression trees.
 *
 * <p> A constant value is represented by a {@link apron.Coeff} object.
 */
public class Texpr0CstNode 
    extends Texpr0Node
    implements Cloneable, Serializable
{

    /** Constant value. */
    public Coeff cst;


    // Operations
    /////////////

    /**
     * Creates a new constant node. 
     *
     * <p> The coefficient is referenced, not copied. 
     */
    public Texpr0CstNode(Coeff c)
    {
        cst = c; 
    }

    /** Create a zero constant node (DoubleScalar). */
    public Texpr0CstNode()
    {
        cst = new DoubleScalar(0.); 
    }

    public Texpr0Node shallowCopy()
    {
        return new Texpr0CstNode(cst); 
    }

    public Texpr0Node deepCopy()
    {
        return new Texpr0CstNode(cst.copy()); 
    }

    public Texpr1Node toTexpr1Node(Environment e)
    {
        return new Texpr1CstNode(cst.copy());
    }

    public String toString(Var[] names)
    { 
        return cst.toString();
    }

    public Coeff getConstant()
    {
        return cst;
    }

    public void setConstant(Coeff c)
    {
        cst = c;
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
        return 0;
    }

    public boolean hasDim(int var)
    {
        return false;
    }

    protected void fillDims(boolean[] ar)
    {
    }

    public boolean isIntervalCst()
    {
        return true;
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
        return cst.isScalar();
    }

    public Texpr0Node substitute(int dim, Texpr0Node t)
    {
        return this;
    }

    public void addDimensions(Dimchange c)
    {
    }

    public Texpr0Node removeDimensions(Dimchange c)
    {
        return this;
    }

    public void permuteDimensions(Dimperm p)
    {
    }

    public int hashCode()
    {
        return cst.hashCode();
    }

    public boolean isEqual(Texpr0Node t)
    {
        return 
            (t instanceof Texpr0CstNode) &&
            cst.isEqual(((Texpr0CstNode)t).cst);
    }

    public int getPrecedence()
    {
        return 3;
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr0CstNode) && (isEqual((Texpr0CstNode)x));
    }
}
