/*
 * ApronException.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Superclass of Apron-specific exceptions that can be thrown
 * by numerical abstract domains.
 *
 * <p> Apron functions can also throw standard
 * {@code java.lang.RuntimeException}, mainly: 
 * {@code java.lang.IndexOutOfBoundsException},
 * {@code java.lang.IllegalArgumentException},
 * {@code java.lang.ArithmeticException}, 
 * and {@code java.lang.NullPointerException}.
 */
public class ApronException 
    extends Exception
{

    /** Manager that caused the exception. */
    public Manager manager;
    
    /**
     * Identifier of the operation that caused the exception.
     * 
     * ({@link apron.Manager#FUNID_COPY}, etc.)
     */
    public int funid;

    public ApronException() 
    { 
        super();
    }

    public ApronException(String msg, Manager man, int fid)
    { 
        super(msg); 
        manager = man;
        funid = fid;
    }

    public int getFunctionID()
    {
        return funid;
    }

    public Manager getManager()
    {
        return manager;
    }

}
