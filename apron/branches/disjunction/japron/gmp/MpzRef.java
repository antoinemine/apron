/*
 * MpzRef.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.io.*;

/**
 * Class of references to numerators and denominator of {@link gmp.Mpq}
 * objects.
 *
 * <p> A {@code mpq_t} GMP C object contains a numerator and a denominator
 * which are themselves {@code mpz_t} GMP C objects and can be manipulated 
 * independently.
 * The MpzRef class proposes a similar interface for Java.
 * A MpzRef to reference the numerator or the denominator of a {@link gmp.Mpq}
 * object can be obtained using the {@link gmp.Mpq#numRef} and
 * {@link gmp.Mpq#denRef} methods.
 * The MpzRef can then be manipulated exactly as a {@link gmp.Mpz}, from which
 * it inherits all methods.
 * This allows manipulating {@link gmp.Mpq} numerators and denominators
 * in-place, without requiring any copy.
 *
 * <p> MpzRef cannot be serialized as it represents a reference to a part of
 * a larger object.
 */
public class MpzRef extends Mpz
{
    /**
     * We keep a pointer to the parent {@link gmp.Mpq} object so that it is 
     * not freed while a MpzRef on it is live.
     */
    protected Object parent;

    /**
     * A MpzRef can only be constructed by a {@link gmp.Mpq} object. 
     */
    private MpzRef() { assert(false); }

    /**
     * MpzRef are not clonable.
     *
     * @throws CloneNotSupportedException
     */
    public MpzRef clone()
        throws CloneNotSupportedException
    {
        throw new CloneNotSupportedException();
    }

    /**
     * Overrides {@link gmp.Mpz#finalize} to prevent deallocation. 
     */
    protected void finalize() { }

    private static native void class_init();

    static { System.loadLibrary("jgmp"); class_init(); }   

    /**
     * A MpzRef represents a reference and cannot be serialized. 
     */
    private void writeObject(ObjectOutputStream out)
        throws NotSerializableException
    { throw new NotSerializableException(); }
    
    /**
     * A MpzRef represents a reference and cannot be serialized. 
     */
    private void readObject(ObjectInputStream in)
        throws NotSerializableException
     { throw new NotSerializableException(); }

    /** A MpzRef object represents a reference and cannot be serialized. */
    private void readObjectNoData() 
        throws NotSerializableException
    { throw new NotSerializableException(); }

}
