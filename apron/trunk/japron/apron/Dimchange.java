/*
 * Dimchange.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of dimension change specifications for level 0 objects.
 *
 * <p> A Dimchange object specifies how many and which dimensions to add to or
 * to remove from a level 0 object ({@link apron.Abstract0}, etc.).
 *
 * <p> A Dimchange object encapsulates a {@code ap_dimchange_t} Apron object
 * allocated in the C heap.
 */
public class Dimchange
    implements Cloneable, Serializable
{

    // Internals
    ////////////

    /**
     * A pointer to an {@code ap_dimchange_t} object allocated in the C  heap. 
     */
    private long ptr;

    private native void init(int intdim, int realdim, int[] dim);

    /** Deallocates the underlying Apron object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }

    private Dimchange() { }


    // Constructors
    ///////////////

    /**
     * Creates a Dimchange.
     *
     * <p> The Dimchange object adds or removes intdim integer-valued
     * and realdim real-valued dimensions.
     * The array dim[], of size intdim+realdim, specifies the position of
     * dimensions to add or remove. 
     *
     * <p> When used to add dimensions, dim[i]=k indicates to add one dimension
     * at position k and shift (increment) the dimensions greater than or 
     * equal to k.
     *
     * <p> When used to remove dimensions, dim[i]=k indicates to remove the
     * dimension k and shift (decrement) the dimensions greater than k.
     */
    public Dimchange(int intdim, int realdim, int[] dim)
    {
        init(intdim, realdim, dim); 
    }


    // Get
    //////

    /**
     * Returns the number of integer-valued and real-valued 
     * variables to be added or removed. 
     */
    public native Dimension getDimension();

    /** Returns the position of the dimensions to be added or removed. */
    public native int[] getContents();

    /** Returns a String representation of the change of dimension. */
    public String toString()
    {
        StringBuffer buf = new StringBuffer();
        Dimension d = getDimension();
        int[] c = getContents();
        buf.append("( i: [");
        boolean first = true;
        for (int pos=0; pos<d.intDim; pos++) {
            if (first) buf.append(" "); else buf.append(", ");
            buf.append(c[pos]);
            first = false;
        }
        buf.append(" ], r: [");
        first = true;
        for (int pos=0; pos<d.realDim; pos++) {
            if (first) buf.append(" "); else buf.append(", ");
            buf.append(c[d.intDim+pos]);
            first = false;
        }
        buf.append(" ] )");
        return buf.toString();
    }


    // Operations
    /////////////

    /**
     * Indicates which dimension i is mapped to, assuming this is used
     * to add dimensions.
     */
    public native int afterAdd(int i);

    /** 
     * Indicates which dimension i is mapped to, assuming this is used
     * to remove dimensions.
     *
     * <p>Returns -1 if the variable index would be removed.
     */
    public native int afterRemove(int i);

    /**
     * Inverts this.
     *
     * <p> Assuming that this is used to add some variables then, 
     * after inversion, it can be used to remove variables and
     * recover the original variable set.
     */
    public native void addInvert();


    /** Returns a copy of this. */
    public Dimchange clone()
    {
        Dimension d = getDimension();
        return new Dimchange(d.intDim, d.realDim, getContents());
    }

    /** Whether x is equal to this. */
    public boolean isEqual(Dimchange x)
    {
        if (x==this) return true;
        Dimension d = getDimension();
        if (!d.isEqual(x.getDimension())) return false;
        int[] c1 = getContents();
        int[] c2 = x.getContents();
        for (int i=0; i<c1.length; i++)
            if (c1[i] != c2[i]) return false;
        return true;
    }

    /** Whether x is a Generator1 and structurally equal to this. */
    public boolean equals(Object x)
    {
        return (x instanceof Dimchange) && (isEqual((Dimchange)x));
    }

    /** Serializes this. */
    private void writeObject(ObjectOutputStream out)
        throws IOException
    { 
        out.writeObject(getDimension());
        out.writeObject(getContents());
    }
    
    /** Reads back a serialized Mpz. */
    private void readObject(ObjectInputStream in)
        throws IOException, ClassNotFoundException
    { 
        Dimension d = (Dimension)in.readObject();
        int[] c = (int[])in.readObject();
        init(d.intDim, d.realDim, c);
    }

    /** Fall-back deserialization function: no change. */
    private void readObjectNoData() 
        throws ObjectStreamException
    {
        int[] x = new int[1];
        init(0,0,x);
    }
}

