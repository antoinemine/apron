/*
 * Manager.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Class of numerical abstract domain instances.
 *
 * <p> Each abstract domain subclasses the Manager class to provide a 
 * Manager factory for the domain.
 * The Manager is itself a factory for abstract elements in the 
 * corresponding abstract domain: abstract elements can be constructed
 * and manipulated by passing the Manager instance to the
 * {@link apron.Abstract0} and {@link apron.Abstract1} classes.
 *
 * <p> A Manager stores a set of abstract domain configuration parameters
 * to tune the cost/precision of the instance of the abstract domain.
 *
 * <p> Finally, a Manager stores thread-local information. It is 
 * safe to call all Apron methods in concurrent threads as long as
 * all arguments (including this, and including the Manager argument)
 * are distinct in all threads.
 *
 * <p> A Manager object encapsulates an {@code ap_manager_t} Apron object 
 * allocated in the C heap.
 */
public class Manager
{

    // Internals
    ////////////

    /**
     * Actually, a pointer to an ap_manager_t object allocated in the C  heap. 
     */
    private long ptr;

    /** Deallocates the Apron ap_manager_t object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }


    // Operation identifiers
    ////////////////////////

    static public final int FUNID_COPY = 1;
    static public final int FUNID_FREE = 2;
    static public final int FUNID_SIZE = 3;
    static public final int FUNID_MINIMIZE = 4;
    static public final int FUNID_CANONICALIZE = 5;
    static public final int FUNID_HASH = 6;
    static public final int FUNID_APPROXIMATE = 7;
    static public final int FUNID_FPRINT = 8;
    static public final int FUNID_FPRINTDIFF = 9;
    static public final int FUNID_FDUMP = 10;
    static public final int FUNID_SERIALIZE_RAW = 11;
    static public final int FUNID_DESERIALIZE_RAW = 12;
    static public final int FUNID_BOTTOM = 13;
    static public final int FUNID_TOP = 14;
    static public final int FUNID_OF_BOX = 15;
    static public final int FUNID_DIMENSION = 16;
    static public final int FUNID_IS_BOTTOM = 17;
    static public final int FUNID_IS_TOP = 18;
    static public final int FUNID_IS_LEQ = 19;
    static public final int FUNID_IS_EQ = 20;
    static public final int FUNID_IS_DIMENSION_UNCONSTRAINED = 21;
    static public final int FUNID_SAT_INTERVAL = 22;
    static public final int FUNID_SAT_LINCONS = 23;
    static public final int FUNID_SAT_TCONS = 24;
    static public final int FUNID_BOUND_DIMENSION = 25;
    static public final int FUNID_BOUND_LINEXPR = 26;
    static public final int FUNID_BOUND_TEXPR = 27;
    static public final int FUNID_TO_BOX = 28;
    static public final int FUNID_TO_LINCONS_ARRAY = 29;
    static public final int FUNID_TO_TCONS_ARRAY = 30;
    static public final int FUNID_TO_GENERATOR_ARRAY = 31;
    static public final int FUNID_MEET = 32;
    static public final int FUNID_MEET_ARRAY = 33;
    static public final int FUNID_MEET_LINCONS_ARRAY = 34;
    static public final int FUNID_MEET_TCONS_ARRAY = 35;
    static public final int FUNID_JOIN = 36;
    static public final int FUNID_JOIN_ARRAY = 37;
    static public final int FUNID_ADD_RAY_ARRAY = 38;
    static public final int FUNID_ASSIGN_LINEXPR_ARRAY = 39;
    static public final int FUNID_SUBSTITUTE_LINEXPR_ARRAY = 40;
    static public final int FUNID_ASSIGN_TEXPR_ARRAY = 41;
    static public final int FUNID_SUBSTITUTE_TEXPR_ARRAY = 42;
    static public final int FUNID_ADD_DIMENSIONS = 43;
    static public final int FUNID_REMOVE_DIMENSIONS = 44;
    static public final int FUNID_PERMUTE_DIMENSIONS = 45;
    static public final int FUNID_FORGET_ARRAY = 46;
    static public final int FUNID_EXPAND = 47;
    static public final int FUNID_FOLD = 48;
    static public final int FUNID_WIDENING = 49;
    static public final int FUNID_CLOSURE = 50;

    // Scalar kinds
    ///////////////

    static public final int SCALAR_DOUBLE = 0;
    static public final int SCALAR_MPQ = 1;
    static public final int SCALAR_MPFR = 2;


    // Operations
    /////////////

    /** 
     * Managers can only be constructed from an abstract domain
     * Manager subclass.
     */
    protected Manager()
    { }

