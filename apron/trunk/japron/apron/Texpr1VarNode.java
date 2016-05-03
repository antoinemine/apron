/*
 * Texpr1VarNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.*;
import java.io.*;

/**
 * Class of variable leaves in {@link apron.Texpr1Node} level 1
 * expression trees.
 *
 * <p> A variable is represented as a Var name.
 */
public class Texpr1VarNode 
    extends Texpr1Node 
    implements Cloneable, Serializable
{

    /** Variable name. */
    public Var var;


    // Operations
    /////////////

    /** Constructs a new variable leaf. */
    public Texpr1VarNode(String v)
    {
        this(new StringVar(v));
    }
    public Texpr1VarNode(Var v)
    {
        var = v; 
    }

    public Texpr1Node shallowCopy()
    {
        return new Texpr1VarNode(var); 
    }

    public Texpr1Node deepCopy()
    {
        return new Texpr1VarNode(var); 
    }

    public Texpr0Node toTexpr0Node(Environment e)
    {
        return new Texpr0DimNode(e.dimOfVar(var));
    }

    public String toString()
    { 
        return var.toString();
    }

    public Var getVariable()
    {
        return var;
    }

    public void setVariable(String v) 
    {
        var = new StringVar(v);
    }

    public void setVariable(Var v)
    {
        var = v;
    }

    public int getDepth()
    { 
        return 0;
    }

    public int getSize()
    {
        return 0;
    }

    public boolean hasVar(Var v)
    {
        return var==v;
    }

    protected void fillVars(HashSet<Var> set)
    {
        set.add(var);
    }

    public boolean isIntervalCst()
    {
        return false;
    }

    public boolean isIntervalLinear()
    {
        return true;
    }

    public boolean isIntervalPolynomial()
    {
        return true;
    }

    public boolean isIntervalPolyfrac()
    {
        return true;
    }

    public boolean isScalar()
    {
        return true;
    }

    public Texpr1Node substitute(String v, Texpr1Node t)
    {
        return this.substitute(new StringVar(v), t);
    }

    public Texpr1Node substitute(Var v, Texpr1Node t)
    {
        if (var.equals(v)) return t;
        else return this;
    }

    public int hashCode()
    {
        return var.hashCode();
    }

    public boolean isEqual(Texpr1Node t)
    {
        return 
            (t instanceof Texpr1VarNode) &&
            var.equals(((Texpr1VarNode)t).var);
    }

    public int getPrecedence()
    {
        return 3;
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr1VarNode) && (isEqual((Texpr1VarNode)x));
    }
}
