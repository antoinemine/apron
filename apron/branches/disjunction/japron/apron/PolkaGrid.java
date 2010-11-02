/*
 * PolkaGrid.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Manager factory for the reduced product of the {@link apron.Polka} 
 * convex polyhedra and {@link apron.PplGrid} the linear congruence
 * equalities domains.
 */
public class PolkaGrid
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
     * Creates a new manager to create and manipulate reduced product of
     * {@link apron.Polka} and {@link apron.PplGrid} linear congruence
     * equalities.
     *
     * <p> If strict is true, then the domain can express strict
     * linear inequalities.
     */
    public PolkaGrid(boolean strict)
    {
        init(strict); 
    }

}