    /**
     * Returns the name of the numerical abstract domain library that 
     * created the manager.
     */
    public native String getLibrary();

    /** 
     * Returns the version of the numerical abstract domain library that 
     * created the manager.
    */
    public native String getVersion();

    /**
     * Returns the current precision level chosen for the given operation 
     * ({@link #FUNID_COPY}, etc.). 
     */
    public native int getAlgorithm(int funid);

    /**
     * Returns the current timeout value chosen for the given operation
     * ({@link #FUNID_COPY}, etc.). 
     */
    public native int getTimeout(int funid);

    /**
     * Returns the memory size limit chosen for the given operation
     * ({@link #FUNID_COPY}, etc.). 
     */
    public native int getMaxObjectSize(int funid);

    /**
     * Whether the given operation updates the exactness flag
     * ({@link #FUNID_COPY}, etc.).. 
     */
    public native boolean getFlagExactWanted(int funid);

    /**
     * Whether the given operation updates the best-precision flag
     * ({@link #FUNID_COPY}, etc.). 
     */
    public native boolean getFlagBestWanted(int funid);

    /** 
     * Sets the current precision level for the given operation.
     * ({@link #FUNID_COPY}, etc.). 
     *
     * <p> 0 is the default. Negative numbers indicate less precision,
     * while positive numbers indicate more precision.
     * <p> The exact meaning of each value depends on the abstract domain.
     */
    public native void setAlgorithm(int funid, int precision);

    /** 
     * Sets a timeout for the given operation ({@link #FUNID_COPY}, etc.).
     * 
     * <p> Note: timeouts are currently unimplemented.
     */
    public native void setTimeout(int funid, int time);

    /**
     * Sets a size limit on the memory used by the given operation
     * ({@link #FUNID_COPY}, etc.).
     *
     * <p> By default, there is no limit.
     * <p> The unit is domain-specific but is consistent with the result
     * of {@link apron.Abstract0#getSize}.
     * <p> 0 indicates no limit.
     */
    public native void setMaxObjectSize(int funid, int size);

    /**
     * Whether the given operation ({@link #FUNID_COPY}, etc.)
     * should update the exactness flag.
     *
     * <p> As computing the exactness information may be costly, it is
     * disabled by default for all operations.
     */
    public native void setFlagExactWanted(int funid, boolean wanted);

    /**
     * Whether the given operation ({@link #FUNID_COPY}, etc.)
     * should update the best-precision flag.
     *
     * <p> As computing the best-precision information may be costly, it is
     * disabled by default for all operations.
     */
    public native void setFlagBestWanted(int funid, boolean wanted);

    /**
     * Whether the last operation with {@link #setFlagExactWanted} set returned
     * an exact result.
     */
    public native boolean wasExact();

    /**
     * Whether the last operation with {@link #setFlagBestWanted} set returned
     * a result which is optimal with respect to the abstraction.
     */
    public native boolean wasBest();

    /**
     * Gets the currently chosen preferred scalar type
     * ({@link #SCALAR_DOUBLE}, etc.).
     */
    public native int getPreferedScalarType();

    /**
     * Sets a preferred scalar type ({@link #SCALAR_DOUBLE}, etc.).
     */
   public native void setPreferedScalarType(int t);

}
