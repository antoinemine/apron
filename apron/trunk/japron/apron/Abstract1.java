/*
 * Abstract1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.Arrays;

/**
 * Class of level 1 numerical abstract values.
 *
 * <p> A level 1 numerical abstract value object represents a set of points 
 * in an environment.
 *
 * <p> An Abstract1 is implemented as a pair containing an
 * {@link apron.Abstract0} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 */
public class Abstract1
    implements Cloneable
{

    /** Level 0 abstract element. */
    protected Abstract0 abs;

    /** Environment. */
    protected Environment env;


    // Internals
    ////////////

    /** Constructor used internally. */
    private Abstract1(Environment e, Abstract0 a)
    {
        abs = a;
        env = e;
    }

    /** Checks that e1 and e2 are equal. */
    static private void checkEnv(Environment e1, Environment e2)
    {
        if (!e1.isEqual(e2))
            throw new IllegalArgumentException("different environments");
    }

    /** Checks that env and e are equal. */
    private void checkEnv(Environment e)
    {
        checkEnv(env, e);
    }

    /** Abstract1 -> Abstract0 for arguments that can be null. */
    private Abstract0 makeAbstract0Opt(Abstract1 x)
    {
        if (x==null) return null;
        checkEnv(x.env);
        return x.abs;
    }

    /** Abstract1[] -> Abstract0[], with environment checking. */
    static private Abstract0[] makeAbstract0Array(Abstract1[] ar)
    {
        Abstract0[] r = new Abstract0[ar.length];
        for (int i=0; i<ar.length; i++) 
            r[i] = ar[i].abs;
        for (int i=1; i<ar.length; i++)
            checkEnv(ar[0].env, ar[i].env);
        return r;
    }

    /** Lincons1[] -> Lincons0[], with environment checking. */
    private Lincons0[] makeLincons0Array(Lincons1[] ar)
    {
        Lincons0[] r = new Lincons0[ar.length];
        for (int i=0; i<ar.length; i++) {
            r[i] = ar[i].getLincons0Ref();
            checkEnv(ar[i].getEnvironment());
        }
        return r;
    }

    /** Tcons1[] -> Tcons0[], with environment checking. */
    private Tcons0[] makeTcons0Array(Tcons1[] ar)
    {
        Tcons0[] r = new Tcons0[ar.length];
        for (int i=0; i<ar.length; i++) {
            r[i] = ar[i].getTcons0Ref();
            checkEnv(ar[i].getEnvironment());
        }
        return r;
    }

    /** Generator1[] -> Generator0[], with environment checking. */
    private Generator0[] makeGenerator0Array(Generator1[] ar)
    {
        Generator0[] r = new Generator0[ar.length];
        for (int i=0; i<ar.length; i++) {
            r[i] = ar[i].getGenerator0Ref();
            checkEnv(ar[i].getEnvironment());
        }
        return r;
    }

    /** Linexpr1[] -> Linexpr0[], with environment checking. */
    private Linexpr0[] makeLinexpr0Array(Linexpr1[] ar)
    {
        Linexpr0[] r = new Linexpr0[ar.length];
        for (int i=0; i<ar.length; i++) {
            r[i] = ar[i].getLinexpr0Ref();
            checkEnv(ar[i].getEnvironment());
        }
        return r;
    }

    /** Texpr1[] -> Texpr0[], with environment checking. */
    private Texpr0Intern[] makeTexpr0InternArray(Texpr1Intern[] ar)
    {
        Texpr0Intern[] r = new Texpr0Intern[ar.length];
        for (int i=0; i<ar.length; i++) {
            r[i] = ar[i].getTexpr0InternRef();
            checkEnv(ar[i].getEnvironment());
        }
        return r;
    }

    /** Var[] variable names -> int[] dimensions */
    private int[] makeDimArray(Var[] ar)
    {
        int[] r = new int[ar.length];
        for (int i=0; i<ar.length; i++)
            r[i] = env.dimOfVar(ar[i]);
        return r;
    }


    // Constructors
    ///////////////

     /**
     * Creates a copy of the abstract element a. 
     */
    public Abstract1(Manager man, Abstract1 a)
        throws ApronException
    { 
        abs = new Abstract0(man, a.abs);
        env = a.env;
    }

    /**
     * Creates a new universal abstract element.
     */
    public Abstract1(Manager man, Environment e)
        throws ApronException
    {
        Dimension d = e.getDimension();
        abs = new Abstract0(man, d.intDim, d.realDim);
        env = e;
    }

    /**
     * Creates a new universal or empty abstract element.
     */
    public Abstract1(Manager man, Environment e, boolean empty)
        throws ApronException
    {
        Dimension d = e.getDimension();
        abs = new Abstract0(man, d.intDim, d.realDim, empty);
        env = e;
    }

    /**
     * Creates a new abstract element from a box.
     *
     * <p> The element is such that vars[i] is in interval box[i].
     * Variables not in vars have interval [-oo,+oo].
     * <p> All variables from vars must be exist in the environment e.
     * <p> vars and box must have the same length.
     */
    public Abstract1(Manager man, Environment e, Var[] vars, Interval[] box)
        throws ApronException
    {
        if (vars.length!=box.length)
            throw new IllegalArgumentException("incompatible array sizes");
        Dimension d = e.getDimension();
        Interval[] itv = new Interval[d.intDim + d.realDim];
        Interval it = new Interval();
        it.setTop();
        for (int i=0; i<d.intDim + d.realDim; i++)
            itv[i] = it;
        for (int i=0; i<vars.length; i++)
            itv[e.dimOfVar(vars[i])] = box[i];
        abs = new Abstract0(man, d.intDim, d.realDim, itv);
        env = e;
    }

    public Abstract1(Manager man, Environment e, String[] vars, Interval[] box)
        throws ApronException
    {
        this(man, e, StringVar.arrayOfStrings(vars), box);
    }


   /**
     * Creates a new abstract element from a set of constraints.
     *
     * <p> All constraints must have the same environment.
     * <p> The array must not be empty (to get an environment).
     */
    public Abstract1(Manager man, Lincons1[] c)
        throws ApronException
    {
        this(man, c[0].getEnvironment());
        meet(man, c); 
    }

    /**
     * Creates a new abstract element from a set of constraints.
     *
     * <p> All constraints must have the same environment.
     * <p> The array must not be empty (to get an environment).
     */
    public Abstract1(Manager man, Tcons1[] c)
        throws ApronException
    {
        this(man, c[0].getEnvironment());
        meet(man, c); 
    }


    // Access
    /////////

    /**
     * Returns the size in memory of the abstract element.
     *
     * <p> The unit is domain-specific.
     */
    public int getSize(Manager man)
        throws ApronException
    {
        return abs.getSize(man);
    }

    /**
     * Returns a hash of the element value.
     */
    public int hashCode(Manager man)
        throws ApronException
    {
        return abs.hashCode(man) ^ env.hashCode();
    }

    /**
     * Returns the environment of the abstract element.
     */
    public Environment getEnvironment()
    {
        return env;
    }

    /**
     * Returns a copy of the Abstract0 underlying this.
     */
    public Abstract0 getAbstract0(Manager man)
        throws ApronException
    {
        return new Abstract0(man, abs);
    }

    /**
     * Returns a (read-only) reference to the Abstract0 underlying 
     * this.
     *
     * <p> Warning: it is dangerous to add, remove, or permute dimensions 
     * as it may desynchrnonize the Abstract0 dimensions form the environment.
     */
    public Abstract0 getAbstract0Ref()
    {
        return abs;
    }

    /**
     * Returns the Manager which was used to create the
     * the abstract element.
     */
    public Manager getCreationManager()
    {
        return abs.getCreationManager();
    }

    /**
     * Returns a String representation of the element as a list of
     * constraints.
     *
     * <p> This method uses the Manager that was used to create this.
     */
   public String toString() 
    {
        return toString(getCreationManager());
    }
    
    /**
     * Returns a String representation of the element as a list of
     * constraints.
     */
    public String toString(Manager man) 
    {
        return abs.toString(man, env.getVars());
    }
    
   /**
     * Returns (an over-approximation of) the set of values e can 
     * take on this.
     *
     * <p> this and e must have the same environment.
     */
    public Interval getBound(Manager man, Linexpr1 e)
        throws ApronException
    {
        checkEnv(e.getEnvironment());
        return abs.getBound(man, e.getLinexpr0Ref());
    }

    /**
     * Returns (an over-approximation of) the set of values e can 
     * take on this.
     *
     * <p> this and e must have the same environment.
     */
    public Interval getBound(Manager man, Texpr1Intern e)
        throws ApronException
    {
        checkEnv(e.getEnvironment());
        return abs.getBound(man, e.getTexpr0InternRef());
    }

    public Interval getBound(Manager man, String var)
        throws ApronException
    {
        return getBound(man, new StringVar(var));
    }
  
    /**
     * Returns (an over-approximation of) the set of values the
     * variable var can take on this.
     */
    public Interval getBound(Manager man, Var var)
        throws ApronException
    {
        return abs.getBound(man, env.dimOfVar(var));
    }

    /**
     * Returns a set of linear constraints describing (an over-approximation of)
     * this.
     */
    public Lincons1[] toLincons(Manager man)
        throws ApronException
    {
        Lincons0[] x = abs.toLincons(man);
        Lincons1[] y = new Lincons1[x.length];
        for (int i=0; i<x.length; i++)
            y[i] = Lincons1.make(env, x[i]);
        return y;
    }

    /**
     * Returns a set of constraints describing (an over-approximation of)
     * this.
     */
    public Tcons1[] toTcons(Manager man)
        throws ApronException
    {
        Tcons0[] x = abs.toTcons(man);
        Tcons1[] y = new Tcons1[x.length];
        for (int i=0; i<x.length; i++)
            y[i] = Tcons1.make(env, x[i]);
        return y;
    }

    /**
     * Returns a box describing (an over-approximation of) this.
     *
     * <p> The variable order in the returned array is that of the
     * environment, i.e., {@code getEnvironment().getVars()}.
     */
    public Interval[] toBox(Manager man)
        throws ApronException
    {
        return abs.toBox(man);
    }

    /**
     * Returns a set of generators describing (an over-approximation of)
     * this.
     */
    public Generator1[] toGenerator(Manager man)
        throws ApronException
    {
        Generator0[] x = abs.toGenerator(man);
        Generator1[] y = new Generator1[x.length];
        for (int i=0; i<x.length; i++)
            y[i] = Generator1.make(env, x[i]);
        return y;
    }


    // Tests
    ////////

    /**
     * Whether the element is definitely empty (true) or maybe not empty
     * (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the element is 
     * definitely empty.
     */
    public boolean isBottom(Manager man)
        throws ApronException
    {
        return abs.isBottom(man);
    }

    /**
     * Whether the element is definitely universal (true) or maybe not 
     * universal (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the element is 
     * definitely not universal.
     */
    public boolean isTop(Manager man)
        throws ApronException
    {
        return abs.isTop(man);
    }

    /**
     * Whether this is definitely included in x (true) or maybe not 
     * included (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this is 
     * definitely not included in x.
     * <p> this and x must have the same environment.
     */
    public boolean isIncluded(Manager man, Abstract1 x)
        throws ApronException
    {
        checkEnv(x.env);
        return abs.isIncluded(man, x.abs);
    }


    /**
     * Whether this is definitely equal to x (true) or maybe not 
     * equal (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this is 
     * definitely not equal to x.
     * <p> this and x must have the same environment.
     */
    public boolean isEqual(Manager man, Abstract1 x)
        throws ApronException
    {
        checkEnv(x.env);
        return abs.isEqual(man, x.abs);
    }

    /**
     * Whether this definitely satisfies the constraint c (true) 
     * or maybe does not satisfy it (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this 
     * definitely does not satisfy c.
     * <p> this and c must have the same environment.
     */
    public boolean satisfy(Manager man, Lincons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        return abs.satisfy(man, c.getLincons0Ref());
    }

    /**
     * Whether this definitely satisfies the constraint c (true) 
     * or maybe does not satisfy it (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this 
     * definitely does not satisfy c.
     * <p> this and c must have the same environment.
     */
    public boolean satisfy(Manager man, Tcons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        return abs.satisfy(man, c.getTcons0Ref());
    }

    public boolean satisfy(Manager man, String var, Interval i)
        throws ApronException
    {
        return satisfy(man, new StringVar(var), i);
    }

    /**
     * Whether the projection of this on variable var is definitely
     * included in the interval i (true) or maybe is not included
     * (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that it is definitely
     * not included.
     */
    public boolean satisfy(Manager man, Var var, Interval i)
        throws ApronException
    {
        return abs.satisfy(man, env.dimOfVar(var), i);
    }

    public boolean isDimensionUnconstrained(Manager man, String var)
        throws ApronException
    {
        return isDimensionUnconstrained(man, new StringVar(var));
    }

    /**
     * Whether the variable var is definitely unbounded in this (true)
     * or may be bounded (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the dimension is
     * definitely bounded.
     */
    public boolean isDimensionUnconstrained(Manager man, Var var)
        throws ApronException
    {
        return abs.isDimensionUnconstrained(man, env.dimOfVar(var));
    }
    
    // Operations
    /////////////


    /**
     * Tries to minimize the memory representation of the element.
     *
     * <p> Use this to gain memory space. However, this may cause subsequent
     * operations to be less efficients as cached information may be
     * discarded.
     */
    public void minimize(Manager man)
        throws ApronException
    {
        abs.minimize(man);
    }

    /**
     * Tries to put the element in canonical form.
     *
     * <p> The notion of canonical form is not fully defined yet.
     */
    public void canonicalize(Manager man)
        throws ApronException
    {
        abs.canonicalize(man);
    }

    /**
     * Tries to simplify the representation of the element, maybe at
     * the expense of precision.
     *
     * <p> The algorithm argument indicates how aggressive the simplification 
     * should be. Its interpretation is domain-dependant.
     */
    public void approximate(Manager man, int algorithm)
        throws ApronException
    {
        abs.approximate(man, algorithm);
    }


    // Meet and join
    /////////////////

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this and op.
     *
     * <p> this and op are not modified.
     * <p> this and op must have the same environment.
     */
    public Abstract1 meetCopy(Manager man, Abstract1 op)
        throws ApronException
    {
        checkEnv(op.env);
        return new Abstract1(env, abs.meetCopy(man, op.abs));
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of the elements in ar.
     *
     * <p> The elements of ar are not modified.
     * <p> The array must not be empty.
     * <p> All array elements must have the same environment.
     */
    static public Abstract1 meet(Manager man, Abstract1[] ar)
        throws ApronException
    {
        return new Abstract1(ar[0].env,
                             Abstract0.meet(man, makeAbstract0Array(ar)));
    }
 
    /**
     * Replaces this with (an over-approximation of) the set-intersection of
     * this and op.
     *
     * <p> op is not modified.
     * <p> this and op must have the same environment.
     */
    public void meet(Manager man, Abstract1 op)
        throws ApronException
    {
        checkEnv(op.env);
        abs.meet(man, op.abs);
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-union of this and op.
     *
     * <p> this and op are not modified.
     * <p> this and op must have the same environment.
     */
    public Abstract1 joinCopy(Manager man, Abstract1 op)
        throws ApronException
     {
        checkEnv(op.env);
        return new Abstract1(env, abs.joinCopy(man, op.abs));
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-union of the elements in ar.
     *
     * <p> The elements of ar are not modified.
     * <p> The array must not be empty.
     * <p> All array elements must have the same environment.
     */
    static public Abstract1 join(Manager man, Abstract1[] ar)
        throws ApronException
     {
        return new Abstract1(ar[0].env,
                             Abstract0.join(man, makeAbstract0Array(ar)));
    }

    /**
     * Replaces this with (an over-approximation of) the set-union of
     * this and op.
     *
     * <p> op is not modified.
     * <p> this and op must have the same environment.
     */
    public void join(Manager man, Abstract1 op)
        throws ApronException
    {
        checkEnv(op.env);
        abs.join(man, op.abs);
    }


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraints in ar.
     *
     * <p> this is not changed.
     * <p> this and all array elements must have the same environment.
     */
    public Abstract1 meetCopy(Manager man, Lincons1[] ar)
        throws ApronException
    {
        return new Abstract1(env, abs.meetCopy(man, makeLincons0Array(ar)));
    }


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraints in ar.
     *
     * <p> this is not changed.
     * <p> this and all array elements must have the same environment.
     */
    public Abstract1 meetCopy(Manager man, Tcons1[] ar)
        throws ApronException
    {
        return new Abstract1(env, abs.meetCopy(man, makeTcons0Array(ar)));
    }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraints in ar.
     *
     * <p> this and all array elements must have the same environment.
     */
    public void meet(Manager man, Lincons1[] ar)
        throws ApronException
    {
        abs.meet(man, makeLincons0Array(ar));
    }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraints in ar.
     *
     * <p> this and all array elements must have the same environment.
     */
    public void meet(Manager man, Tcons1[] ar)
        throws ApronException
    {
        abs.meet(man, makeTcons0Array(ar));
    }


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraint cr.
     *
     * <p> this is not changed.
     * <p> this and c must have the same environment.
     */
    public Abstract1 meetCopy(Manager man, Lincons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        return new Abstract1(env, abs.meetCopy(man, c.getLincons0Ref()));
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraint in c.
     *
     * <p> this is not changed.
     * <p> this and c must have the same environment.
     */
    public Abstract1 meetCopy(Manager man, Tcons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        return new Abstract1(env, abs.meetCopy(man, c.getTcons0Ref()));
    }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraint c.
     *
     * <p> this and c must have the same environment.
     */
    public void meet(Manager man, Lincons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        abs.meet(man, c.getLincons0Ref());
    }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraint c.
     *
     * <p> this and c must have the same environment.
     */
    public void meet(Manager man, Tcons1 c)
        throws ApronException
    {
        checkEnv(c.getEnvironment());
        abs.meet(man, c.getTcons0Ref());
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * this with some rays added.
     *
     * <p> this is not changed.
     * <p> ar can only contain ray and lines, not vertices.
     * <p> this and all array elements must have the same environment.
     */
    public Abstract1 addRayCopy(Manager man, Generator1[] ar)
        throws ApronException
    {
        return new Abstract1(env, abs.addRayCopy(man, makeGenerator0Array(ar)));
    }
    
   /**
     * Replaces this with (an over-approximation of) this with some rays 
     * added.
     *
     * <p> ar can only contain ray and lines, not vertices.
     * <p> this and all array elements must have the same environment.
     */
    public void addRay(Manager man, Generator1[] ar)
        throws ApronException
    {
        abs.addRay(man, makeGenerator0Array(ar));
    }

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * this with one ray added.
     *
     * <p> this is not changed.
     * <p> this and r must have the same environment.
    */
    public Abstract1 addRayCopy(Manager man, Generator1 r)
        throws ApronException
    {
        checkEnv(r. getEnvironment());
        return new Abstract1(env, abs.addRayCopy(man, r.getGenerator0Ref()));
    }

   /**
     * Replaces this with (an over-approximation of) this with one ray
     * added.
     *
     * <p> this and r must have the same environment.
     */
    public void addRay(Manager man, Generator1 r)
        throws ApronException
    {
        checkEnv(r.getEnvironment());
        abs.addRay(man, r.getGenerator0Ref());
    }



    // Assignment and substitution
    //////////////////////////////

    public void assign(Manager man, String[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        assign(man, StringVar.arrayOfStrings(var), ar, dest);
    }
    
    /**
     * Replaces this with (an over-approximation of) this after
     * assigning ar[i] to  var[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public void assign(Manager man, Var[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        abs.assign(man, makeDimArray(var), makeLinexpr0Array(ar), 
                   makeAbstract0Opt(dest));
    }
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning ar[i] to var[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public Abstract1 assignCopy(Manager man, Var[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        return new Abstract1(env, abs.assignCopy(man, makeDimArray(var), makeLinexpr0Array(ar), makeAbstract0Opt(dest)));
    }
    public void assign(Manager man, String[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        assign(man, StringVar.arrayOfStrings(var), ar, dest);
    }
    /**
     * Replaces this with (an over-approximation of) this after
     * assigning ar[i] to var[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public void assign(Manager man, Var[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        abs.assign(man, makeDimArray(var), makeTexpr0InternArray(ar), 
                   makeAbstract0Opt(dest));
    }
    
    public Abstract1 assignCopy(Manager man, String[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {   
        return assignCopy(man, StringVar.arrayOfStrings(var), ar, dest);
    }

    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning ar[i] to var[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public Abstract1 assignCopy(Manager man, Var[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        return new Abstract1(env, abs.assignCopy(man, makeDimArray(var), makeTexpr0InternArray(ar), makeAbstract0Opt(dest)));
    }

    public void substitute(Manager man, String[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        substitute(man, StringVar.arrayOfStrings(var), ar, dest);
    }

    /**
     * Replaces this with (an over-approximation of) this after
     * substituting var[i] with ar[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
    */
    public void substitute(Manager man, Var[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        abs.substitute(man, makeDimArray(var), makeLinexpr0Array(ar), 
                       makeAbstract0Opt(dest));
    }
    

    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting var[i] with ar[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public Abstract1 substituteCopy(Manager man, Var[] var, Linexpr1[] ar, Abstract1 dest)
        throws ApronException
    {
        return new Abstract1(env, abs.substituteCopy(man, makeDimArray(var), makeLinexpr0Array(ar), makeAbstract0Opt(dest)));
    }
    
    public void substitute(Manager man, String[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        substitute(man, StringVar.arrayOfStrings(var), ar, dest);
    }
    /**
     * Replaces this with (an over-approximation of) this after
     * substituting var[i] with ar[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public void substitute(Manager man, Var[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        abs.substitute(man, makeDimArray(var), makeTexpr0InternArray(ar), 
                       makeAbstract0Opt(dest));
    }
    public Abstract1 substituteCopy(Manager man, String[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        return substituteCopy(man, StringVar.arrayOfStrings(var), ar, dest);
    }
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting var[i] with ar[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and the array must have the same environment.
     */
    public Abstract1 substituteCopy(Manager man, Var[] var, Texpr1Intern[] ar, Abstract1 dest)
        throws ApronException
    {
        return new Abstract1(env, abs.substituteCopy(man, makeDimArray(var), makeTexpr0InternArray(ar), makeAbstract0Opt(dest)));
    }

    public void assign(Manager man, String var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        assign(man, new StringVar(var), expr, dest);
    }
    /**
     * Replaces this with (an over-approximation of) this after
     * assigning expr to var.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public void assign(Manager man, Var var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        abs.assign(man, env.dimOfVar(var), expr.getLinexpr0Ref(), makeAbstract0Opt(dest));
    }

    public Abstract1 assignCopy(Manager man, String var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        return assignCopy(man, new StringVar(var), expr, dest);
    }
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning expr to var.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public Abstract1 assignCopy(Manager man, Var var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        return new Abstract1(env, abs.assignCopy(man, env.dimOfVar(var), expr.getLinexpr0Ref(), makeAbstract0Opt(dest)));
    }

    public void assign(Manager man, String var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        assign(man, new StringVar(var), expr, dest);
    }

    /**
     * Replaces this with (an over-approximation of) this after
     * assigning expr to var.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public void assign(Manager man, Var var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        abs.assign(man, env.dimOfVar(var), expr.getTexpr0InternRef(), makeAbstract0Opt(dest));
    }

    public Abstract1 assignCopy(Manager man, String var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        return assignCopy(man, new StringVar(var), expr, dest);
    }
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning expr to var.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public Abstract1 assignCopy(Manager man, Var var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        return new Abstract1(env, abs.assignCopy(man, env.dimOfVar(var), expr.getTexpr0InternRef(), makeAbstract0Opt(dest)));
    }


    public void substitute(Manager man, String var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        substitute(man, new StringVar(var), expr, dest);
    }
    /**
     * Replaces this with (an over-approximation of) this after
     * substituting var with expr.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public void substitute(Manager man, Var var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        abs.substitute(man, env.dimOfVar(var), expr.getLinexpr0Ref(), makeAbstract0Opt(dest));
    }


    public Abstract1 substituteCopy(Manager man, String var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    { 
        return substituteCopy(man, new StringVar(var), expr, dest);
    }
    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting var with expr.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public Abstract1 substituteCopy(Manager man, Var var, Linexpr1 expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        return new Abstract1(env, abs.substituteCopy(man, env.dimOfVar(var), expr.getLinexpr0Ref(), makeAbstract0Opt(dest)));
    }

    public void substitute(Manager man, String var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        substitute(man, new StringVar(var), expr, dest);
    }
    /**
     * Replaces this with (an over-approximation of) this after
     * substituting var with expr.
     *
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public void substitute(Manager man, Var var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        abs.substitute(man, env.dimOfVar(var), expr.getTexpr0InternRef(), makeAbstract0Opt(dest));
    }

    public Abstract1 substituteCopy(Manager man, String var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        return substituteCopy(man, new StringVar(var), expr, dest);
    }
    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting var with expr.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     * <p> this, dest, and expr must have the same environment.
     */
    public Abstract1 substituteCopy(Manager man, Var var, Texpr1Intern expr, Abstract1 dest)
        throws ApronException
    {
        checkEnv(expr.getEnvironment());
        return new Abstract1(env, abs.substituteCopy(man, env.dimOfVar(var), expr.getTexpr0InternRef(), makeAbstract0Opt(dest)));
    }


    // Projection
    /////////////
    public Abstract1 forgetCopy(Manager man, String [] var, boolean project)
        throws ApronException
    {
        return forgetCopy(man, StringVar.arrayOfStrings(var), project);
    }


    /**
     * Returns a new element containing (an over-approximation of) this after
     * forgetting the value of the variables in var.
     *
     * <p> this is not modified.
     * <p> If project is true, the variables are set to 0. 
     * Otherwise, they are set to [-oo,+oo].
     */
    public Abstract1 forgetCopy(Manager man, Var[] var, boolean project)
        throws ApronException
    {
        return new Abstract1(env, abs.forgetCopy(man, makeDimArray(var), project));
    }

    public void forget(Manager man, String[] var, boolean project)
        throws ApronException
    {
        forget(man, StringVar.arrayOfStrings(var), project);
    }

    /**
     * Replaces this with (an over-approximation of) this after
     * forgetting the value of the variables in var.
     *
     * <p> If project is true, the variables are set to 0. 
     * Otherwise, they are set to [-oo,+oo].
     */
    public void forget(Manager man, Var[] var, boolean project)
        throws ApronException
    {
        abs.forget(man, makeDimArray(var), project);
    }
    
    public  Abstract1 forgetCopy(Manager man, String var, boolean project)
        throws ApronException
    { 
        return forgetCopy(man, new StringVar(var), project);
    }

    /**
     * Returns a new element containing (an over-approximation of) this after
     * forgetting the value of the variable var.
     *
     * <p> this is not modified.
     */
    public  Abstract1 forgetCopy(Manager man, Var var, boolean project)
        throws ApronException
    { 
        return new Abstract1(env, abs.forgetCopy(man, env.dimOfVar(var), project));
    }

    public void forget(Manager man, String var, boolean project)
        throws ApronException
    { 
        forget(man, new StringVar(var), project);
    }

    /**
     * Replaces this with (an over-approximation of) this after
     * forgetting the value of the variable  var.
     */
    public void forget(Manager man, Var var, boolean project)
        throws ApronException
    { 
        abs.forget(man, env.dimOfVar(var), project);
    }


    // Change of environment
    ////////////////////////

    /**
     * Returns a copy of this with new environment e.
     *
     * <p> Variables in this and not in e are removed, while variables in
     * e and not in this are created.
     * <p> project indicates whether added variables are set to 0 (true) or
     * [-oo,+oo] (false).
     * <p> this and e must have compatible environments.
     * <p> this is not modified.
     */
    public Abstract1 changeEnvironmentCopy(Manager man, Environment e, boolean project)
        throws ApronException
    {
        Dimchange[] d = env.dimchange2(e);
        Abstract0 a = abs.addDimensionsCopy(man, d[0], project);
        a.removeDimensions(man, d[1]);
        return new Abstract1(e, a);
    }

    /**
     * Changes the environment into e.
     *
     * <p> Variables in this and not in e are removed, while variables in
     * e and not in this are created.
     * <p> project indicates whether added variables are set to 0 (true) or
     * [-oo,+oo] (false).
     * <p> this and e must have compatible environments.
     */
    public void changeEnvironment(Manager man, Environment e, boolean project)
        throws ApronException
    {
        Dimchange[] d = env.dimchange2(e);
        abs.addDimensions(man, d[0], project);
        abs.removeDimensions(man, d[1]);
        env = e;
    }

    /** Used by minimizeEnvironment */
    private Environment withoutUnconstrained(Manager man)
        throws ApronException
    {
        int total = env.getSize();
        boolean[] uncons = new boolean[total];
        for (int i=0; i<total; i++)
            uncons[i] = abs.isDimensionUnconstrained(man, i);
        int nb = 0;
        for (int i=0; i<total; i++)
            if (uncons[i]) nb++;
        Var[] vars = new Var[nb];
        for (int i=0, j=0; i<total; i++)
            if (uncons[i]) { vars[j] = env.varOfDim(i); j++; }
        return env.remove(vars);
    }

    /**
     * Returns a copy of this with all unconstrained variables removed.
     *
     * <p> this is not modified.
     */
    public Abstract1 minimizeEnvironmentCopy(Manager man)
        throws ApronException
    {
        return changeEnvironmentCopy(man, withoutUnconstrained(man), false);
    }

    /**
     * Removes all unconstrained variables.
     */
    public void minimizeEnvironment(Manager man)
        throws ApronException
    {
        changeEnvironment(man, withoutUnconstrained(man), false);
    }

    public Abstract1 renameCopy(Manager man, String[] org, String[] dst)
        throws ApronException
    {
        return renameCopy(man, StringVar.arrayOfStrings(org), StringVar.arrayOfStrings(dst));
    }

    /**
     * Returns a copy of this with variable org[i] renamed as dst[i].
     *
     * <p> org and dst must have the same size.
     * <p> this is not modified.
     */
    public Abstract1 renameCopy(Manager man, Var[] org, Var[] dst)
        throws ApronException
    {
        Dimperm[] p = new Dimperm[1];
        Environment e = env.rename(org, dst, p);
        return new Abstract1(e, abs.permuteDimensionsCopy(man, p[0]));
    }

    public void rename(Manager man, String[] org, String[] dst)
        throws ApronException
    {
        rename(man, StringVar.arrayOfStrings(org), StringVar.arrayOfStrings(dst));
    }

    /**
     * Renames variable org[i] as dst[i].
     *
     * <p> org and dst must have the same size.
     */
    public void rename(Manager man, Var[] org, Var[] dst)
        throws ApronException
    {
        Dimperm[] p = new Dimperm[1];
        env = env.rename(org, dst, p);
        abs.permuteDimensions(man, p[0]);
    }

    /**
     * Returns the meet of this and op on their least common environment.
     */
    public Abstract1 unifyCopy(Manager man, Abstract1 op)
        throws ApronException
    {
        Environment lce = env.lce(op.env);
        Abstract1 r = changeEnvironmentCopy(man, lce, false);
        r.meet(man, op.changeEnvironmentCopy(man, lce, false));
        return r;
    }

    /**
     * Replaces this with the meet of this and op on their least 
     * common environment.
     */
    public void unify(Manager man, Abstract1 op)
        throws ApronException
    {
        Environment lce = env.lce(op.env);
        changeEnvironment(man, lce, false);
        meet(man, op.changeEnvironmentCopy(man, lce, false));
    }


    // Expansion and folding
    ////////////////////////
     public Abstract1 expandCopy(Manager man, String org, String[] dst)
        throws ApronException
    {
        return expandCopy(man, new StringVar(org), StringVar.arrayOfStrings(dst));
    }
    
    /**
     * Returns a new element where variable org is duplicated as
     * dst[0] ... dst[dst.length-1].
     *
     * <p> this is not modified.
     */
    public Abstract1 expandCopy(Manager man, Var org, Var[] dst)
        throws ApronException
    {
        /* expand */
        Abstract0 r = abs.expandCopy(man, env.dimOfVar(org), dst.length);
        /* reorder & compute environment */
        Dimperm[] p = new Dimperm[1];
        Environment e;
        if (env.isInt(org)) e = env.addPerm(dst, null, p);
        else e = env.addPerm(null, dst, p);
        r.permuteDimensions(man, p[0]);
        return new Abstract1(e, r);
    }
    public void expand(Manager man, String org, String[] dst)
        throws ApronException
    {
        expand(man, new StringVar(org), StringVar.arrayOfStrings(dst)); 
    }
 
    /**
     * Duplicates the variable org as dst[0] ... dst[dst.length-1].
     */
    public void expand(Manager man, Var org, Var[] dst)
        throws ApronException
    {
        /* expand */
        abs.expand(man, env.dimOfVar(org), dst.length);
        /* reorder & compute environment */
        Dimperm[] p = new Dimperm[1];
        if (env.isInt(org)) env = env.addPerm(dst, null, p);
        else env = env.addPerm(null, dst, p);
        abs.permuteDimensions(man, p[0]);
    }

    public Abstract1 foldCopy(Manager man, String[] var)
        throws ApronException
    {
        return foldCopy(man, StringVar.arrayOfStrings(var));
    } 
    /**
     * Returns a new element where all the variables var[i]
     * are folded into the variable var[0].
     *
     * <p> The array must contain either only integer-valued or real-valued
     * variables.
     * <p> this is not modified.
     */
    public Abstract1 foldCopy(Manager man, Var[] var)
        throws ApronException
    {
        int[] dim = makeDimArray(var);
        Arrays.sort(dim);
        Var[] var0 = { env.varOfDim(dim[0]) };
        Var[] var1 = { var[0] };
        Environment e = env.remove(var);
        if (env.isInt(var0[0])) e = e.add(var0, null);
        else e = e.add(null, var0);
        Abstract1 r = new Abstract1(e, abs.foldCopy(man, dim));
        if (!var0[0].equals(var1[0])) r.rename(man, var0, var1);
        return r;
    }

     public void fold(Manager man, String[] var)
        throws ApronException
    {
        fold(man, StringVar.arrayOfStrings(var));
    }   
    /**
     * Folds all variables var[i] into variable var[0].
     *
     * <p> The array must contain either only integer-valued or real-valued
     * variables.
     */
    public void fold(Manager man, Var[] var)
        throws ApronException
    {
        int[] dim = makeDimArray(var);
        Arrays.sort(dim);
        Var[] var0 = { env.varOfDim(dim[0]) };
        Var[] var1 = { var[0] };
        if (env.isInt(var0[0])) env = env.remove(var).add(var0, null);
        else env = env.remove(var).add(null, var0);
        abs.fold(man, dim);
        if (!var0[0].equals(var1[0])) rename(man, var0, var1);
    }
 

    // Widening
    ///////////
    
     /**
     * Returns a new element equal to this widened with op.
     *
     * <p> this and op are not modified.
     * <p> this and op must have the same environment.
     */
    public Abstract1 widening(Manager man, Abstract1 op)
        throws ApronException
    {
        checkEnv(op.env);
        return new Abstract1(env, abs.widening(man, op.abs));
    }

   /**
     * Returns a new element equal to this widened with op,
     * with thresholds constraints set c.
     *
     * <p> this and op are not modified.
     * <p> this, op, and c must have the same environment.
    */
    public Abstract1 wideningThreshold(Manager man, Abstract1 op, Lincons1[] c)
        throws ApronException
    {
        checkEnv(op.env);
        return new Abstract1(env, abs.wideningThreshold(man, op.abs, makeLincons0Array(c)));
    }
  

    // Closure
    //////////

    /**
     * Returns a new element which is (an over-approximation of)
     * the topological closure of this.
     *
     * <p> this is not modified.
     */
    public Abstract1 closureCopy(Manager man)
        throws ApronException
    {
        return new Abstract1(env, abs.closureCopy(man));
    }
 
    /**
     * Replaces this with (an over-approximation of) its topological
     * closure.
     */
    public void closure(Manager man)
        throws ApronException
    {
        abs.closure(man);
    }



    // Serialization
    ////////////////

    /**
     * Converts the abstract element to a byte array.
     *
     * <p> The byte array can be stored to and retrieved from disk, or
     * sent through a network. However, the sending and receiving parties
     * must use compatible managers and identical environments.
     */
    public byte[] serialize(Manager man)
        throws ApronException
    {
        return abs.serialize(man);
    }

     /**
     * Restores an abstract element from a byte array.
     *
     * <p> The specified manager must be compatible with that used during
     * the {@link #serialize} call.
     * The environments must be identical.
     */
   static public Abstract1 deserialize(Manager man, Environment e, byte[] buf)
        throws ApronException
    {
        return new Abstract1(e, Abstract0.deserialize(man, buf));
    }

}
