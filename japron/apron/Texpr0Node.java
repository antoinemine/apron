/*
 * Texpr0Node.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 0 concrete expression trees.
 *
 * <p> A Texpr0Node represents an expression tree, which can be
 * constructed and inspected in Java.
 * Each node is actually a binary operator ({@link apron.Texpr0BinNode}), 
 * a unary operator ({@link apron.Texpr0UnNode}), a variable represented
 * by dimension ({@link apron.Texpr0DimNode}), or a constant 
 * ({@link apron.Texpr0CstNode}).
 *
 * <p> An expression tree must be converted to and from a
 * {@link apron.Texpr0Intern} to be used in abstract domain transfer 
 * functions.
 */
public abstract class Texpr0Node
    implements Cloneable, Serializable
{

    /** Shallow copy. */
    public abstract Texpr0Node shallowCopy();

    /** Deep copy. */
    public abstract Texpr0Node deepCopy();

    /** No constructor: use constructors from a sub-class. */
    protected Texpr0Node() { }

    /**
     * Constructs an expression tree from a linear expression.
     *
     * <p> All coefficients are copied.
     */
    static public Texpr0Node fromLinexpr0(Linexpr0 c)
    {
        Texpr0Node r = null;
        Coeff cst = c.getCst();
        if (!cst.isZero()) r = new Texpr0CstNode(cst.copy());
        Linterm0[] l = c.getLinterms();
        for (int i=0; i<l.length; i++) {
            if (l[i].coeff.isZero()) continue;
            Texpr0Node rr = 
                new Texpr0BinNode(Texpr0BinNode.OP_MUL,
                                  new Texpr0CstNode(l[i].coeff.copy()),
                                  new Texpr0DimNode(l[i].dim));
            r = (r==null) ? rr :
                new Texpr0BinNode(Texpr0BinNode.OP_ADD, r, rr);
        }
        if (r==null) return new Texpr0CstNode();
        else return r;
    }

    /**
     * Converts the level 0 tree to a level 1 tree in
     * environment e.
     *
     * <p> All dimension leaves are replaced with variable leaves.
     */
    public abstract Texpr1Node toTexpr1Node(Environment e);

    /** 
     * Converts the expression tree to a String.
     * 
     * <p> Dimension i is denoted as xi.
     */
    public String toString()
    { 
        return toString((Var[])null); 
    }

    /** 
     * Converts the expression tree to a String.
     * 
     * <p> Dimension i is denoted by names[i].
     */
    public abstract String toString(Var[] names);

    public String toString(String[] names)
    {
        return toString(StringVar.arrayOfStrings(names));
    }

    /** Returns the depth of the expression tree. */
    public abstract int getDepth();

    /** Returns the number of internal nodes in the tree. */
    public abstract int getSize();

    /** Maximum occurring dimension + 1. */
    public abstract int maxDim();

    /** Whether the dimension dim is present. */
    public abstract boolean hasDim(int dim);

    /** Used internally by getDims. */
    protected abstract void fillDims(boolean[] ar);

    /** Returns an ordered list of all occurring dimensions. */
    public int[] getDims()
    {
        /* computes the occurrence map */
        boolean[] ar = new boolean[maxDim()];
        for (int i=0; i<ar.length; i++) 
            ar[i] = false;
        fillDims(ar);
        /* count the number of distinct occurring variables */
        int nb = 0;
        for (int i=0; i<ar.length; i++) 
            if (ar[i]) nb++;
        /* fills the index list */
        int[] d = new int[nb];
        nb = 0;
        for (int i=0; i<ar.length; i++)
            if (ar[i]) {
                d[nb] = i;
                nb++;
            }
        return d;
    }

    /** Whether the expression is constant. */
    public abstract boolean isIntervalCst();

    /** 
     * Whether the expression is linear with interval or scalar coefficients, 
     * and there is no rounding. 
     */
    public abstract boolean isIntervalLinear();

    /**
     * Whether the expression is polynomial, with interval or scalar 
     * coefficients, and there is no rounding. 
     */
    public abstract boolean isIntervalPolynomial();

    /** 
     * Whether the expression is a polynomial fraction, with interval or 
     * scalar coefficients, and there is no rounding. 
     */
    public abstract boolean isIntervalPolyfrac();

    /** Whether all coefficients are scalar (non interval). */
    public abstract boolean isScalar();
    
    /** 
     * Substitutes all occurrences of the dimension dim with 
     * the expression tree t. 
     *
     * <p> this is modified in-place.
     * <p> As some nodes can change their kind (a node for a dimension dim
     * becomes the root of the tree t), a new root node is returned.
     */
    public abstract Texpr0Node substitute(int dim, Texpr0Node t);

    /**
     * Returns a copy of this where all occurrences of the dimension dim
     * are substituted with the expression tree t. 
     */
    public Texpr0Node substituteCopy(int dim, Texpr0Node t)
    {
        Texpr0Node x = this.deepCopy();
        return x.substitute(dim, t);
    }

    /** Adds some dimensions and shifts coefficients accordingly . */
    public abstract void addDimensions(Dimchange c);

    /** Returns a copy of this with some dimensions added. */
    public Texpr0Node addDimensionsCopy(Dimchange c)
    {
        Texpr0Node x = this.deepCopy();
        x.addDimensions(c);
        return x;
    }

    /**
     * Removes some dimensions and shifts coefficients accordingly 
     *
     * <p> this is modified in-place.
     * <p> As some nodes can change their kind (removed dimensions become
     * [-oo,+oo] constants), a new root node is returned.
     */
    public abstract Texpr0Node removeDimensions(Dimchange c);

    /** Returns a copy of this with some dimensions removed. */
    public Texpr0Node removeDimensionsCopy(Dimchange c)
    {
        Texpr0Node x = this.deepCopy();
        return x.removeDimensions(c);
    }

    /** Permutes some dimensions in the expression. */
    public abstract void permuteDimensions(Dimperm p);

    /** Returns a copy of this with some dimensions permuted. */
    public Texpr0Node permuteDimensionsCopy(Dimperm p)
    {
        Texpr0Node x = this.deepCopy();
        x.permuteDimensions(p);
        return x;
    }

    /** Recursive hashing. */
    public abstract int hashCode();

    /** Structural (recursive) equality. */
    public abstract boolean isEqual(Texpr0Node t);

    /** Precedence of the node, used when printing. */
    public abstract int getPrecedence();


    // Rounding format
    //////////////////

    /** Real (no rounding). */
    static public final int RTYPE_REAL = 0;

    /** Integer rounding. */
    static public final int RTYPE_INT = 1;

    /** Single-precision rounding: 32-bit IEEE 754 (float). */
    static public final int RTYPE_SINGLE = 2;

    /** Double-precision rounding: 64-bit IEEE 754 (double). */
    static public final int RTYPE_DOUBLE = 3;

    /** Extended-precision rounding: 80-bit (Intel's long double). */
    static public final int RTYPE_EXTENDED = 4;

    /** Quad-precision rounding: 128-bit (Motorola's long double). */
    static public final int RTYPE_QUAD = 5;

    /** Suffix String representation of a rounding type. */
    static public String suffixRType(int rtype)
    {
        switch (rtype) {
        case RTYPE_REAL:     return "";
        case RTYPE_INT:      return "i";
        case RTYPE_SINGLE:   return "f";
        case RTYPE_DOUBLE:   return "d";
        case RTYPE_EXTENDED: return "l";
        case RTYPE_QUAD:     return "q";
        default: throw new IllegalArgumentException("unknown rounding type: "+rtype);
        }
    }


    // Rounding direction
    /////////////////////
    
    /** Rounds to nearest. */
    static public final int RDIR_NEAREST = 0;
    
    /** Rounds towards 0 (truncation). */
    static public final int RDIR_ZERO = 1;
    
    /** Rounds towards +oo. */
    static public final int RDIR_UP = 2;

    /** Rounds towards -oo. */
    static public final int RDIR_DOWN = 3;

    /** Non-deterministic rounding (all four modes considered). */
    static public final int RDIR_RND = 4;

    /** Suffix String representation of a rounding direction. */
    static public String suffixRDir(int rdir)
    {
        switch (rdir) {
        case RDIR_NEAREST:    return "n";
        case RDIR_ZERO:       return "0";
        case RDIR_UP:         return "+oo";
        case RDIR_DOWN:       return "-oo";
        case RDIR_RND:        return "?";
        default: throw new IllegalArgumentException("unknown rounding direction: "+rdir);
        }
    }

    /** Returns a deep copy of this. */
    public Texpr0Node clone()
    {
        return deepCopy();
    }

    /** Whether x is a Texpr0 tree and structurally equal to this. */
    public abstract boolean equals(Object x);
}
