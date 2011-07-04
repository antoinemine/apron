/*
 * RandState.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.io.*;

/**
 * Pseudo-random number generation states.
 *
 * <p> A RandState object wraps a {@code gmp_randstate_t} GMP object, i.e., 
 * a state for pseudo-random number generation functions.
 * The state defines an algorithm and a current value state (that should be 
 * seeded).
 */
public class RandState
{
 
    // Internals
    ////////////

    /**
     * Actually, a pointer to a gmp_randstate_t object allocated in the C heap.
     */
    private long ptr;
   
    private native void init();
    private native void init_mt();
    private native void init_lc_2exp(Mpz a, int c, int m2exp);
    private native void init_lc_2exp_size(int size);
    private native void init(RandState op);

    /** Deallocates the GMP gmp_randstate_t object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("jgmp"); class_init(); }


    // Constructors
    ///////////////

    /** Creates a random state with the default algorithm. */
    public RandState() { init(); }

    /** Creates a random state for a Mersenne Twister algorithm.
     *
     * <p>The boolean argument is not actually used. It is used
     * to differentiate from the default algorithm constructor.
     */
    public RandState(boolean b) { init_mt(); }

    /** 
     * Creates a random state for a congruential algorithm.
     *
     * <p>X=(aX+c) mod 2^m2exp. 
     */
    public RandState(Mpz a, int c, int m2exp) { init_lc_2exp(a, c, m2exp); }

    /** 
     * Creates a random state for a congruential algorithm.
     *
     * <p> The multiplicand and modulo are selected from a table to ensure that
     * at least size bits of each value is used.
     * <p> The maximum size currently supported is 128.
    */
    public RandState(int size) { init_lc_2exp_size(size); }

    /** Creates a copy of the random state op. */
    public RandState(RandState op) { init(op); }


    // Seeding
    //////////

    /** Sets an initial seed value for the state. */
    public native void Seed(Mpz seed);

    /** Sets an initial seed value for the state. */
    public native void Seed(int seed);

    // Random values
    ////////////////

    /**
     * Returns an uniformly distributed random number of n bits
     * (i.e., in 0..2^n-1). 
     *
     * <p> n must be positive.
     */
    public native int randomBits(int n);

    /**
     * Returns an uniformly distributed random number in 0..n-1.
     *
     * <p> n must be positive.
     */
    public native int randomInt(int n);
}
