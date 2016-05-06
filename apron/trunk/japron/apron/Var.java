/*
 * Var.java
 *
 * Definition of abstract Var class for level 1 variables
 *
 * APRON Library / Java Apron binding
 *
 */
package apron;

import java.io.*;

/**
 * <p> Abstract class for Variables for level 1.
 *
 * <p> Variables need to implement Comparable to ensure
 *     a total ordering in the envirnment, and all 
 *     labels in an environment are of type Var
 */
public interface Var extends Comparable<Var>, Cloneable, Serializable
{ 
    public Var clone();
    public int compareTo(Var var);
}
