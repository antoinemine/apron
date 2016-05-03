/*
 * Linexpr1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 1 linear expressions.
 *
 * <p> A Linexpr1 is a linear expression with {@link apron.Coeff} coefficients
 * and variables denoted by names in an environment.
 *
 * <p> A Linexpr1 is implemented as a pair containing a (sparse or dense)
 * {@link apron.Linexpr0} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 */
public class Linexpr1
    implements Cloneable, Serializable
{

    /** Level 0 expression. */
    protected Linexpr0 expr;

    /** Environment. */
    protected Environment env;


    // Constructors
    ///////////////

    /** Initialization-less constructor, used by make. */
    private Linexpr1() { }

    /** 
     * This function is used internally to construct a level 1 expression
     * by referencing a level 0 expression.
     *
     * <p> The level 0 expression should no longer be used afterwards.
     */
    static public Linexpr1 make(Environment v, Linexpr0 x)
    {
        Linexpr1 r = new Linexpr1();
        r.expr = x;
        r.env = v;
        return r;
    }

    /** 
     * Creates a empty sparse expression of the given size on environment
     * e.
     *
     * <p> size is only an initial guess; the expression will grow
     * dynamically when needed.
     * <p> All coefficient are initially set to 0.
     */
    public Linexpr1(Environment e, int size) 
    { 
        expr = new Linexpr0(true, size); 
        env = e; 
    }

    /** Creates a new dense expression. */
    public Linexpr1(Environment e)
    {
        expr = new Linexpr0(false, e.getSize()); 
        env = e; 
    }

    /** 
     * Creates a copy of c. 
     *
     * <p> All coefficients are copied. 
     */
    public Linexpr1(Linexpr1 c) 
    { 
        expr = new Linexpr0(c.expr);
        env = c.env; 
    }

    /**
     * Creates a new sparse expression initialized with the corresponding 
     * terms and constant coefficient. 
     *
     * <p> All coefficients are copied.
     **/
    public Linexpr1(Environment e, Linterm1[] term, Coeff cst) 
    { 
         expr = new Linexpr0(true, term.length);
        env = e;
        setCoeff(term);
        setCst(cst);
    }

    /**
     * Creates a new dense expression initialized with the corresponding 
     * variable and constant coefficients. 
     *
     * <p> All coefficients are copied.
     * <p> The number of coefficients must match the environment size;
     **/
    public Linexpr1(Environment e, Coeff[] term, Coeff cst) 
    { 
        if (term.length != e.getSize())
            throw new IllegalArgumentException("invalid array size");
        expr = new Linexpr0(term, cst);
        env = e;
    }

    /**
     * Creates a new level 1 linear expression from a level 0 
     * linear expression.
     *
     * <p> All coefficients are copied.
     * <p> The environment must be large enough for all variables in x.
     */
    public Linexpr1(Environment e, Linexpr0 x)
    {
        if (e.getSize() < x.maxDim())
            throw new IllegalArgumentException("environment is too small");
        expr = new Linexpr0(x);
        env = e;
    }

    
    // Set functions
    ////////////////


    /**
     * Sets the constant coefficient to c. 
     *
     * <p> The coefficient is copied.
     */
    public void setCst(Coeff c)
    {
        expr.setCst(c);
    }

    public void setCoeff(String var, Coeff c)
    {
        setCoeff(new StringVar(var), c);
    }
    /** 
     * Sets the coefficient of variable var to c. 
     */
    public void setCoeff(Var var, Coeff c)
    { 
        expr.setCoeff(env.dimOfVar(var), c); 
    }

    /**
     * Sets several variable coefficients at the same time. 
     *
     * <p> All coefficients are copied.
     */
    public void setCoeff(Linterm1[] c)
    {
        for (int i=0; i<c.length; i++)
            setCoeff(c[i].var, c[i].coeff);
    }


    // Get functions
    ////////////////

    /**
     * Returns the environment of the expression.
     */
    public Environment getEnvironment()
    {
        return env;
    }

    /**
     * Returns a copy of the Linexpr0 underlying this expression.
     */
    public Linexpr0 getLinexpr0()
    {
        return new Linexpr0(expr);
    }

    /**
     * Returns a (read-only) reference to the Linexpr0 underlying 
     * this expression.
     *
     * <p> Warning: it is dangerous to add coefficients, resize the expression,
     * add, remove, or permute dimensions as it may desynchrnonize the
     * Linexpr0 dimensions and the Environment.
     */
    public Linexpr0 getLinexpr0Ref()
    {
        return expr;
    }

    /**
     * Returns a copy of the constant coefficient. 
     **/
    public Coeff getCst()
    {
        return expr.getCst();
    }

    public Coeff getCoeff(String var)
    {
        return getCoeff(new StringVar(var));
    }
    /**
     * Returns a copy of the coefficient of variable var. 
     */
    public Coeff getCoeff(Var var)
    {
        return expr.getCoeff(env.dimOfVar(var));
    }

    /**
     * Gets a copy of all variable coefficients.
     *
     * <p> All integer-valued variables appear before real-valued variables.
     * Integer-valued and real-valued variables appear in increasing
     * lexicographic order (i.e., the dimension order).
     * <p> For sparse expressions, some variable may be missing
     * (and should be considered implicitly as 0).
     */
    public Linterm1[] getLinterms()
    {
        Linterm0[] x = expr.getLinterms();
        Linterm1[] r = new Linterm1[x.length];
        for (int i=0; i<x.length; i++)
            r[i] = new Linterm1(env.varOfDim(x[i].dim), x[i].coeff);
        return r;
    }

    /**
     * Returns a copy of all variable coefficients.
     *
     * <p> All integer-valued variables appear before real-valued variables.
     * Integer-valued and real-valued variables appear in increasing
     * lexicographic order (i.e., the dimension order).
     */
    public Coeff[] getCoeffs()
    {
        return expr.getCoeffs();
    }

    /** 
     * Returns a string representation of the expression. 
     */
    public String toString()
    {
        return expr.toString(env.getVars());
    }


    /** Returns a hash of the expression. */
    public int hashCode()
    {
        return expr.hashCode() ^ env.hashCode();
    }


    // Tests
    ////////

    /** Whether the expression is sparse or dense. */
    public boolean isSparse()
    {
        return expr.isSparse();
    }

    /**
     * Whether the expression only depends on integer-valued variables.
     */
    public boolean isInteger()
    {
        return expr.isInteger(env.getDimension().intDim);
    }

    /**
     * Whether the expression only depends on real-valued variables.
     */
    public boolean isReal()
    {
       return expr.isReal(env.getDimension().intDim);
    }

    /** Whether the expression has only scalar coefficients. */
    public boolean isLinear()
    {
        return expr.isLinear();
    }

    /**
     * Whether the expression has only scalar coefficients, except maybe
     * for the constant coefficient. 
     */
    public boolean isQuasilinear()
    {
        return expr.isQuasilinear();
    }

    /** Whether this and x have identical coefficients and environment. */
    public boolean isEqual(Linexpr1 x)
    {
        return env.isEqual(x.env) && expr.isEqual(x.expr);
    }

    /**
     * Compares this and x.
     *
     * <p> The order is a lexicographic order of 
     * {@link apron.Environment#cmp} and {@link apron.Linexpr0#cmp}.
     */
    public int cmp(Linexpr1 x)
    {
        int c = env.cmp(x.env);
        if (c==0) return expr.cmp(x.expr);
        else return c;
    }



    // Operations
    /////////////

    /**
     * Simplifies the representation.
     *
     * @see apron.Linexpr0#minimize
     */
    public void minimize()
    {
        expr.minimize();
    }


    /**
     * Expands the expression to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     */
    public void extendEnvironment(Environment e)
    {
        expr.addDimensions(env.dimchange(e));
        env = e;
    }

    /**
     * Returns a new expression expanded to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     * <p> this is not modified.
     */
    public Linexpr1 extendEnvironmentCopy(Environment e)
    {
        return Linexpr1.make(e, expr.addDimensionsCopy(env.dimchange(e)));
    }

    /** Returns a copy of this. */
    public Linexpr1 clone()
    {
        return new Linexpr1(this);
    }

    /** Whether x is a Linexpr1 tree and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Linexpr1) && (isEqual((Linexpr1)x));
    }
}
