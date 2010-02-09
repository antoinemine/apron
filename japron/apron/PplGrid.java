/*
 * PplGrid.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Manager factory for the Parma Polyhedra Library linear
 * congruence equalities domain.
 */
public class PplGrid
    extends Manager
{

    // Internals
    ////////////

    private native void init();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }


    // Constructors
    ///////////////
    
    /**
     * Creates a new manager to create and manipulate linear
     * congruence equalities using the Parma Polyhedra Library.
     */
    public PplGrid()
    {
        init(); 
    }

}
