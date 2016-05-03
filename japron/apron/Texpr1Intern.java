/*
 * Texpr1Intern.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 1 expression trees in opaque Apron representation.
 *
 * <p> A Texpr1Intern represents an expression tree, with unary and binary
 * operator nodes, and {@link apron.Coeff} constant and variables
 * (represented by names in an environment) leaves.
 *
 * <p> A Texpr1Intern is implemented as a pair containing an opaque
 * level 0 tree {@link apron.Texpr0Intern} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 *
 * <p> A Texpr1Intern encapsulates an Apron {@code ap_texpr0_t} object,
 * an opaque object that can be passed to and retreived from Apron
 * abstract domains.
 * Texpr1Intern objects can be converted from and to {@link apron.Texpr1Node}
 * concrete expression trees for construction and inspection.
 */
public class Texpr1Intern
    implements Cloneable, Serializable
{

    /** Level 0 expression. */
    protected Texpr0Intern expr;

    /** Environment. */
    protected Environment env;


    // Constructors
    ///////////////

    /** Initialization-less constructor, used by make. */
    private Texpr1Intern() { }

    /** 
     * This function is used internally to construct a new level 1
     * expression from a level 0 expression.
     *
     * <p> The level 0 expression should no longer be used afterwards.
     */
    static public Texpr1Intern make(Environment v, Texpr0Intern x)
    {
        Texpr1Intern r = new Texpr1Intern();
        r.expr = x;
        r.env = v;
        return r;
    }


    /** 
     * Converts a Java concrete tree to an opaque Apron tree (deep copy). 
     *
     * <p> As concrete Texpr1Node trees do not specify an environment,
     * it must be passed as argument to the conversion method.
     */
    public Texpr1Intern(Environment e, Texpr1Node t)
    {
        expr = new Texpr0Intern(t.toTexpr0Node(e));
        env = e;
    }

    /** Converts a linear expression to a comb-like expression tree. */
    public Texpr1Intern(Linexpr1 e)
    {
        expr = new Texpr0Intern(e.getLinexpr0Ref());
        env = e.getEnvironment();
    }

    /** Deep copy. */
    public Texpr1Intern(Texpr1Intern c)
    {
        expr = new Texpr0Intern(c.expr);
        env = c.env;
    }

    /**
     * Creates a new Texpr1Intern from a Texpr0Intern.
     *
     * <p> All coefficients are copied.
     * <p> The environment must be large enough for all variables in x.
     */
    public Texpr1Intern(Environment e, Texpr0Intern x)
    {
        if (e.getSize() < x.maxDim())
            throw new IllegalArgumentException("environment is too small");
        expr = new Texpr0Intern(x);
        env = e;
        
    }
 

    // Get functions
    ////////////////


    /** Converts an opaque Apron tree to a concrete Java tree. */
    public Texpr1Node toTexpr1Node()
    {
        return expr.toTexpr0Node().toTexpr1Node(env);
    }

    /**
     * Returns the environment of the expression.
     */
    public Environment getEnvironment()
    {
        return env;
    }

    /**
     * Returns a copy of the Texpr0Intern underlying this expression.
     */
    public Texpr0Intern getTexpr0Intern()
    {
        return new Texpr0Intern(expr);
    }

    /**
     * Returns a (read-only) reference to the Texpr0Intern underlying 
     * this expression.
     *
     * <p> Warning: it is dangerous to add, remove, or permute dimensions 
     * as it may desynchrnonize the Texpr0Intern dimensions form 
     * the environment.
     */
    public Texpr0Intern getTexpr0InternRef()
    {
        return expr;
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

    /** Whether the variable var is present. */
    public boolean hasVar(Var var)
    {
        return env.hasVar(var) && expr.hasDim(env.dimOfVar(var));
    }

    public boolean hasVar(String var)
    {
        return this.hasVar(new StringVar(var));
    }
    
    /**
     * Returns the list of all occurring variables. 
     *
     * <p> The ordering is that of dimensions.
     */
    public Var[] getVars()
    {
        int[] d = expr.getDims();
        Var[] s = new Var[d.length];
        for (int i=0; i<d.length; i++)
            s[i] = env.varOfDim(d[i]);
        return s;
    }
    
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
        return expr.isIntervalPolyfrac();
    }

    /** Whether all coefficients are scalar (non interval). */
    public boolean isScalar()
    {
        return expr.isScalar();
    }

    /** Recursive hashing. */
    public int hashCode()
    {
        return expr.hashCode() ^ env.hashCode();
    }

    /** Structural (recursive) equality. */
    public boolean isEqual(Texpr1Intern t)
    {
        return env.isEqual(t.env) && expr.isEqual(t.expr);
    }

    /**
     * Converts the expression tree to a String.
     */
    public String toString()
    {
        return expr.toString(env.getVars());
    }

 
    // Operations
    /////////////

    public void substitute(String var, Texpr1Intern t)
    {
        Var vn = new StringVar(var);

        this.substitute(vn, t);
    }
    /** 
     * Substitutes all occurrences of the variable var with 
     * the expression tree t. 
     *
     * <p> this and t must be defined on the same environment.
     */
    public void substitute(Var var, Texpr1Intern t)
    {
        if (!env.isEqual(t.env))
            throw new IllegalArgumentException("incompatible environments");
        expr.substitute(env.dimOfVar(var), t.expr);
    }

    public Texpr0Intern substituteCopy(String var, Texpr1Intern t)
    {
        Var vn = new StringVar(var);

        return this.substituteCopy(vn, t);
    }

    /**
     * Returns a copy of this where all occurrences of the variable var
     * are substituted with the expression tree t. 
     *
     * <p> this and t must be defined on the same environment.
     * <p> this is not modified.
     */
    public Texpr0Intern substituteCopy(Var var, Texpr1Intern t)
    {
        if (!env.isEqual(t.env))
            throw new IllegalArgumentException("incompatible environments");
        return expr.substituteCopy(env.dimOfVar(var), t.expr);
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
     * Returns an expression expanded to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     * <p> this is not modified.
     */
    public Texpr1Intern extendEnvironmentCopy(Environment e)
    {
        return Texpr1Intern.make(e, expr.addDimensionsCopy(env.dimchange(e)));
    }

    /** Returns a deep copy of this. */
    public Texpr1Intern clone()
    {
        return new Texpr1Intern(this);
    }

    /** Whether x is a Texpr1Intern tree and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Texpr1Intern) && (isEqual((Texpr1Intern)x));
    }

}
