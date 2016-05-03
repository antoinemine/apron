/*
 * Linexpr0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of level 0 linear expressions.
 *
 * <p> A Linexpr0 is a linear expression with {@link apron.Coeff} coefficients
 * and variables denoted by dimension (i.e., integer starting from 0).
 *
 * <p> A Linexpr0 object encapsulates an {@code ap_linexpr0_t} Apron object 
 * allocated in the C heap.
 *
 * <p> A Linexpr0 can be either sparse or dense. 
 * A dense Linexpr0 is similar to an array of coefficients. Its size is
 * set a creation time, but can be manually changed.
 * It has a coefficient for each dimension, from 0 to size-1.
 * A sparse Linexpr0 needs not be continuous: omitted dimensions
 * implicitly have a 0 coefficient. Also, sparse Linexpr0 grow 
 * automatically on-demand.
 */
public class Linexpr0
    implements Cloneable, Serializable
{

    // Internals
    ////////////

    /**
     * Actually, a pointer to an ap_linexpr0_t object allocated in the C heap. 
    */
    private long ptr;

    private native void init(boolean sparse, int size);
    private native void init(Linexpr0 c);

    /** Deallocates the underlying Apron object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }

    
    // Constructors
    ///////////////

    /** Creates a new empty sparse Linexpr0. */
    public Linexpr0() { init(true, 1); }

    /** 
     * Creates a new dense Linexpr0 of the given size.
     *
     * <p>All coefficient are initially set to 0.
     */
    public Linexpr0(int size) { init(false, size); }

    /** 
     * Creates a new dense or sparse Linexpr0 of the given size.
     *
     * <p> All coefficient are initially set to 0.
     */
    public Linexpr0(boolean sparse, int size) { init(sparse, size); }

    /** 
     * Creates a copy of c. 
     *
     * <p> All coefficients are copied. 
     */
    public Linexpr0(Linexpr0 c) { init(c); }

    /**
     * Creates a new sparse Linexpr0 with the specified dimension
     * and constant coefficient. 
     *
     * <p> All coefficients are copied.
     */
    public Linexpr0(Linterm0[] term, Coeff cst) 
    { 
        init(true, term.length);
        setCoeff(term);
        setCst(cst);
    }

    /**
     * Creates a new dense Linexpr0 with the specified dimension
     * and constant coefficients. 
     *
     * <p> The Linexpr0 has the same size as the term array.
     * <p> Dimension i gets coefficient term[i].
     * <p> All coefficients are copied.
     */
    public Linexpr0(Coeff[] term, Coeff cst) 
    { 
        init(false, term.length);
        for (int i=0; i<term.length; i++)
            setCoeff(i, term[i]);
        setCst(cst);
    }

    
    // Set functions
    ////////////////

    /** 
     * Sets the coefficient of dimension dim to c. 
     *
     * <p> Trying to set a dimension greater than the size is an error for
     * a dense Linexpr0. 
     * For sparse Linexpr0, if there is no coefficient for dim, one is created.
     * <p> dim must be positive.
     * <p> The coefficient is copied.
     */
    public native void setCoeff(int dim, Coeff c);

    /** Sets the constant coefficient to c. 
     *
     * <p> The coefficient is copied.
     */
    public native void setCst(Coeff c);

    /**
     * Sets several dimension coefficients at the same time. 
     *
     * <p> All coefficients are copied.
     */
    public void setCoeff(Linterm0[] c)
    {
        for (int i=0; i<c.length; i++)
            setCoeff(c[i].dim, c[i].coeff);
    }


    // Get functions
    ////////////////

    /**
     * Returns a copy of the coefficient of dimension dim.
     *
     * <p> dim must be positive.
     */
    public native Coeff getCoeff(int dim);

    /** Returns a copy of the constant coefficient. */
    public native Coeff getCst();

    /**
     * Returns a copy of all dimensions coefficients.
     *
     * <p> The array is sorted in increasing order of dimension.
     * For sparse Linexpr0, some dimensions may be missing.
     */
    public native Linterm0[] getLinterms();

    /**
     * Returns a copy of all dimension coefficients.
     */
    public Coeff[] getCoeffs()
    {
        Linterm0[] l = getLinterms();
        int nb = l[l.length-1].dim + 1;
        Coeff[] c = new Coeff[nb];
        for (int i=0; i<nb; i++)
            c[i] = Coeff.create();
        for (int i=0; i<l.length; i++)
            c[l[i].dim] = l[i].coeff;
        return c;
    }

    /** 
     * Returns the size of the Linexpr0. 
     *
     * <p> The size is fixed at creation time for dense Linexpr0, but
     * changes dynamically for sparse Linexpr0.
     **/
    public native int getSize();

    /** Maximum dimension occurring in the expression + 1. */
    public native int maxDim();

    /**
     * Returns a string representation of the expression. 
     *
     * <p> Dimension i is denoted as xi.
     */

    public String toString() { return toString((Var[])null); }

    public String toString(String[] names) {
        return toString(StringVar.arrayOfStrings(names));
    }

    /** 
     * Returns a string representation of the expression. 
     *
     * <p> Dimension i is denoted by names[i].
     */
    public String toString(Var[] names)
    {
        StringBuffer buf = new StringBuffer();
        boolean first = true;
        Linterm0[] x = getLinterms();
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
        Coeff c = getCst();
        if (!c.isZero()) {
            if (!first && !(c instanceof Scalar && ((Scalar)c).sgn()<0)) buf.append(" +");
            else buf.append(" ");
            buf.append(c.toString());
            first = false;
        }
        if (first) buf.append("0");
        return buf.toString();
    }

    /** Returns a hash of the expression. */
    public native int hashCode();


    // Tests
    ////////

    /** Whether the Linexpr0 is sparse or dense. */
    public native boolean isSparse();

    /**
     * Whether the expression only depends on the integer-valued dimensions
     * (dimensions 0 to intdim-1).
     */
    public native boolean isInteger(int intdim);

    /**
     * Whether the expression only depends on real-valued dimensions
     * (dimensions intdim and greater).
     */
    public native boolean isReal(int intdim);

    /** Whether the expression has only scalar coefficients. */
    public native boolean isLinear();

    /**
     * Whether the expression has only scalar coefficients, except maybe
     * for the constant coefficient. 
    */
    public native boolean isQuasilinear();

    /** Whether this and x have identical coefficients. */
    public native boolean isEqual(Linexpr0 x);

    /** Compares this and x with a lexicographic ordering. */
    public native int cmp(Linexpr0 x);


    // Operations
    /////////////

   /** 
     * Changes the size of the Linexpr0. 
     * 
     * <p> Added coefficients are set to 0. 
     */
    public native void resize(int size);

    /**
     * Simplifies the representation.
     *
     * <p> Transforms intervals into scalars when possible (when their bounds 
     * are equal).
     * For sparse Linexpr0, also removes zero coefficients.
     */
    public native void minimize();
 
    /** Adds some dimensions and shifts coefficients accordingly . */
    public native void addDimensions(Dimchange c);

    /** Permutes some dimensions in the expression. */
    public native void permuteDimensions(Dimperm p);

    /**
     * Returns a copy of the expression with some dimensions added. 
     *
     * <p> this is not changed.
    */
    public Linexpr0 addDimensionsCopy(Dimchange c)
    { 
        Linexpr0 l = new Linexpr0(this);
        l.addDimensions(c);
        return l;
    }

    /**
     * Returns a copy of the expression with some dimensions permuted. 
     *
     * <p> this is not changed.
     */
    public Linexpr0 permuteDimensionsCopy(Dimperm p)
    {
        Linexpr0 l = new Linexpr0(this);
        l.permuteDimensions(p);
        return l;
    }

    /** Returns a copy of this. */
    public Linexpr0 clone()
    {
        return new Linexpr0(this);
    }

    /** Whether x is a Linexpr0 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Linexpr0) && (isEqual((Linexpr0)x));
    }

    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        out.writeObject(getLinterms());
        out.writeObject(getCst());
        out.writeInt(getSize());
        out.writeBoolean(isSparse());
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        Linterm0[] term = (Linterm0[])in.readObject();
        Coeff cst = (Coeff)in.readObject();
        int size = in.readInt();
        boolean sparse = in.readBoolean();
        init(sparse, size);
        setCoeff(term);
        setCst(cst);
    }

    /** Fall-back deserialization function: returns 0. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init(true, 1);
    }
}
