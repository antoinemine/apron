/*
 * TimeoutException.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

/**
 * Exception thrown by an abstract transfer function when
 * the timeout is exceeded.
 *
 * <p> The timeout is set by {@link apron.Manager#setTimeout}, on a
 * per-function, per-manager basis. There is no timeout by default.
 *
 * <p> Note: timeouts are not currently implemented.
 */
public class TimeoutException
    extends ApronException
{
    public TimeoutException()
    {
        super();
    }

    public TimeoutException(String msg, Manager man, int fid)
    { 
        super(msg, man, fid); 
    }

}
