/*
 * Texpr1CstNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.*;
import java.math.BigInteger;
import java.io.*;
import gmp.*;

/**
 * Class of constant leaves in {@link apron.Texpr1Node} level 1
 * expression trees.
 *
 * <p> A constant value is represented by a {@link apron.Coeff} object.
 */
public class Texpr1CstNode 
    extends Texpr1Node
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
    public Texpr1CstNode(Coeff c)
    {
        cst = c; 
    }

    /** Create a zero constant node (DoubleScalar). */
    public Texpr1CstNode()
    {
        cst = new DoubleScalar(0.); 
    }

    public Texpr1Node shallowCopy()
    {
        return new Texpr1CstNode(cst); 
    }

    public Texpr1Node deepCopy()
    {
        return new Texpr1CstNode(cst.copy()); 
    }

    public Texpr0Node toTexpr0Node(Environment e)
    {
        return new Texpr0CstNode(cst.copy());
    }

    public String toString()
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

    public boolean hasVar(Var var)
    {
        return false;
    }

    protected void fillVars(HashSet<Var> set)
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

    public Texpr1Node substitute(Var var, Texpr1Node t)
    {
        return this;
    }

    public int hashCode()
    {
        return cst.hashCode();
    }

    public boolean isEqual(Texpr1Node t)
    {
        return 
            (t instanceof Texpr1CstNode) &&
            cst.isEqual(((Texpr1CstNode)t).cst);
    }

    public int getPrecedence()
    {
        return 3;
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr1CstNode) && (isEqual((Texpr1CstNode)x));
    }
}
