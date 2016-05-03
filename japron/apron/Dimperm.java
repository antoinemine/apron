/*
 * Dimperm.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of dimension permutations for level 0 objects.
 *
 * <p> A Dimperm object encapsulates an {@code ap_dimperm_t} Apron object 
 * allocated in the C heap.
 */
public class Dimperm
    implements Cloneable, Serializable
{

    // Internals
    ////////////

    /**
     * Actually, a pointer to an ap_dimperm_t object allocated in the C heap. 
     */
    private long ptr;

    private native void init(int[] dim);
    private native void init(int size);

    /** Deallocates the underlying Apron object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }

    private Dimperm() { }

   
    // Constructors
    ///////////////

    /**
     * Creates a permutation.
     *
     * <p> The permutation maps dimension i to dimension dim[i].
     */
    public Dimperm(int[] dim)
    {
        init(dim); 
    }

    /** Creates an identity permutation of dimensions 0 to size-1. */
    public Dimperm(int size)
    {
        init(size); 
    }

    /** Creates a copy of this. */
    public Dimperm(Dimperm x)
    {
        init(x.getContents());
    }


    // Operations
    /////////////

    /** Sets this to the identity permutation. */
    public native void setId();

    /** Changes the permutation so that i maps to dim. */
    public native void setElem(int i, int dim);

    /** Gets the variable index i is mapped to. */
    public native int getElem(int i);

    /** Returns the number of elements in this permutation. */
    public native int getSize();

    /**
     * Returns the permutation map. 
     *
     * <p> if x=a.getContents(), then a maps dimension i to x[i].
     */
    public native int[] getContents();

    /** 
     * Returns a new permutation which is the inverse of this. 
     *
     * <p> this is not modified.
     */
    public native Dimperm invert();

    /**
     * Returns a permutation equal to this composed with op. 
     *
     * <p> The result maps i to op[this[i]].
     * <p> this and op must have the same size.
     * <p> this is not changed.
     */
    public native Dimperm compose(Dimperm op);

    /** Returns a String representation of the permutation. */
    public String toString() {
        StringBuffer buf = new StringBuffer();
        int[] c = getContents();
        buf.append("[");
        for (int i=0; i<c.length; i++) {
            if (i==0) buf.append(" "); else buf.append(", ");
            buf.append(i); buf.append(" -> "); buf.append(c[i]);
        }
        buf.append(" ]");
        return buf.toString();
    }

    /** Returns a copy of this. */
    public Dimperm clone()
    {
        return new Dimperm(this);
    }

    /** Whether x is equal to this. */
    public boolean isEqual(Dimperm x)
    {
        int[] c1 = getContents();
        int[] c2 = x.getContents();
        if (c1.length != c2.length) return false;
        for (int i=0; i<c1.length; i++)
            if (c1[i] != c2[i]) return false;
        return true;
    }

    /** Whether x is a Dimperm and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Dimperm) && (isEqual((Dimperm)x));
    }

    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        out.writeObject(getContents());
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        init((int[])in.readObject());
    }

    /** Fall-back deserialization function: empty permutation. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        init(0);
    }
}

