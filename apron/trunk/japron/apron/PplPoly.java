/*
 * PplPoly.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Manager factory for the Parma Polyhedra Library convex polyhedra domain.
 */
public class PplPoly
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
     * using the Parma Polyhedra Library.
     *
     * <p> If strict is true, then the domain can express strict
     * inequalities, i.e., non-closed convex polyhedra.
     * Otherwise, the domain expresses closed convex polyhedra.
     */
    public PplPoly(boolean strict)
    {
        init(strict); 
    }

}
