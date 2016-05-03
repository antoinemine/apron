/*
 * Texpr0Intern.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 0 expression trees in opaque Apron representation.
 *
 * <p> A Texpr0Intern represents an expression tree, with unary and binary
 * operator nodes, and {@link apron.Coeff} constant and variables
 * (represented by dimension) leaves.
 *
 * <p> A Texpr0Intern encapsulates an Apron {@code ap_texpr0_t} object,
 * an opaque object that can be passed to and retreived from Apron
 * abstract domains.
 * Texpr0Intern objects can be converted from and to {@link apron.Texpr0Node}
 * concrete expression trees for construction and inspection.
 */
public class Texpr0Intern
    implements Cloneable, Serializable
{

    // Internals
    ////////////
    
    /**
     * Actually, a pointer to an ap_texpr0_t object allocated in the C heap. 
     */
    private long ptr;

    private native void init(Texpr0Node c);
    private native void init(Texpr0Intern c);
    private native void init(Linexpr0 e);

    /** Deallocates the underlying ap_texpr0_t Apron object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }

    private Texpr0Intern() { }


    // Operations
    /////////////

    /** Converts a Java concrete tree to an opaque Apron tree (deep copy). */
    public Texpr0Intern(Texpr0Node t)
    { 
        init(t); 
    }

    /** Converts a linear expression to a comb-like expression tree. */
    public Texpr0Intern(Linexpr0 e)
    {
        init(e); 
    }

    /** Deep copy. */
    public Texpr0Intern(Texpr0Intern c)
    {
        init(c); 
    }

    /** Converts an opaque Apron tree to a concrete Java tree. */
    public native Texpr0Node toTexpr0Node();

    /** Returns the depth of the expression tree. */
    public native int getDepth();

    /** Returns the number of internal nodes in the tree. */
    public native int getSize();

    /** Maximum occurring dimension + 1. */
    public native int maxDim();

    /** Whether the dimension dim is present. */
    public native boolean hasDim(int dim);

    /** Returns an ordered list of all occurring dimensions. */
    public native int[] getDims();

    /** Whether the expression is constant. */
    public native boolean isIntervalCst();

    /** 
     * Whether the expression is linear with interval or scalar coefficients, 
     * and there is no rounding. 
     */
    public native boolean isIntervalLinear();

    /**
     * Whether the expression is polynomial, with interval or scalar 
     * coefficients, and there is no rounding. 
     */
    public native boolean isIntervalPolynomial();

    /** 
     * Whether the expression is a polynomial fraction, with interval or 
     * scalar coefficients, and there is no rounding. 
     */
    public native boolean isIntervalPolyfrac();

    /** Whether all coefficients are scalar (non interval). */
    public native boolean isScalar();

    /** 
     * Substitutes all occurrences of the dimension dim with 
     * the expression tree t. 
     */
    public native void substitute(int dim, Texpr0Intern t);

    /**
     * Returns a copy of this where all occurrences of the dimension dim
     * are substituted with the expression tree t. 
     */
    public Texpr0Intern substituteCopy(int dim, Texpr0Intern t)
    {
        Texpr0Intern x = new Texpr0Intern(this);
        x.substitute(dim, t);
        return x;
    }

    /** Adds some dimensions and shifts coefficients accordingly. */
    public native void addDimensions(Dimchange c);

    /** Returns a copy of this with some dimensions added. */
    public Texpr0Intern addDimensionsCopy(Dimchange c)
    {
        Texpr0Intern x = new Texpr0Intern(this);
        x.addDimensions(c);
        return x;
    }

    /** Removes some dimensions and shifts coefficients accordingly. */
    public native void removeDimensions(Dimchange c);

    /** Returns a copy of this with some dimensions removed. */
    public Texpr0Intern removeDimensionsCopy(Dimchange c)
    {
        Texpr0Intern x = new Texpr0Intern(this);
        x.removeDimensions(c);
        return x;
    }

    /** Permutes some dimensions in the expression. */
    public native void permuteDimensions(Dimperm p);

    /** Returns a copy of this with some dimensions permuted. */
    public Texpr0Intern permuteDimensionsCopy(Dimperm p)
    {
        Texpr0Intern x = new Texpr0Intern(this);
        x.permuteDimensions(p);
        return x;
    }

    /** Recursive hashing. */
    public native int hashCode();

    /** Structural (recursive) equality. */
    public native boolean isEqual(Texpr0Intern t);


    /** 
     * Converts the expression tree to a String. 
     *
     * <p> Dimension i is denoted as xi.
     **/
    public String toString()
    {
        return toString((Var[])null); 
    }

    /**
     * Converts the expression tree to a String.
     * 
     * <p> Dimension i is denoted by names[i].
     */
    public String toString(Var[] names)
    {
        return toTexpr0Node().toString(names);
    }

    public String toString(String[] names)
    {
        return toString(StringVar.arrayOfStrings(names));
    }


    /** Returns a deep copy of this. */
    public Texpr0Intern clone()
    {
        return new Texpr0Intern(this);
    }

    /** Whether x is a Texpr0Intern tree and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Texpr0Intern) && (isEqual((Texpr0Intern)x));
    }

    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        out.writeObject(toTexpr0Node());
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        init((Texpr0Node)(in.readObject()));
    }

    /** Fall-back deserialization function: returns 0. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init(new Texpr0CstNode());
    }
}
