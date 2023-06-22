/*
 * Pplite.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 * Copyright (C) Enea Zaffanella 2018-2023
 */

package apron;

import java.util.Arrays;

/**
 * Manager factory for the PPLite domains.
 */
public class Pplite
    extends Manager
{

    // Internals
    ////////////

    private native void init(boolean strict);

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }


    // Constructors
    ///////////////

    /**
     * Creates a new manager to create and manipulate convex polyhedra.
     * using the PPLite library.
     *
     * <p> If strict is true, then the domain can express strict
     * inequalities, i.e., non-closed convex polyhedra.
     * Otherwise, the domain expresses closed convex polyhedra.
     */
    public Pplite(boolean strict)
    {
        init(strict);
    }

    // Ad-hoc methods (not in Manager)
    //////////////////////////////////

    /**
     * Sets specific implementation of polyhedra domain; this should
     * be called before creating objects, since the different
     * implementations are incompatible.
     * <p>  Accepted values:
     * "Poly" (default), "F_Poly", "U_Poly", "UF_Poly", "P_Set", "FP_Set".
     */
    public native void setKind(String name);
    public native String getKind();

    /**
     * Sets the widening specification used by domains; legal values are:
     *  - "safe": the Cousot and Cousot POPL77 specification, which does not
     *     assume the inclusion between arguments;
     *  - "risky": the Cousot and Cousot PLILP92 footnote 6 alternative
     *     specification, assuming the inclusion between arguments.
     */
    public native void setWidenSpec(String name);
    public native String getWidenSpec();

    /**
     * Splits element a using (non-interval) linear inequality constraint c.
     * The return value is computed by adding the complement of c to
     * element a, whereas a itself is updated by adding constraint c.
     * If parameter integral is true, then the constraint c is
     * assumed to evaluate to an integral value and refined accordingly;
     * e.g., (x &lt; 5) is refined into (x &le; 4) and its complement is
     * computed as (x &ge; 5).
     * If parameter integral is false, a rational split is computed;
     * in this case, if strict is false then the complement of a lose
     * constraint is still lose; strict splits require a strict manager.
     */
    public native Abstract0 split(Abstract0 a, Lincons0 c,
                                  boolean integral, boolean strict);

    /** Returns true if a is a disjunctive abstract element */
    public native boolean isDisjunctive(Abstract0 a);

    /** Returns the number of disjuncts in a */
    public native int getNumDisjuncts(Abstract0 a);

    /** Returns a set of linear constraints for the n-tn disjunct of a */
    public native Lincons0[] toDisjunctLincons(Abstract0 a, int n)
        throws ApronException;

    /** Returns a set of constraints for the n-tn disjunct of a */
    public native Tcons0[] toDisjunctTcons(Abstract0 a, int n)
        throws ApronException;

    /** Modifies a to have at most n disjuncts. */
    public native void collapse(Abstract0 a, int n)
        throws ApronException;

    /** Geometric inclusion test */
    public native boolean isGeomSubsetEq(Abstract0 a1, Abstract0 a2)
        throws ApronException;

}
