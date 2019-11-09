/*
 * Generator0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of geometrical features of level 0 linear abstract elements.
 *
 * <p> A generator is a 0- or 1-dimensional affine feature of a polyhedron:
 * a vertex, a ray, or a line.
 * Generator0 objects represent generators on level 0 abstract objects:
 * variables are denoted by dimension (i.e., integer starting from 0).
 *
 * <p> A Generator0 is represented as a (sparse or dense) linear expression, 
 * i.e., a {@link apron.Linexpr0} expression where the constant coefficient 
 * is ignored and all coefficients are scalar.
 */
public class Generator0
    implements Cloneable, Serializable
{
    /** 
     * Coordinates of the generator.
     *
     * <p> All coefficients must be scalar.
     * <p> The constant coefficient is ignored.
     */
    public Linexpr0 coord;

    /**
     * Kind of generator: {@link #LINE}, {@link #RAY}, {@link #VERTEX}, 
     * {@link #LINEMOD}, or {@link #RAYMOD}.
     */
    public int kind;

       
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

    /** 
     * Creates a new generator of the given kind with the given coordinate.
     *
     * <p> c is referenced, not copied.
     */
    public Generator0(int k, Linexpr0 c)
    { 
        coord = c; 
        kind = k; 
    }

    /**
     * Creates a copy of g.
     *
     * <p> The linear expression (coordinate) is copied.
     */
    public Generator0(Generator0 g)
    { 
        coord = new Linexpr0(g.coord);
        kind = g.kind; 
    }


    // Operations
    /////////////

    /**
     * Returns a string representation of the generator.
     *
     * <p> Dimension i is denoted as xi.
     */
    public String toString()
    { 
        return toString((Var[])null); 
    }

    /** 
     * Returns a string representation of the generator.
     *
     * <p> Dimension i is denoted by names[i].
     */
    public String toString(Var[] names)
    {
        StringBuffer buf = new StringBuffer();
        boolean first = true;
        Linterm0[] x = coord.getLinterms();
        switch (kind) {
        case LINE:    buf.append("LINE:    "); break;
        case RAY:     buf.append("RAY:     "); break;
        case VERTEX:  buf.append("VERTEX:  "); break;
        case LINEMOD: buf.append("LINEMOD: "); break;
        case RAYMOD:  buf.append("RAYMOD:  "); break;
        default: throw new IllegalArgumentException("unknown generator kind");
        }
        for (int i=0; i<x.length; i++) {
            Coeff c = x[i].coeff;
            if (c.isZero()) continue;
            if (!first && !(c instanceof Scalar && ((Scalar)c).sgn()<0)) buf.append(" +");
            else buf.append(" ");
            buf.append(c.toString());
            if (names==null) { buf.append('x'); buf.append(x[i].dim); }
            else buf.append(names[x[i].dim].toString());
            first = false;
        }
        if (first) buf.append("0");
        return buf.toString();        
    }

    public String toString(String[] names)
    {
        return toString(StringVar.arrayOfStrings(names));
    }

    /** Returns a hash of the generator. */
    public int hashCode()
    {
        return coord.hashCode() ^ kind;
    }

    /** Adds some dimensions and shifts coefficients accordingly . */
    public void addDimensions(Dimchange c)
    {
        coord.addDimensions(c);
    }

    /** Permutes some dimensions in the generator. */
    public void permuteDimensions(Dimperm p)
    {
        coord.permuteDimensions(p);
    }

    /**
     * Returns a copy of the generator with some dimensions added. 
     *
     * <p> this is not changed.
     */
    public Generator0 addDimensionsCopy(Dimchange c)
    { 
        Generator0 l = new Generator0(this);
        l.addDimensions(c);
        return l;
    }

    /**
     * Returns a copy of the generator with some dimensions permuted. 
     *
     * <p> this is not changed.
     */
    public Generator0 permuteDimensionsCopy(Dimperm p)
    {
        Generator0 l = new Generator0(this);
        l.permuteDimensions(p);
        return l;
    }

    /** Returns a copy of this. */
    public Generator0 clone()
    {
        return new Generator0(this);
    }

    
    /** Whether this and x have identical coordinate. */
    public boolean isEqual(Generator0 x)
    {
        return coord.isEqual(x.coord) && kind==x.kind;
    }

    /** Whether x is a Generator0 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Generator0) && (isEqual((Generator0)x));
    }

    public Linexpr0 getCoordinate()
    {
        return coord;
    }

    public int getKind()
    {
        return kind;
    }

    public void setCoordinate(Linexpr0 c)
    {
        coord = c;
    }

    public void setKind(int k)
    {
        kind = k;
    }
}
