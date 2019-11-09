/*
 * Lincons0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 0 linear constraints.
 *
 * <p> A Lincons0 represents a linear constraint over variables denoted
 * by dimension (i.e., integer starting from 0).
 * It has a left member (which is a {@link apron.Linexpr0} level 0 linear
 * expression), a constraint operator (=, &gt;=, &gt;, &lt;&gt;, or = modulo), 
 * a right member which is always 0, and  an optional constant modulo 
 * factor (for = modulo constraints only).
 */
public class Lincons0
    implements Cloneable, Serializable
{
    /** Left member of the constraint. */
    public Linexpr0 expr;

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


    /** Constructs an universal constraint 0 &gt;= 0. */
    public Lincons0()
    { 
        this(SUPEQ, new Linexpr0(), null);
    }

     /**
      * Constructs a constraint that is either universal (0 &gt;= 0), or 
      * unsatisfiable (-1 &gt;= 0).
      */
    public Lincons0(boolean sat)
    { 
        expr = new Linexpr0();
        if (!sat) expr.setCst(new DoubleScalar(-1));
        kind = SUPEQ;
        scalar = null;
    }

    /**
     * Constructs a constraint with given kind and e as left member.
     *
     * <p> e is referenced, not copied.
     */
    public Lincons0(int k, Linexpr0 e)
    { 
        this(k, e, null);
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     *
     * <p> e and s are referenced, not copied.
     */
    public Lincons0(int k, Linexpr0 e, Scalar s)
    { 
        expr = e; 
        kind = k;
        scalar = s;
    }

    /**
     * Constructs a copy of c. 
     *
     * <p> The left member expression and the optional scalar are copied.
     */
    public Lincons0(Lincons0 c)
    { 
        this(c.kind, new Linexpr0(c.expr),
             (c.scalar==null) ? null : c.scalar.copy());
    }


    // Set functions
    ////////////////

    /** Sets the left expression. */
    public void setExpression(Linexpr0 e)
    {
        expr = e;
    }

    /** Sets the coefficient of dimension dim to c. */
    public void setCoeff(int dim, Coeff c)
    {
        expr.setCoeff(dim, c);
    }

    /** Sets the constant coefficient to c. */
    public void setCst(Coeff c)
    {
        expr.setCst(c);
    }

    /** Sets several dimension coefficients at the same time. */
    public void setCoeff(Linterm0[] c)
    {
        expr.setCoeff(c);
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

 
   // Get functions
    ////////////////

    /** Returns the left expression. */
    public Linexpr0 getExpression()
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

    /** Returns a copy of the coefficient of dimension dim. */
    public Coeff getCoeff(int dim)
    {
        return expr.getCoeff(dim);
    }
        
    /** Returns a copy of the constant coefficient. */
    public Coeff getCst()
    {
        return expr.getCst();
    }

    /** Returns a copy of all dimension coefficients. */
    public Linterm0[] getLinterms()
    {
        return expr.getLinterms();
    }

    /** Returns a copy of all dimension coefficients. */
    public Coeff[] getCoeffs()
    {
        return expr.getCoeffs();
    }

    /** Returns the size of the constraint. */
    public int getSize()
    {
        return expr.getSize();
    }

    /** Returns the maximum occurring dimension + 1. */
    public int maxDim()
    {
        return expr.maxDim();
    }

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


    // Tests
    ////////

    /** Whether the underlying expression is sparse or dense. */
    public boolean isSparse()
    {
        return expr.isSparse();
    }

    /**
     * Whether the constraint only depends on the integer-valued dimensions
     * (dimensions 0 to intdim-1).
     */
    public boolean isInteger(int intdim)
    {
        return expr.isInteger(intdim);
    }

    /**
     * Whether the constraint only depends on real-valued dimensions
     * (dimensions intdim and greater).
     */
    public boolean isReal(int intdim)
    {
        return expr.isReal(intdim);
    }

    /** Whether the constraint has only scalar coefficients. */
    public boolean isLinear()
    {
        return expr.isLinear();
    }

    /**
     * Whether the constraint has only scalar coefficients, except maybe
     * for the constant coefficient. 
     */
    public boolean isQuasilinear()
    {
        return expr.isQuasilinear();
    }

    /** 
     * Whether the constraint is trivially unsatisfiable.
     *
     * <p> Returns true if the constraint has the form b &gt;= 0 or [a,b] &gt;= 0
     * with b negative.
     */
    public native boolean isUnsat();


    /** Whether this and x have identical kinds and coefficients. */
    public boolean isEqual(Lincons0 x)
    {
        return
            kind == x.kind 
            &&
            ((scalar==null && x.scalar==null) 
             ||
             (scalar!=null && x.scalar!=null && scalar.isEqual(x.scalar))) 
            &&
            expr.isEqual(x.expr);
    }


    // Operations
    /////////////

   /** 
     * Changes the size of the underlying expression.
     * 
     * <p> Added coefficients are set to 0. 
     */
    public void resize(int size)
    {
        expr.resize(size);
    }

    /**
     * Simplifies the representation.
     *
     * @see apron.Linexpr0#minimize
     */
    public void minimize()
    {
        expr.minimize();
    }
 

    /** Adds some dimensions and shifts coefficients accordingly . */
    public void addDimensions(Dimchange c)
    {
        expr.addDimensions(c);
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
    public Lincons0 addDimensionsCopy(Dimchange c)
    { 
        Lincons0 l = new Lincons0(this);
        l.addDimensions(c);
        return l;
    }

    /**
     * Returns a copy of the constraint with some dimensions permuted. 
     *
     * <p> this is not changed.
     */
    public Lincons0 permuteDimensionsCopy(Dimperm p)
    {
        Lincons0 l = new Lincons0(this);
        l.permuteDimensions(p);
        return l;
    }

    /** Returns a copy of this. */
    public Lincons0 clone()
    {
        return new Lincons0(this);
    }

    /** Whether x is a Lincons0 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Lincons0) && (isEqual((Lincons0)x));
    }

}

