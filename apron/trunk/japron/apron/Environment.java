/*
 * Environment.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * <p> Class of environments for level 1 objects.
 *
 * <p> An Environment is a set of integer-valued variables and 
 * a set of real-valued variable names.
 * Level 1 objects ({@link apron.Abstract1}, etc.) refer to variables
 * by name, and are defined relative to an environment.
 * In an environment, no two variables can have the same name
 * (even an integer-valued and a real-valued).
 *
 * <p> An environment maps level 1 variable names to level 0 dimensions.
 * First, it sorts variables into integer-valued variables first,
 * and real-valued variables next. Each set of variables is further
 * sorted in lexicographic order.
 * Methods to add, remove, or permute variables in environments take
 * care to maintain this order. Changes of environments on level 1
 * objects automatically apply any needed permutation required to retain
 * consistency with the environment.
 * 
 * <p> Environment objects are immutable.
 *
 * <p> An Environment object encapsulates an {@code ap_environment_t} 
 * Apron object  allocated in the C heap.
 */
public class Environment
    implements Cloneable, Serializable
{

    // Internals
    ////////////

    /**
     * Actually, a pointer to an ap_environment_t object allocated in the C 
     * heap. 
     */
    private long ptr;
 
    private native void init();
    private native void init(Var[] intVars, Var[] realVars);

    /** Deallocates the underlying Apron object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }


    // Constructors
    ///////////////

    /**
     * Constructs an empty environment.
     */
    public Environment()
    { 
        init(); 
    }

    public Environment(String[] intVars, String[] realVars)
    {
        this(StringVar.arrayOfStrings(intVars), StringVar.arrayOfStrings(realVars));
    }
    /**
     * Constructs an environment containing some integer-valued and
     * some real-valued variables.
     *
     * <p> All variable names must be distinct.
     */
    public Environment(Var[] intVars, Var[] realVars)
    { 
        init(intVars, realVars); 
    }


    // Operations
    /////////////
    
    public Environment add(String[] intVars, String[] realVars)
    {
        return add(StringVar.arrayOfStrings(intVars), StringVar.arrayOfStrings(realVars));
    }
    /**
     * Returns a new environment with some integer-valued and/or 
     * real-valued variables added.
     *
     * <p> this is not modified.
     * <p> One or both array(s) can be null.
     * <p> Added variables must not already exist.
     */
    public native Environment add(Var[] intVars, Var[] realVars);

    
    public Environment addPerm(String[] intVars, String[] realVars, Dimperm[] p)
    {
        return this.addPerm(StringVar.arrayOfStrings(intVars), StringVar.arrayOfStrings(realVars), p);
    }
    /**
     * Returns a new environment with some integer-valued and/or 
     * real-valued variables added.
     *
     * <p> Similar to {@link #add(Var[], Var[])}, but also returns
     * in p[0] the level 0 dimension permutation induced by the variable 
     * addition in order to keep them sorted.
     */
    public native Environment addPerm(Var[] intVars, Var[] realVars, Dimperm[] p);

    public Environment remove(String[] vars)
    {
        return remove(StringVar.arrayOfStrings(vars));
    }
    /**
     * Returns a new environment with some variables removed.
     *
     * <p> this is not modified.
     * <p> Removed variables must exist in this.
     */
    public native Environment remove(Var[] vars);


    /**
     * Returns the least environment extending this and op.
     *
     * <p> this and op are not modified.
     */
    public native Environment lce(Environment op);

    /**
     * Returns the least environment extending all environments in ar.
     *
     * <p> Environments in ar are not modified.
     */
    static public native Environment lce(Environment[] ar);

    public Environment rename(String[] org, String[] dst)
    {
        return this.rename(StringVar.arrayOfStrings(org), StringVar.arrayOfStrings(dst));
    }
    /**
     * Returns the environment obtained by mapping each variable
     * named org[i] to dst[i].
     *
     * <p> org[] and dst[] must have the same size.
     */
    public Environment rename(Var[] org, Var[] dst)
    {
        Dimperm[] p = null;
        return rename(org, dst, p);
    }

    public Environment rename(String[] org, String[] dst, Dimperm[] p)
    {
        return this.rename(StringVar.arrayOfStrings(org), StringVar.arrayOfStrings(dst), p);
    }
    /**
     * Returns the environment obtained by mapping each variable
     * named org[i] to dst[i].
     *
     * <p> Similar to {@link #rename(Var[], Var[])}, but also returns
     * in p[0] the level 0 dimension permutation to switch from the original
     * to the renamed environment.
     */
    public native Environment rename(Var[] org, Var[] dst, Dimperm[] p);



    // Access
    /////////

    /**
     * Returns the number of integer-valued and real-valued variables in the
     * environment.
     */
    public native Dimension getDimension();

    /**
     * Returns the total number of variables in the environment.
     */
    public int getSize()
    { 
        Dimension d = getDimension();
        return d.intDim + d.realDim;
    }

    public boolean hasVar(String var)
    {
        return hasVar(new StringVar(var));
    }
    /**
     * Whether a variable named var exists in the environment.
     */
    public native boolean hasVar(Var var);

    public int dimOfVar(String var)
    {
        return this.dimOfVar(new StringVar(var));
    }
    /**
     * Returns the index associated to the variable in the environment.
     *
     * <p> Throws an IllegalArgumentException if no such variable exists.
     */
    public native int dimOfVar(Var var);

    public boolean isInt(String var)
    {
        return isInt(new StringVar(var));
    }
    /**
     * Whether the given variable is integer-valued.
     */
    public boolean isInt(Var var)
    {
        return dimOfVar(var) < getDimension().intDim; 
    }

    /**
     * Whether the given dimension is integer-valued.
     */
    public boolean isInt(int dim)
    { 
        return dim < getDimension().intDim; 
    }

    /**
     * Returns the name of the variable at dimension dim.
     */
    public native Var varOfDim(int dim);

    /**
     * Returns the set of integer-valued variable names,
     * ordered in lexical order.
     */
    public native Var[] getIntVars();

    /**
     * Returns the set of real-valued variable names,
     * ordered in lexical order.
     */
    public native Var[] getRealVars();

    /**
     * Returns the set of all variables, ordered by dimension.
     *
     * <p> Integer-valued variables appear first, in lexicographic order,
     * then real-valued variables, also in lexicographic order.
     */
    public native Var[] getVars();

    /**
     * Returns the list of variable names in the environment,
     * ordered by dimension.
     */ 
    public String toString()
    {
        Var[] a = getIntVars();
        Var[] b = getRealVars();
        StringBuffer buf = new StringBuffer();
        boolean first = true;
        buf.append("( i: {");
        for (int i=0; i<a.length; i++) {
            if (first) buf.append(" "); else buf.append(", ");
            buf.append(a[i].toString());
            first = false;
        }
        first = true;
        buf.append(" }, r: {");
        for (int i=0; i<b.length; i++) {
            if (first) buf.append(" "); else buf.append(", ");
            buf.append(b[i]);
            first = false;
        }
        buf.append(" } )");
        return buf.toString();
    }


    /**
     * Returns the dimension specification to add to convert a 
     * level 0 object on the environment this to a level 0 object on 
     * the super-environment e.
     *
     * <p> e must be a super-environment of this.
     */
    public native Dimchange dimchange(Environment e);

    /**
     * Returns the dimension specification to add and to remove to 
     * convert a level 0 object on environment this to a level 0 object
     * on environment e.
     *
     * <p> To convert from a to b, assuming {@code r=a.dimchange2(b)}, you
     * should first add r[0] dimensions and then remove r[1]
     * dimensions.
     * <p> The two environments must not have variables with the
     * same name and different type.
     */
    public native Dimchange[] dimchange2(Environment e);


    // Comparisons
    //////////////


    /**
     * Whether this and e are equal. 
     */
    public native boolean isEqual(Environment e);


    /**
     * Whether this is a subset of e.
     */
    public native boolean isIncluded(Environment e);


    /**
     * Returns -2 if the environments are incompatible,
     * -1 if this is a subset of e, 
     * 0 if this and e are equal,
     * 1 if this is a superset of e,
     * and 2 otherwise, if this and e have a least common element.
     *
     * <p> Note: -2 is returned when there exists a variable that is
     * integer in one environment, and real in the other one.
     */
    public native int cmp(Environment e);


    /**
     * Returns a hash of the environment.
     */
    public native int hashCode();

    /** Returns this (as Environment are immutable). */
    public Environment clone()
    {
        return this;
    }

    /** Whether x is a Generator1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Environment) && (isEqual((Environment)x));
    }

    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        out.writeObject(getIntVars());
        out.writeObject(getRealVars());
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        Var[] i = (Var[])in.readObject();
        Var[] r = (Var[])in.readObject();
        init(i,r);
    }

    /** Fall-back deserialization function: empty environment. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init();
    }
}
