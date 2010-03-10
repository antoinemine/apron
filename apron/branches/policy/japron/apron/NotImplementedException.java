/*
 * NotImplementedException.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Exception thrown when calling an abstract transfer
 * function that is not implemented in the domain.
 */
public class NotImplementedException 
    extends ApronException
{

    public NotImplementedException()
    {
        super();
    }

    public NotImplementedException(String msg, Manager man, int fid)
    { 
        super(msg, man, fid); 
    }

}
