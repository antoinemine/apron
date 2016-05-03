/*
 * Texpr1UnNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.*;
import java.io.*;

/**
 * Class of unary operator nodes in {@link apron.Texpr1Node} level 1
 * expression trees.
 */
public class Texpr1UnNode 
    extends Texpr1Node 
    implements Cloneable, Serializable
{

    /** Operation ({@link #OP_NEG}, etc.). */
    public int op;

    /** Rounding type ({@link #RTYPE_REAL}, etc). */
    public int rtype;

    /** Rounding direction ({@link #RDIR_NEAREST}, etc.). */
    public int rdir;

    /** Argument. */
    public Texpr1Node arg;


    // Operators
    ////////////

    /** Negation (no rounding). */
    static public final int OP_NEG = 6;

    /** Cast. */
    static public final int OP_CAST = 7;

    /** Square root. */
    static public final int OP_SQRT = 8;

    // Operations
    /////////////

    /**
     * Creates a new unary operator node, with the specified operator,
     * rounding type, rounding direction, and argument.
     *
     * <p> The subtree argument is referenced, not copied.
     */
    public Texpr1UnNode(int o, int rtyp, int rdi, Texpr1Node ar)
    {
        op = o; 
        rtype = rtyp; 
        rdir = rdi; 
        arg = ar; 
    }

    /**
     * Creates a new unary operator node, with the specified operator,
     * and argument, and no rounding.
     *
     * <p> The subtree argument is referenced, not copied.
     */
    public Texpr1UnNode(int o, Texpr1Node ar)
    { 
        this(o, RTYPE_REAL, RDIR_RND, ar); 
    }

    public Texpr1Node shallowCopy()
    { 
        return new Texpr1UnNode(op, rtype, rdir, arg); 
    }

    public Texpr1Node deepCopy()
    {
        return new Texpr1UnNode(op, rtype, rdir, arg.deepCopy()); 
    }

    public Texpr0Node toTexpr0Node(Environment e)
    {
        return new Texpr0UnNode(op, rtype, rdir, arg.toTexpr0Node(e));
    }

    public String toString()
    { 
        String o = Texpr0UnNode.opName(op);
        String suf = (rtype==RTYPE_REAL) ? "" : ("_" + Texpr0Node.suffixRType(rtype) + "," + Texpr0Node.suffixRDir(rdir));
        String a = arg.toString();
        if (arg.getPrecedence() > Texpr0UnNode.opPrecedence(op)) 
            return o + suf + a;
        else return o + suf + "(" + a + ")"; 
    }

    public int getOperation()
    {
        return op;
    }

    public void setOperation(int o)
    {
        op = o;
    }

    public int getRoundingType()
    {
        return rtype;
    }
    
    public void setRoundingType(int t)
    {
        rtype = t;
    }

    public int getRoundingDirection()
    {
        return rdir;
    }

    public void setRoundingDirection(int d)
    {
        rdir = d;
    }

    public Texpr1Node getArgument()
    {
        return arg;
    }

    public void setArgument(Texpr1Node n)
    {
        arg = n;
    }

    public int getDepth()
    { 
        return arg.getDepth() + 1;
    }

    public int getSize()
    {
        return arg.getSize() + 1;
    }

    public boolean hasVar(Var var)
    {
        return arg.hasVar(var);
    }

    protected void fillVars(HashSet<Var> set)
    {
        arg.fillVars(set);
    }

    public boolean isIntervalCst()
    {
        return arg.isIntervalCst();
    }

    public boolean isIntervalLinear()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_NEG:
        case OP_CAST:
            return arg.isIntervalLinear();
        default: 
            return false;
        }
    }

    public boolean isIntervalPolynomial()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_NEG:
        case OP_CAST:
            return arg.isIntervalPolynomial();
        default: 
            return false;
        }
    }

    public boolean isIntervalPolyfrac()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_NEG:
        case OP_CAST:
            return arg.isIntervalPolyfrac();
        default: 
            return false;
        }
    }

    public boolean isScalar()
    {
        return arg.isScalar();
    }
    
    public Texpr1Node substitute(Var var, Texpr1Node t)
    {
        arg = arg.substitute(var, t);
        return this;
    }

    public int hashCode()
    {
        return arg.hashCode() ^ rtype ^ rdir ^ op;
    }

    public boolean isEqual(Texpr1Node t)
    {
        if (t instanceof Texpr1UnNode) {
            Texpr1UnNode tt = (Texpr1UnNode) t;
            return 
                op==tt.op && rtype==tt.rtype && rdir==tt.rdir &&
                arg.isEqual(tt.arg);
        }
        else return false;
    }

    public int getPrecedence()
    {
        return Texpr0UnNode.opPrecedence(op);
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr1UnNode) && (isEqual((Texpr1UnNode)x));
    }

}
