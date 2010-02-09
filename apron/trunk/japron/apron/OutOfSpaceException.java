/*
 * OutOfSpaceException.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Exception thrown by an abstract transfer function when
 * the maximum memory consumption is exceeded.
 *
 * <p> The maximum memory consumption is set by 
 * {@link apron.Manager#setMaxObjectSize}, on a
 * per-function, per-manager basis. There is no limit by default.
 * The unit of memory consumption is unspecified (it may not be bytes).
 *
 * <p> This exception is not a (fatal) {@code java.lang.OutOfMemoryError}, 
 * but merely exceeding a self-imposed limit. The function can be called again
 * after raising the limit.
 */
public class OutOfSpaceException
    extends ApronException
{

    public OutOfSpaceException()
    {
        super();
    }

    public OutOfSpaceException(String msg, Manager man, int fid)
    { 
        super(msg, man, fid); 
    }

}
