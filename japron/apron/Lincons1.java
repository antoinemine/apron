/*
 * Lincons1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 1 linear constraints.
 *
 * <p> A Lincons1 represents a linear constraint over variables denoted
 * by names in an environment.
 *
 * <p> A Lincons1 is implemented as a pair containing a (sparse or dense)
 * {@link apron.Lincons0} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 */
public class Lincons1
    implements Cloneable, Serializable
{
 
    /** Level 0 constraint. */
    protected Lincons0 cons;

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
     * Congruence equality constraint modulo a constant: 
     * expr = 0 mod scalar. 
     */
    static public final int EQMOD = 3;

    /** Disquality constraint: expr &lt;&gt; 0. */
    static public final int DISEQ = 4;


    /** Initialization-less constructor, used by make. */
    private Lincons1() { }

    /** 
     * This function is used internally to construct a level 1 constraint
     * by referencing a level 0 constraint.
     *
     * <p> The level 0 constraint should no longer be used afterwards.
     */
    static public Lincons1 make(Environment v, Lincons0 x)
    {
        Lincons1 r = new Lincons1();
        r.cons = x;
        r.env = v;
        return r;
    }

    /** Constructs a universal constraint 0 &gt;= 0. */
    public Lincons1(Environment v)
    { 
        cons = new Lincons0();
        env = v;
    }

    /**
      * Constructs a constraint that is either universal (0 &gt;= 0), or 
      * unsatisfiable (-1 &gt;= 0).
      */
    public Lincons1(Environment v, boolean sat)
    { 
        cons = new Lincons0(sat);
        env = v;
    }

    /**
     * Constructs a constraint with given kind and e as left member.
     *
     * <p> e is copied.
     */
    public Lincons1(int k, Linexpr1 e)
    { 
        cons = new Lincons0(k, e.getLinexpr0());
        env = e.getEnvironment();
    }

    /**
     * Constructs a constraint with given kind, e as left member, and
     * s as modulo factor.
     *
     * <p> e is copied, s is referenced.
     */
    public Lincons1(int k, Linexpr1 e, Scalar s)
    { 
        cons = new Lincons0(k, e.getLinexpr0(), s);
        env = e.getEnvironment();
    }

    /**
     * Constructs a copy of c. 
     *
     * <p> The left member expression and the optional scalar are copied.
     */
    public Lincons1(Lincons1 c)
    { 
        cons = new Lincons0(c.cons);
        env = c.env;
    }

    /**
     * Creates a new Lincons1 from a Lincons0.
     *
     * <p> All coefficients are copied.
     * <p> The environment must be large enough for all variables in x.
     */
    public Lincons1(Environment e, Lincons0 x)
    {
        if (e.getSize() < x.maxDim())
            throw new IllegalArgumentException("environment is too small");
        cons = new Lincons0(x);
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
        cons.setCst(c);
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
        cons.setCoeff(env.dimOfVar(var), c); 
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

    /** Returns a copy of the Lincons0 underlying the constraint. */
    public Lincons0 getLincons0()
    {
        return new Lincons0(cons);
    }

    /**
     * <p> Returns a (read-only) reference to the Lincons0 underlying 
     * this constraint.
     *
     * <p> Warning: it is dangerous to add coefficients, resize the constraint,
     * add, remove, or permute dimensions as it may desynchrnonize the
     * Lincons0 dimensions and the environment.
     */
    public Lincons0 getLincons0Ref()
    {
        return cons;
    }

    /** Returns the constant coefficient in the left member expression. */
    public Coeff getCst()
    {
        return cons.getCst();
    }

    /** Returns a copy of the coefficient of variable var. */
    public Coeff getCoeff(Var var)
    {
        return cons.getCoeff(env.dimOfVar(var));
    }

    /**
     * Returns a copy of all variable coefficients.
     *
     * @see apron.Linexpr1#getLinterms
     */
    public Linterm1[] getLinterms()
    {
        Linterm0[] x = cons.getLinterms();
        Linterm1[] r = new Linterm1[x.length];
        for (int i=0; i<x.length; i++)
            r[i] = new Linterm1(env.varOfDim(x[i].dim), x[i].coeff);
        return r;
    }
    
    /**
     * Returns a copy of all variable coefficients.
     *
     * @see apron.Linexpr1#getCoeffs
     */
    public Coeff[] getCoeffs()
    {
        return cons.getCoeffs();
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
 

    // Tests
    ////////

    /** Whether the constraint is sparse or dense. */
    public boolean isSparse()
    {
        return cons.isSparse();
    }

    /**
     * Whether the constraint only depends on integer-valued variables.
     */
    public boolean isInteger()
    {
        return cons.isInteger(env.getDimension().intDim);
    }

    /**
     * Whether the constraint only depends on real-valued variables.
     */
    public boolean isReal()
    {
       return cons.isReal(env.getDimension().intDim);
    }

    /** Whether the constraint has only scalar coefficients. */
    public boolean isLinear()
    {
        return cons.isLinear();
    }

    /**
     * Whether the constraint has only scalar coefficients, except maybe
     * for the constant coefficient. 
    */
    public boolean isQuasilinear()
    {
        return cons.isQuasilinear();
    }

    /** 
     * Whether the constraint is trivially unsatisfiable.
     *
     * <p> Returns true if the constraint has the form b &gt;= 0 or [a,b] &gt;= 0
     * with b negative.
     */
    public boolean isUnsat()
    {
        return cons.isUnsat();
    }

    /** Whether this and x have identical kinds and coefficients. */
    public boolean isEqual(Lincons1 x)
    {
        return env.isEqual(x.env) && cons.isEqual(x.cons);
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
        cons.minimize();
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
    public Lincons1 extendEnvironmentCopy(Environment e)
    {
        return Lincons1.make(e, cons.addDimensionsCopy(env.dimchange(e)));
    }

    /** Returns a copy of this. */
    public Lincons1 clone()
    {
        return new Lincons1(this);
    }

    /** Whether x is a Lincons1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Lincons1) && (isEqual((Lincons1)x));
    }
}

