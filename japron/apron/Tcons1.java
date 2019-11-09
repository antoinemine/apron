/*
 * Tcons1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 1 constraints on arbitrary expressions.
 *
 * <p> A Tcons1 represents a constraint over variables denoted
 * by name in an environment.
 *
 * <p> A Tcons1 is implemented as a pair containing a 
 * {@link apron.Tcons0} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 */
public class Tcons1
    implements Cloneable, Serializable
{
 
    /** Level 0 constraint. */
    protected Tcons0 cons;

    /** Environment. */
    protected Environment env;
    
   
    // Constructors
    ///////////////

    /** Equality constraint: expr = 0. */
    static public final int EQ = 0;

    /** Inequality constraint: expr &gt;= 0.  */
    static public final int SUPEQ = 1;

    /** Strict inequality constraint: expr &gt; 0. */
    static public final int SUP = 2;

    /** 
     * Congruence equality constraint modulo a constant: expr = 0 mod scalar. 
     */
    static public final int EQMOD = 3;

    /** Disquality constraint: expr &lt;&gt; 0. */
    static public final int DISEQ = 4;

    /** Initialization-less constructor, used by make. */
    private Tcons1() { }

    /** 
     * This function is used internally to construct a level 1 constraint
     * by referencing a level 0 constraint.
     *
     * <p> The level 0 constraint should no longer be used afterwards.
     */
    static public Tcons1 make(Environment v, Tcons0 x)
    {
        Tcons1 r = new Tcons1();
        r.cons = x;
        r.env = v;
        return r;
    }

    /**
     * Constructs a constraint with given kind and e as left member.
     *
     * <p> e is copied.
     */
    public Tcons1(int k, Texpr1Intern e)
    { 
        cons = new Tcons0(k, e.getTexpr0Intern());
        env = e.getEnvironment();
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     *
     * <p> e is copied, s is referenced.
     */
    public Tcons1(int k, Texpr1Intern e, Scalar s)
    { 
        cons = new Tcons0(k, e.getTexpr0Intern(), s);
        env = e.getEnvironment();
   }

    /** Constructs a constraint with given kind and e as left member. */
    public Tcons1(Environment v, int k, Texpr1Node e)
    {
        cons = new Tcons0(k, e.toTexpr0Node(v));
        env = v;
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     */
    public Tcons1(Environment v, int k, Texpr1Node e, Scalar s)
     {
         cons = new Tcons0(k, e.toTexpr0Node(v), s);
         env = v;
    }

    /**
     * Constructs a copy of c. 
     *
     * <p> The left member expression and the optional scalar are copied.
     */
    public Tcons1(Tcons1 c)
    { 
        cons = new Tcons0(c.cons);
        env = c.env;
    }

    /**
     * Converts a linear constraint to a comb-line tree expression constraint. 
     */
    public Tcons1(Lincons1 c)
    { 
        cons = new Tcons0(c.getLincons0Ref());
        env = c.getEnvironment();
    }

    /**
     * Creates a new Tcons1 from a Tcons0.
     *
     * <p> All coefficients are copied.
     * <p> The environment must be large enough for all variables in x.
     */
    public Tcons1(Environment e, Tcons0 x)
    {
        if (e.getSize() < x.expr.maxDim())
            throw new IllegalArgumentException("environment is too small");
        cons = new Tcons0(x);
        env = e;
    }


    // Get functions
    ////////////////

    /** Returns the environment of the constraint. */
    public Environment getEnvironment()
    {
        return env;
    }

   /** Returns the constraint kind. */
    public int getKind()
    {
        return cons.kind;
    }

    /** Returns the optional modulo factor (may be null). */
    public Scalar getScalar()
    {
        return cons.scalar;
    }

    /**
     * Converts the expression tree to a concrete Java tree. 
     */
    public Texpr1Node toTexpr1Node()
    {
        return cons.expr.toTexpr0Node().toTexpr1Node(env);
    }


    /** Returns a copy of the Tcons0 underlying the constraint. */
    public Tcons0 getTcons0()
    {
        return new Tcons0(cons);
    }

    /**
     * Returns a (read-only) reference to the Tcons0 underlying 
     * this constraint.
     *
     * <p> Warning: it is dangerous to add, remove, or permute dimensions 
     * as it may desynchrnonize the Tcons0 dimensions form the environment.
     */
    public Tcons0 getTcons0Ref()
    {
        return cons;
    }

    /** Returns the depth of the expression tree. */
    public int getDepth()
    {
        return cons.getDepth();
    }

    /** Returns the number of internal nodes in the tree. */
    public int getSize()
    {
        return cons.getSize();
    }

    public boolean hasVar(String var)
    {
        return this.hasVar(new StringVar(var));
    }
    
    /** Whether the variable var is present. */
    public boolean hasVar(Var var)
    {
        return env.hasVar(var) && cons.hasDim(env.dimOfVar(var));
    }

    /**
     * Returns the list of all occurring variables. 
     *
     * <p> The ordering is that of dimensions.
     */
    public Var[] getVars()
    {
        int[] d = cons.getDims();
        Var[] s = new Var[d.length];
        for (int i=0; i<d.length; i++)
            s[i] = env.varOfDim(d[i]);
        return s;
    }

    /** Whether the expression is constant. */
    public boolean isIntervalCst()
    {
        return cons.isIntervalCst();
    }

    /** 
     * Whether the expression is linear with interval or scalar coefficients, 
     * and there is no rounding. 
     */
    public boolean isIntervalLinear()
    {
        return cons.isIntervalLinear();
    }

    /**
     * Whether the expression is polynomial, with interval or scalar 
     * coefficients, and there is no rounding. 
     */
    public boolean isIntervalPolynomial()
    {
        return cons.isIntervalPolynomial();
    }

    /** 
     * Whether the expression is a polynomial fraction, with interval or 
     * scalar coefficients, and there is no rounding. 
     */
    public boolean isIntervalPolyfrac()
    {
        return cons.isIntervalPolyfrac();
    }

    /** Whether all coefficients are scalar (non interval). */
    public boolean isScalar()
    {
        return cons.isScalar();
    }

    /** 
     * Returns a string representation of the constraint.
     */
    public String toString()
    { 
        return cons.toString(env.getVars());
    }

    /** Returns a hash of the constraint. */
    public int hashCode()
    {
        return cons.hashCode() ^ env.hashCode();
    }
 

    // Set functions
    ////////////////
    
    /** Changes the kind of constraint. */
    public void setKind(int k)
    {
        cons.kind = k;
    }

    /** Sets the optional modulo factor (may be null). */
    public void setScalar(Scalar s)
    {
        cons.scalar = (s==null) ? null : s.copy();
    }



    // Operations
    /////////////

    public void substitute(String var, Texpr1Intern t)
    {
        substitute(new StringVar(var), t);
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
        cons.substitute(env.dimOfVar(var), t.getTexpr0InternRef());
    }

    public Tcons1 substituteCopy(String var, Texpr1Intern t)
    {
         return substituteCopy(new StringVar(var), t);
    }
    
    /**
     * Returns a copy of this where all occurrences of the variable var
     * are substituted with the expression tree t. 
     *
     * <p> this and t must be defined on the same environment.
     * <p> this is not modified.
     */
    public Tcons1 substituteCopy(Var var, Texpr1Intern t)
    {
        Tcons1 x = new Tcons1(this);
        x.substitute(var, t);
        return x;
    }

    /**
     * Expands the constraint to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     */
    public void extendEnvironment(Environment e)
    {
        cons.addDimensions(env.dimchange(e));
        env = e;
    }

    /**
     * Returns a constraint expanded to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     * <p> this is not modified.
     */
    public Tcons1 extendEnvironmentCopy(Environment e)
    {
        return Tcons1.make(e, cons.addDimensionsCopy(env.dimchange(e)));
    }

    /** Returns a copy of this. */
    public Tcons1 clone()
    {
        return new Tcons1(this);
    }

    /** Whether x and this are structurally equal. */
    public boolean isEqual(Tcons1 x)
    {
        return env.isEqual(x.env) && cons.isEqual(x.cons);
    }

    /** Whether x is a Tcons1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Tcons1) && (isEqual((Tcons1)x));
    }
}

