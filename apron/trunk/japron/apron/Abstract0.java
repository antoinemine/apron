/*
 * Abstract0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Class of level 0 numerical abstract values.
 *
 * <p> A level 0 numerical abstract value object represents a set of points 
 * in Z^n * R^m, for some n and m.
 *
 * <p> All abstract element methods are applied within a given 
 * {@link apron.Manager}
 * that indicates which numerical abstract domain instance is used.
 * Most often, which Manager is used is specified as an argument.
 * It must be a Manager compatible (i.e., from the same factory, and 
 * constructed using the same parameters) with the one the Abstract0 arguments 
 * were created with.
 * A few functions (such as finalize, toString) cannot take a Manager argument.
 * They are applied implicitly with the Manager used when creating the 
 * object.
 *
 * <p> An Abstract0 object encapsulates an {@code ap_abstract0_t} Apron object 
 * allocated in the C heap.
 */
public class Abstract0
    implements Cloneable
{

    // Internals
    ////////////

    /**
     * Actually, a pointer to an ap_abstract0_t object allocated in the C 
     * heap. 
    */
    private long ptr;
 
    private native void init(Manager man, Abstract0 o) throws ApronException;
    private native void init(Manager man, int intdim, int realdim, boolean empty) throws ApronException;
    private native void init(Manager man, int intdim, int realdim, Interval[] box) throws ApronException;

    /** Deallocates the Apron ap_abstract0_t object. */
    protected native void finalize();

    private static native void class_init();

    static { System.loadLibrary("japron"); class_init(); }

    private Abstract0() { }


    // Constructors
    ///////////////

    /**
     * Creates a copy of the abstract element a. 
     */
    public Abstract0(Manager man, Abstract0 a)
        throws ApronException
    { init(man, a); }

    /**
     * Creates a new universal abstract element.
     *
     * <p> The element has intdim integer-valued dimensions, followed by
     * realdim real-valued dimensions.
     * <p> It is initialized to the full space: Z^intdim * R^realdim.
     */
    public Abstract0(Manager man, int intdim, int realdim)
        throws ApronException
    { init(man, intdim, realdim, false); }

    /**
     * Creates a new universal or empty abstract element.
     *
     * <p> The element has intdim integer-valued dimensions, followed by
     * realdim real-valued dimensions.
     * <p> It is initialized to the full space if empty is false, and to
     * the empty space if empty is true.
     */
    public Abstract0(Manager man, int intdim, int realdim, boolean empty)
        throws ApronException
    { init(man, intdim, realdim, empty); }
    
    /**
     * Creates a new abstract element from a box.
     *
     * <p> The element has intdim integer-valued dimensions, followed by
     * realdim real-valued dimensions.
     * <p> box must contain intdim+realdim intervals.
     * <p> The constructed element is the Cartesian product of the intervals.
     */
    public Abstract0(Manager man, int intdim, int realdim, Interval[] box)
        throws ApronException
    { init(man, intdim, realdim, box); }


   /**
     * <p> Creates a new abstract element from a set of constraints.
     *
     * <p> The element has intdim integer-valued dimensions, followed by
     * realdim real-valued dimensions.
     * <p> The constructed element is (an over-approximation of) the set
     * of points satisfying all constraints.
     */
    public Abstract0(Manager man, int intdim, int realdim, Lincons0[] c)
        throws ApronException
    { init(man, intdim, realdim, false); meet(man, c); }

   /**
     * Creates a new abstract element from a set of constraints.
     *
     * <p> The element has intdim integer-valued dimensions, followed by
     * realdim real-valued dimensions.
     * <p> The constructed element is (an over-approximation of) the set
     * of points satisfying all constraints.
     */
    public Abstract0(Manager man, int intdim, int realdim, Tcons0[] c)
        throws ApronException
    { init(man, intdim, realdim, false); meet(man, c); }


    // Access
    /////////

    /**
     * Returns the size in memory of the abstract element.
     *
     * <p> The memory unit is domain-specific.
     */
    public native int getSize(Manager man)
        throws ApronException;

    /**
     * Returns a hash of the element value.
     */
    public native int hashCode(Manager man)
        throws ApronException;

    /**
     * Returns the number of integer-valued and real-valued dimensions
     * in the element.
     */
    public native Dimension getDimension(Manager man)
        throws ApronException;

    /**
     * Returns the Manager which was used to create the
     * the abstract element.
     */
    public native Manager getCreationManager();

    /**
     * Returns a String representation of the element as a list of
     * constraints.
     *
     * <p> This method uses the Manager that was used to create this.
     */
    public String toString() 
    {
        return toString(getCreationManager());
    }
    
    /**
     * Returns a String representation of the element as a list of
     * constraints.
     *
     * <p> Dimension i is denoted as xi.
     */
    public String toString(Manager man) 
    {
        return toString(man, (Var[])null);
    }

    public String toString(Manager man, String[] names)
    {
        return toString(man, StringVar.arrayOfStrings(names));
    }

    /**
     * Returns a String representation of the element as a list of
     * constraints.
     *
     * <p> Dimension i as denoted by names[i].
     */
    public String toString(Manager man, Var[] names) 
    {
        try {
            if (isBottom(man)) return "<empty>";
            if (isTop(man)) return "<universal>";
            StringBuffer buf = new StringBuffer();
            //Tcons0[] l = toTcons(man);
            Lincons0[] l = toLincons(man);
            boolean first = true;
            buf.append("{");
            for (int i=0; i<l.length; i++) {
                if (first) buf.append(" "); else buf.append("; ");
                buf.append(l[i].toString(names));
                first = false;
            }
            buf.append(" }");
            return buf.toString();
        }
        catch (ApronException e) {
            return "<caught exception: " + e.getMessage() + ">";
        }
    }


    /**
     * Returns (an over-approximation of) the set of values e can 
     * take on this.
     */
    public native Interval getBound(Manager man, Linexpr0 e)
        throws ApronException;

    /**
     * Returns (an over-approximation of) the set of values e can 
     * take on this.
     */
    public native Interval getBound(Manager man, Texpr0Intern e)
        throws ApronException;
  
    /**
     * Returns (an over-approximation of) the set of values the
     * variable dim can take on this.
     */
    public native Interval getBound(Manager man, int dim)
        throws ApronException;


    /**
     * Returns a set of linear constraints describing (an over-approximation of)
     * this.
     */
    public native Lincons0[] toLincons(Manager man)
        throws ApronException;

    /**
     * Returns a set of constraints describing (an over-approximation of)
     * this.
     */
    public native Tcons0[] toTcons(Manager man)
        throws ApronException;

    /**
     * Returns a box describing (an over-approximation of) this.
     */
    public native Interval[] toBox(Manager man)
        throws ApronException;

    /**
     * Returns a set of generators describing (an over-approximation of)
     * this.
     */
    public native Generator0[] toGenerator(Manager man)
        throws ApronException;



    // Tests
    ////////

    /**
     * Whether the element is definitely empty (true) or maybe not empty
     * (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the element is definitely not empty.
     */
    public native boolean isBottom(Manager man)
        throws ApronException;

    /**
     * Whether the element is definitely universal (true) or maybe not 
     * universal (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the element is definitely not universal.
     */
    public native boolean isTop(Manager man)
        throws ApronException;

    /**
     * Whether this is definitely included in x (true) or maybe not 
     * included (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this is definitely not included in x.
     */
    public native boolean isIncluded(Manager man, Abstract0 x)
        throws ApronException;

    /**
     * Whether this is definitely equal to x (true) or maybe not 
     * equal (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this is definitely not equal to x.
     */
    public native boolean isEqual(Manager man, Abstract0 x)
        throws ApronException;

    /**
     * Whether this definitely satisfies the constraint c (true) 
     * or maybe does not satisfy it (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this definitely does not satisfy c.
     */
    public native boolean satisfy(Manager man, Lincons0 c)
        throws ApronException;

    /**
     * Whether this definitely satisfies the constraint c (true) 
     * or maybe does not satisfy it (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that this definitely does not satisfy c.
     */
    public native boolean satisfy(Manager man, Tcons0 c)
        throws ApronException;

    /**
     * Whether the projection of this on dimension dim is definitely
     * included in the interval i (true) or maybe is not included
     * (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that it is definitely not included.
     */
    public native boolean satisfy(Manager man, int dim, Interval i)
        throws ApronException;

    /**
     * Whether the dimension dim is definitely unbounded in this (true)
     * or may be bounded (false).
     *
     * <p> If {@link apron.Manager#wasExact} returns true, 
     * then false indicates that the dimension is definitely bounded.
     */
    public native boolean isDimensionUnconstrained(Manager man, int dim)
        throws ApronException;



    // Operations
    /////////////


    /**
     * Tries to minimize the memory representation of the element.
     *
     * <p> Use this to gain memory space. However, this may cause subsequent
     * operations to be less efficients as cached information may be
     * discarded.
     */
    public native void minimize(Manager man)
        throws ApronException;

    /**
     * Tries to put the element in canonical form.
     *
     * <p> The notion of canonical form is not fully defined yet.
     */
    public native void canonicalize(Manager man)
        throws ApronException;

    /**
     * Tries to simplify the representation of the element, maybe at
     * the expense of precision.
     *
     * <p> The algorithm argument indicates how aggressive the simplification 
     * should be. Its interpretation is domain-dependant.
     */
    public native void approximate(Manager man, int algorithm)
        throws ApronException;


    // Meet and join
    /////////////////

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this and op.
     *
     * <p> this and op are not modified.
     */
    public native Abstract0 meetCopy(Manager man, Abstract0 op)
        throws ApronException;
 
    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of the elements in ar.
     *
     * <p> The elements of ar are not modified.
     */
    static public native Abstract0 meet(Manager man, Abstract0[] ar)
        throws ApronException;
 
    /**
     * Replaces this with (an over-approximation of) the set-intersection of
     * this and op.
     *
     * <p> op is not modified.
     */
    public native void meet(Manager man, Abstract0 op)
        throws ApronException;
    

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-union of this and op.
     *
     * <p> this and op are not modified.
     */
    public native Abstract0 joinCopy(Manager man, Abstract0 op)
        throws ApronException;
 
    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-union of the elements in ar.
     *
     * The elements of ar are not modified.
     */
    static public native Abstract0 join(Manager man, Abstract0[] ar)
        throws ApronException;
 
    /**
     * Replaces this with (an over-approximation of) the set-union of
     * this and op.
     *
     * <p> op is not modified.
     */
    public native void join(Manager man, Abstract0 op)
        throws ApronException;
    

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraints in ar.
     *
     * <p> this is not changed.
     */
    public native Abstract0 meetCopy(Manager man, Lincons0[] ar)
        throws ApronException;


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraints in ar.
     *
     * <p> this is not changed.
     */
    public native Abstract0 meetCopy(Manager man, Tcons0[] ar)
        throws ApronException;

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraints in ar.
     */
    public native void meet(Manager man, Lincons0[] ar)
        throws ApronException;

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraints in ar.
     */
    public native void meet(Manager man, Tcons0[] ar)
        throws ApronException;



    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraint cr.
     *
     * <p> this is not changed.
     */
    public Abstract0 meetCopy(Manager man, Lincons0 c)
        throws ApronException
    { Lincons0[] ar = { c }; return meetCopy(man, ar); }


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * the set-intersection of this with the constraint in c.
     *
     * <p> this is not changed.
     */
    public Abstract0 meetCopy(Manager man, Tcons0 c)
        throws ApronException
    { Tcons0[] ar = { c }; return meetCopy(man, ar); }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraint c.
     */
    public void meet(Manager man, Lincons0 c)
        throws ApronException
    { Lincons0[] ar = { c }; meet(man, ar); }

    /**
     * Replaces this with (an over-approximation of) the set-intersection 
     * of this with the constraint c.
     */
    public void meet(Manager man, Tcons0 c)
        throws ApronException
    { Tcons0[] ar = { c }; meet(man, ar); }


    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * this with some rays added.
     *
     * <p> this is not changed.
     * <p> ar can only contain ray and lines, not vertices.
     */
    public native Abstract0 addRayCopy(Manager man, Generator0[] ar)
        throws ApronException;

   /**
     * Replaces this with (an over-approximation of) this with some rays 
     * added.
     *
     * <p> ar can only contain ray and lines, not vertices.
     */
    public native void addRay(Manager man, Generator0[] ar)
        throws ApronException;

    /**
     * Returns a new abstract element that contains (an over-approximation of)
     * this with one ray added.
     *
     * <p> this is not changed.
     */
    public Abstract0 addRayCopy(Manager man, Generator0 r)
        throws ApronException
    { Generator0[] ar = { r }; return addRayCopy(man, ar); }

   /**
     * Replaces this with (an over-approximation of) this with one ray
     * added.
     */
    public void addRay(Manager man, Generator0 r)
        throws ApronException
    { Generator0[] ar = { r }; addRay(man, ar); }



    // Assignment and substitution
    //////////////////////////////


    /**
     * Replaces this with (an over-approximation of) this after
     * assigning ar[i] to  dim[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native void assign(Manager man, int[] dim, Linexpr0[] ar, Abstract0 dest)
        throws ApronException;
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning ar[i] to dim[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native Abstract0 assignCopy(Manager man, int[] dim, Linexpr0[] ar, Abstract0 dest)
        throws ApronException;

    /**
     * Replaces this with (an over-approximation of) this after
     * assigning ar[i] to dim[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native void assign(Manager man, int[] dim, Texpr0Intern[] ar, Abstract0 dest)
        throws ApronException;
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning ar[i] to dim[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native Abstract0 assignCopy(Manager man, int[] dim, Texpr0Intern[] ar, Abstract0 dest)
        throws ApronException;


    /**
     * Replaces this with (an over-approximation of) this after
     * substituting dim[i] with ar[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native void substitute(Manager man, int[] dim, Linexpr0[] ar, Abstract0 dest)
        throws ApronException;
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting dim[i] with ar[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native Abstract0 substituteCopy(Manager man, int[] dim, Linexpr0[] ar, Abstract0 dest)
        throws ApronException;

    /**
     * Replaces this with (an over-approximation of) this after
     * substituting dim[i] with ar[i], in parallel.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native void substitute(Manager man, int[] dim, Texpr0Intern[] ar, Abstract0 dest)
        throws ApronException;
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting dim[i] with ar[i], in parallel.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public native Abstract0 substituteCopy(Manager man, int[] dim, Texpr0Intern[] ar, Abstract0 dest)
        throws ApronException;



    /**
     * Replaces this with (an over-approximation of) this after
     * assigning expr to dim.
     *
     * The result is intersected with dest, if dest is not null.
     */
    public void assign(Manager man, int dim, Linexpr0 expr, Abstract0 dest)
        throws ApronException
    { Linexpr0[] ar = { expr }; int[] d = { dim }; assign(man, d, ar, dest); }
    
    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning expr to dim.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public Abstract0 assignCopy(Manager man, int dim, Linexpr0 expr, Abstract0 dest)
        throws ApronException
    { Linexpr0[] ar = { expr }; int[] d = { dim }; return assignCopy(man, d, ar, dest); }

    /**
     * Replaces this with (an over-approximation of) this after
     * assigning expr to dim.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public void assign(Manager man, int dim, Texpr0Intern expr, Abstract0 dest)
        throws ApronException
    { Texpr0Intern[] ar = { expr }; int[] d = { dim }; assign(man, d, ar, dest); }

    /**
     * Returns a new element containing (an over-approximation of) this after
     * assigning expr to dim.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public Abstract0 assignCopy(Manager man, int dim, Texpr0Intern expr, Abstract0 dest)
        throws ApronException
    { Texpr0Intern[] ar = { expr }; int[] d = { dim }; return assignCopy(man, d, ar, dest); }


    /**
     * Replaces this with (an over-approximation of) this after
     * substituting dim with expr.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public void substitute(Manager man, int dim, Linexpr0 expr, Abstract0 dest)
        throws ApronException
    { Linexpr0[] ar = { expr }; int[] d = { dim }; substitute(man, d, ar, dest); }

    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting dim with expr.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public Abstract0 substituteCopy(Manager man, int dim, Linexpr0 expr, Abstract0 dest)
        throws ApronException
    { Linexpr0[] ar = { expr }; int[] d = { dim }; return substituteCopy(man, d, ar, dest); }

    /**
     * Replaces this with (an over-approximation of) this after
     * substituting dim with expr.
     *
     * <p> The result is intersected with dest, if dest is not null.
     */
    public void substitute(Manager man, int dim, Texpr0Intern expr, Abstract0 dest)
        throws ApronException
    { Texpr0Intern[] ar = { expr }; int[] d = { dim }; substitute(man, d, ar, dest); }

    /**
     * Returns a new element containing (an over-approximation of) this after
     * substituting dim with expr.
     *
     * <p> this is not modified.
     * <p> The result is intersected with dest, if dest is not null.
     */
    public Abstract0 substituteCopy(Manager man, int dim, Texpr0Intern expr, Abstract0 dest)
        throws ApronException
    { Texpr0Intern[] ar = { expr }; int[] d = { dim }; return substituteCopy(man, d, ar, dest); }


    // Projection
    /////////////

    /**
     * Returns a new element containing (an over-approximation of) this after
     * forgetting the value of the variables in dim.
     *
     * <p> this is not modified.
     * <p> If project is true, the variables are set to 0. 
     * Otherwise, they are set to [-oo,+oo].
     */
    public native Abstract0 forgetCopy(Manager man, int[] dim, boolean project)
        throws ApronException;

    /**
     * Replaces this with (an over-approximation of) this after
     * forgetting the value of the variables in dim.
     *
     * <p> If project is true, the variables are set to 0. 
     * Otherwise, they are set to [-oo,+oo].
     */
    public native void forget(Manager man, int[] dim, boolean project)
        throws ApronException;

    /**
     * Returns a new element containing (an over-approximation of) this after
     * forgetting the value of the variable dim.
     *
     * <p> this is not modified.
     */
    public  Abstract0 forgetCopy(Manager man, int dim, boolean project)
        throws ApronException
    { int[] d = { dim }; return forgetCopy(man, d, project); }

    /**
     * Replaces this with (an over-approximation of) this after
     * forgetting the value of the variable  dim.
     */
    public void forget(Manager man, int dim, boolean project)
        throws ApronException
    { int[] d = { dim }; forget(man, d, project); }


    // Change and permutation of dimensions
    ///////////////////////////////////////

    /**
     * Returns a new element containing this after adding some dimensions.
     *
     * <p> this is not modified.
     * <p> If project is true, the added variables are set to 0. 
     * Otherwise, they are set to set to [-oo,+oo].
     */
    public native Abstract0 addDimensionsCopy(Manager man, Dimchange d, boolean project)
        throws ApronException;

    /**
     * Adds some dimensions to this.
     *
     * <p> If project is true, the added variables are set to 0. 
     * Otherwise, they are set to set to [-oo,+oo].
     */
    public native void addDimensions(Manager man, Dimchange d, boolean project)
        throws ApronException;

   /**
     * Returns a new element containing this after removing some dimensions.
     *
     * <p> this is not modified.
     */
    public native Abstract0 removeDimensionsCopy(Manager man, Dimchange d)
        throws ApronException;

    /**
     * Removes some dimensions from this.
     */
    public native void removeDimensions(Manager man, Dimchange d)
        throws ApronException;

   /**
     * Returns a new element containing this after permuting some dimensions.
     *
     * <p> this is not modified.
     */
    public native Abstract0 permuteDimensionsCopy(Manager man, Dimperm p)
        throws ApronException;

    /**
     * Permutes some dimensions to this.
     */
    public native void permuteDimensions(Manager man, Dimperm p)
        throws ApronException;


    // Expansion and folding
    ////////////////////////
    
    /**
     * Returns a new element where variable dim is duplicated n times.
     *
     * <p> If the duplicated dimension is integer (resp. float), copies are 
     * appended after all integer (resp. float) variables.
     * <p> this is not modified.
     */
    public native Abstract0 expandCopy(Manager man, int dim, int n)
        throws ApronException;
    
    /**
     * Duplicates n times the variable dim in this.
     *
     * <p> If the duplicated dimension is integer (resp. float), copies are 
     * appended after all integer (resp. float) variables.
     */
    public native void expand(Manager man, int dim, int n)
        throws ApronException;


    /**
     * Returns a new element where all dimensions from dim[]
     * are folded into dimension dim[0].
     *
     * <p> The array must be sorted and contain either only integer-valued
     * or real-valued variables.
     * <p> this is not modified.
     */
    public native Abstract0 foldCopy(Manager man, int[] dim)
        throws ApronException;
    
    /**
     * Folds all dimensions from dim[] into dimension dim[0].
     *
     * <p> The array must be sorted and contain either only integer-valued
     * or real-valued variables.
     * <p> The array must be sorted.
     */
    public native void fold(Manager man, int[] dim)
        throws ApronException;


    // Widening
    ///////////

    
     /**
     * Returns a new element equal to this widened with op.
     *
     * <p> this and op are not modified.
     */
    public native Abstract0 widening(Manager man, Abstract0 op)
        throws ApronException;
    
   /**
     * Returns a new element equal to this widened with op,
     * with thresholds constraints set c.
     *
     * <p> this and op are not modified.
     */
    public native Abstract0 wideningThreshold(Manager man, Abstract0 op,
                                              Lincons0[] c)
        throws ApronException;
    

    // Closure
    //////////

    /**
     * Returns a new element which is (an over-approximation of)
     * the topological closure of this.
     *
     * <p> this is not modified.
     */
    public native Abstract0 closureCopy(Manager man)
        throws ApronException;
    
    /**
     * Replaces this with (an over-approximation of) its topological
     * closure.
     */
    public native void closure(Manager man)
        throws ApronException;



    // Serialization
    ////////////////

    /**
     * Converts the abstract element to a byte array.
     *
     * <p> The byte array can be stored to and retrieved from disk, or
     * sent through a network. However, the sending and receiving parties
     * must use compatible managers.
     */
    public native byte[] serialize(Manager man)
        throws ApronException;

    /**
     * Restores an abstract element from a byte array.
     *
     * <p> The specified manager must be compatible with that used during
     * the {@link #serialize} call.
     */
    static public native Abstract0 deserialize(Manager man, byte[] buf)
        throws ApronException;

}
