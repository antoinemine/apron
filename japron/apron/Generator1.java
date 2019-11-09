/*
 * Generator1.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of geometrical features of level 1 linear abstract elements.
 *
 * <p> A Generator1 represents a 0- or 1-dimensional affine feature of a 
 * polyhedron (vertex, ray, or line) over variables denoted
 * by names in an environment.
 *
 * <p> A Generator1 is implemented as a pair containing a (sparse or dense)
 * {@link apron.Generator0} and an {@link apron.Environment}, 
 * manipulated in conjunction.
 * Direct access to the fields is prohibited, to avoid desynchronizing them.
 */
public class Generator1
    implements Cloneable, Serializable
{
    /** Level 0 generator. */
    protected Generator0 gen;

    /** Environment. */
    protected Environment env;


    // Constructors
    ///////////////

    /** Line: { lambda * coord | lambda in R }. */
    static public final int LINE = 0;

    /** Ray: { lambda * coord | lambda &gt;= 0 in R }. */
    static public final int RAY = 1;

    /** Vertex: { coord }. */
    static public final int VERTEX = 2;

    /** Modular line: { lambda * coord | lambda in Z }. */
    static public final int LINEMOD = 3;

    /** Modular ray: { lambda * coord | lambda in N }. */
    static public final int RAYMOD = 4;

    /** Initialization-less constructor, used by make. */
    private Generator1() { }

    /** 
     * This function is used internally to construct a new level 1 generator
     * by referencing a level 0 generator.
     *
     * <p> The level 0 generator should no longer be used afterwards.
     */
    static public Generator1 make(Environment v, Generator0 x)
    {
        Generator1 r = new Generator1();
        r.gen = x;
        r.env = v;
        return r;
    }

    /** 
     * Creates a new generator of the given kind.
     *
     * <p> c is copied.
     */
    public Generator1(int k, Linexpr1 c)
    { 
        gen = new Generator0(k, c.getLinexpr0());
        env = c.getEnvironment();
    }

    /**
     * Creates a copy of g.
     *
     * <p> The linear expression (coordinate) is copied.
     */
    public Generator1(Generator1 g)
    { 
        gen = new Generator0(g.gen);
        env = g.env;
    }

    /**
     * Creates a new Generator1 from a Generator0.
     *
     * <p> All coefficients are copied.
     * <p> The environment must be large enough for all variables in x.
     */
    public Generator1(Environment e, Generator0 x)
    {
        if (e.getSize() < x.coord.maxDim())
            throw new IllegalArgumentException("environment is too small");
        gen = new Generator0(x);
        env = e;
    }


    // Operations
    /////////////

   /**
     * Returns the environment of the generator.
     */
    public Environment getEnvironment()
    {
        return env;
    }

    /**
     * Returns a copy of the Generator0 underlying this generator.
     */
    public Generator0 getGenerator0()
    {
        return new Generator0(gen);
    }

    /**
     * Returns a (read-only) reference to the Generator0 underlying 
     * this generator.
     *
     * <p> Warning: it is dangerous to add coefficients, resize the generator,
     * add, remove, or permute dimensions as it may desynchrnonize the
     * Generator0 dimensions form the environment.
     */
    public Generator0 getGenerator0Ref()
    {
        return gen;
    }

 
    /** Returns a String representation of the generator. */
    public String toString()
    { 
        return gen.toString(env.getVars());
    }

    /** Returns a hash of the generator. */
    public int hashCode()
    {
        return gen.hashCode() ^ env.hashCode();
    }

    /**
     * Expands the generator to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     */
    public void extendEnvironment(Environment e)
    {
        gen.addDimensions(env.dimchange(e));
        env = e;
    }

    /**
     * Returns a generator expanded to fit the given environment.
     *
     * <p> e must be a super-environment of that of this.
     * <p> this is not modified.
     */
    public Generator1 extendEnvironmentCopy(Environment e)
    {
        return Generator1.make(e, gen.addDimensionsCopy(env.dimchange(e)));
    }

    /** Returns a copy of this. */
    public Generator1 clone()
    {
        return new Generator1(this);
    }

    /** Whether this and x have identical coordinate and environment. */
    public boolean isEqual(Generator1 x)
    {
        return gen.isEqual(x.gen) && env.isEqual(x.env);
    }

    /** Whether x is a Generator1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Generator1) && (isEqual((Generator1)x));
    }
}
