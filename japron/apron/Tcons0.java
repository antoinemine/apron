/*
 * Tcons0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 0 constraints on arbitrary expressions.
 *
 * <p> A Tcons0 represents a constraint over variables denoted
 * by dimension (i.e., integer starting from 0).
 * It has a left member (which is a {@link apron.Texpr0Intern} level 0
 * opaque expression), a constraint operator (=, &gt;=, &gt;, &lt;&gt;, or = modulo), 
 * a right member which is always 0, and  an optional constant modulo 
 * factor (for = modulo constraints only).
 */
public class Tcons0
    implements Cloneable, Serializable
{
    /** Left member of the constraint. */
    public Texpr0Intern expr;

    /**
     * Kind of constraint: {@link #EQ}, {@link #SUPEQ}, {@link #SUP}, 
     * {@link #EQMOD}, or {@link #DISEQ}. 
     */
    public int kind;

    /** 
     * Optional modulo factor.
     *
     * <p> null, except for {@link #EQMOD}. 
     */
    public Scalar scalar;

    
    // Constructors
    ///////////////

    /** Equality constraint: expr = 0. */
    static public final int EQ = 0;

    /** Inequality constraint: expr &gt;= 0.  */
    static public final int SUPEQ = 1;

    /** Strict inequality constraint: expr &gt; 0. */
    static public final int SUP = 2;

    /**
     * Congruence equality constraint modulo a constant: 
     * expr = 0 mod scalar. 
     */
    static public final int EQMOD = 3;

    /** Disquality constraint: expr &lt;&gt; 0. */
    static public final int DISEQ = 4;


    /**
     * Constructs a constraint with given kind and e as left member.
     *
     * <p> e is referenced, not copied.
     */
    public Tcons0(int k, Texpr0Intern e)
    { 
        this(k, e, null);
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     *
     * <p> e and s are referenced, not copied.
     */
    public Tcons0(int k, Texpr0Intern e, Scalar s)
    { 
        expr = e; 
        kind = k;
        scalar = s;
    }

    /**
     * Constructs a constraint with given kind and e as left member. 
     *
     * <p> e is converted to an opaque {@link apron.Texpr0Intern}
     * Apron expression tree.
     */
    public Tcons0(int k, Texpr0Node e)
    {
        this(k, new Texpr0Intern(e), null);
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     *
     * <p> e is converted to an opaque {@link apron.Texpr0Intern}
     * Apron expression tree.
     */
    public Tcons0(int k, Texpr0Node e, Scalar s)
    { 
        this(k, new Texpr0Intern(e), s);
    }

    /** Constructs a copy of c. 
     *
     * <p> The left member expression and the optional scalar are copied.
     */
    public Tcons0(Tcons0 c)
    { 
        this(c.kind, new Texpr0Intern(c.expr),
             (c.scalar==null) ? null : c.scalar.copy());
    }

    /**
     * Converts a linear constraint to a comb-line tree expression constraint. 
     */
    public Tcons0(Lincons0 c)
    { 
        this(c.kind, new Texpr0Intern(c.expr),
             (c.scalar==null) ? null : c.scalar.copy());
    }


    // Operations
    /////////////

    /**
     * Returns a string representation of the constraint.
     *
     * <p> Dimension i is denoted as xi.
     */
    public String toString() { return toString((Var[])null); }

    /** 
     * Returns a string representation of the constraint.
     *
     * <p> Dimension i is denoted by names[i].
     */
    public String toString(Var[] names)
    { 
        String s = expr.toString(names);
        switch (kind) {
        case EQ: return s + " = 0";
        case SUPEQ: return s + " >= 0";
        case SUP: return s + " > 0";
        case DISEQ: return s + " <> 0";
        case EQMOD: return s + " = 0 mod " + ((scalar==null) ? "<null>" : scalar.toString());
        default: throw new IllegalArgumentException("unknown constraint kind");
        }
    }

    public String toString(String[] names)
    {
        return toString(StringVar.arrayOfStrings(names));
    }

    /** Returns a hash of the constraint. */
    public int hashCode()
    {
        return expr.hashCode() ^ ((scalar==null) ? 0 : scalar.hashCode()) ^ kind;
    }


    /** 
     * Substitutes all occurrences of the dimension dim with 
     * the  tree t. 
     */
    public void substitute(int dim, Texpr0Intern t)
    {
        expr.substitute(dim,t);
    }

    /**
     * Returns a copy of this where all occurrences of the dimension dim 
     * are substituted with the expression tree t. 
     */
    public Tcons0 substituteCopy(int dim, Texpr0Intern t)
    {
        Tcons0 x = new Tcons0(this);
        x.substitute(dim, t);
        return x;
    }

    /** Adds some dimensions and shifts coefficients accordingly . */
    public void addDimensions(Dimchange c)
    {
        expr.addDimensions(c);
    }

    /** Removes some dimensions and shifts coefficients accordingly . */
    public void removeDimensions(Dimchange c)
    {
        expr.removeDimensions(c);
    }

    /** Permutes some dimensions in the constraint. */
    public void permuteDimensions(Dimperm p)
    {
        expr.permuteDimensions(p);
    }
    
    /** 
     * Returns a copy of the constraint with some dimensions added. 
     * 
     * <p> this is not changed.
     */
    public Tcons0 addDimensionsCopy(Dimchange c)
    { 
        Tcons0 l = new Tcons0(this);
        l.addDimensions(c);
        return l;
    }

    /**
     * Returns a copy of the constraint with some dimensions removed.
     *
     * <p> this is not changed.
     */
    public Tcons0 removeDimensionsCopy(Dimchange c)
    { 
        Tcons0 l = new Tcons0(this);
        l.removeDimensions(c);
        return l;
    }

    /**
     * Returns a copy of the constraint with some dimensions permuted. 
     *
     * <p> this is not changed.
     */
    public Tcons0 permuteDimensionsCopy(Dimperm p)
    {
        Tcons0 l = new Tcons0(this);
        l.permuteDimensions(p);
        return l;
    }


    // Get functions
    ////////////////

 
    /** Whether the expression is constant. */
    public boolean isIntervalCst()
    {
        return expr.isIntervalCst();
    }

    /** 
     * Whether the expression is linear with interval or scalar coefficients, 
     * and there is no rounding. 
     */
    public boolean isIntervalLinear()
    {
        return expr.isIntervalLinear();
    }

    /**
     * Whether the expression is polynomial, with interval or scalar 
     * coefficients, and there is no rounding. 
     */
    public boolean isIntervalPolynomial()
    {
        return expr.isIntervalPolynomial();
    }

    /** 
     * Whether the expression is a polynomial fraction, with interval or 
     * scalar coefficients, and there is no rounding. 
     */
    public boolean isIntervalPolyfrac()
    {
        return isIntervalPolyfrac();
    }

    /** Whether all coefficients are scalar (non interval). */
    public boolean isScalar()
    {
        return isScalar();
    }

    /**
     * Converts the expression tree to a concrete Java tree. 
     */
    public Texpr0Node toTexpr0Node()
    {
        return expr.toTexpr0Node();
    }


    /** Returns the depth of the expression tree. */
    public int getDepth()
    {
        return expr.getDepth();
    }

    /** Returns the number of internal nodes in the tree. */
    public int getSize()
    {
        return expr.getSize();
    }

    /** Maximum occurring dimension + 1. */
    public int maxDim()
    {
        return expr.maxDim();
    }

    /** Whether the dimension dim is present. */
    public boolean hasDim(int dim)
    {
        return expr.hasDim(dim);
    }

    /** Returns an ordered list of all occurring dimensions. */
    public int[] getDims()
    {
        return expr.getDims();
    }

    /** Returns the left expression. */
    public Texpr0Intern getExpression()
    {
        return expr;
    }

    /** Returns the constraint kind. */
    public int getKind()
    {
        return kind;
    }

    /** Returns the optional modulo factor (may be null). */
    public Scalar getScalar()
    {
        return scalar;
    }


    // Set functions
    ////////////////

    /** Sets the left expression. */
    public void setExpression(Texpr0Intern e)
    {
        expr = e;
    }

    /** Sets the constraint kind. */
    public void setKind(int k)
    {
        kind = k;
    }

    /** Sets the optional modulo factor (may be null). */
    public void setScalar(Scalar s)
    {
        scalar= (s==null) ? null : s.copy();
    }


    /** Returns a copy of this. */
    public Tcons0 clone()
    {
        return new Tcons0(this);
    }

    /** Whether x and this are structurally equal. */
    public boolean isEqual(Tcons0 x)
    {
        return expr.isEqual(x.expr) && kind==x.kind &&
            ((scalar==null && x.scalar==null) ||
             (scalar!=null && x.scalar!=null && scalar.isEqual(x.scalar)));
    }

    /** Whether x is a Tcons0 tree and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Tcons0) && (isEqual((Tcons0)x));
    }
}

