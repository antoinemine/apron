/*
 * Texpr0UnNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of unary operator nodes in {@link apron.Texpr0Node} level 0
 * expression trees.
 */
public class Texpr0UnNode 
    extends Texpr0Node
    implements Cloneable, Serializable
{

    /** Operation ({@link #OP_NEG}, etc.). */
    public int op;

    /** Rounding type ({@link #RTYPE_REAL}, etc). */
    public int rtype;

    /** Rounding direction ({@link #RDIR_NEAREST}, etc.). */
    public int rdir;

    /** Argument. */
    public Texpr0Node arg;


    // Operators
    ////////////

    /** Negation (no rounding). */
    static public final int OP_NEG = 6;

    /** Cast. */
    static public final int OP_CAST = 7;

    /** Square root. */
    static public final int OP_SQRT = 8;

    /** Returns a String representation of an operator. */
    static public String opName(int op)
    {
        switch (op) {
        case OP_NEG:    return "-";
        case OP_CAST:   return "cast";
        case OP_SQRT:   return "sqrt";
        default: throw new IllegalArgumentException("unknown unary operation: "+op);
        }
    }

    /** Returns the precedence of an operator, used for printing. */
    static public int opPrecedence(int op)
    {
        switch (op) {
        case OP_NEG:  return 3;
        case OP_CAST:
        case OP_SQRT: return 4;
        default: throw new IllegalArgumentException("unknown unary operation: "+op);
        }
    }


    // Operations
    /////////////

    /**
     * Creates a new unary operator node, with the specified operator,
     * rounding type, rounding direction, and argument.
     *
     * <p> The subtree argument is referenced, not copied.
     */
    public Texpr0UnNode(int o, int rtyp, int rdi, Texpr0Node ar)
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
    public Texpr0UnNode(int o, Texpr0Node ar)
    { 
        this(o, RTYPE_REAL, RDIR_RND, ar); 
    }


    public Texpr0Node shallowCopy()
    { 
        return new Texpr0UnNode(op, rtype, rdir, arg); 
    }

    public Texpr0Node deepCopy()
    {
        return new Texpr0UnNode(op, rtype, rdir, arg.deepCopy()); 
    }

    public Texpr1Node toTexpr1Node(Environment e)
    {
        return new Texpr1UnNode(op, rtype, rdir, arg.toTexpr1Node(e));
    }

    public String toString(Var[] names)
    { 
        String o = opName(op);
        String suf = (rtype==RTYPE_REAL) ? "" : ("_" + suffixRType(rtype) + "," +suffixRDir(rdir));
        String a = arg.toString(names);
        if (arg.getPrecedence() > opPrecedence(op)) return o + suf + a;
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

    public Texpr0Node getArgument()
    {
        return arg;
    }

    public void setArgument(Texpr0Node n)
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

    public int maxDim()
    {
        return arg.maxDim();
    }

    public boolean hasDim(int var)
    {
        return arg.hasDim(var);
    }

    protected void fillDims(boolean[] ar)
    {
        arg.fillDims(ar);
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

    public Texpr0Node substitute(int dim, Texpr0Node t)
    {
        arg = arg.substitute(dim, t);
        return this;
    }

    public void addDimensions(Dimchange c)
    {
        arg.addDimensions(c);
    }

    public Texpr0Node removeDimensions(Dimchange c)
    {
        arg = arg.removeDimensions(c);
        return this;
    }

    public void permuteDimensions(Dimperm p)
   {
       arg.permuteDimensions(p);
    }

    public int hashCode()
    {
        return arg.hashCode() ^ rtype ^ rdir ^ op;
    }

    public boolean isEqual(Texpr0Node t)
    {
        if (t instanceof Texpr0UnNode) {
            Texpr0UnNode tt = (Texpr0UnNode) t;
            return 
                op==tt.op && rtype==tt.rtype && rdir==tt.rdir &&
                arg.isEqual(tt.arg);
        }
        else return false;
    }

    public int getPrecedence()
    {
        return opPrecedence(op);
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr0UnNode) && (isEqual((Texpr0UnNode)x));
    }
}
