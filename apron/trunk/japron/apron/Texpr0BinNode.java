/*
 * Texpr0BinNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.io.*;

/**
 * Class of binary operator nodes in {@link apron.Texpr0Node} level 0
 * expression trees.
 */
public class Texpr0BinNode 
    extends Texpr0Node
    implements Cloneable, Serializable
{

    /** Operation ({@link #OP_ADD}, etc.). */
    public int op;

    /** Rounding type ({@link #RTYPE_REAL}, etc). */
    public int rtype;

    /** Rounding direction ({@link #RDIR_NEAREST}, etc.). */
    public int rdir;

    /** Left argument. */
    public Texpr0Node lArg;

    /** Right argument. */
    public Texpr0Node rArg;


    // Operators
    ////////////

    /** Addition. */
    static public final int OP_ADD = 0;

    /** Subtraction. */
    static public final int OP_SUB = 1;

    /** Multiplication. */
    static public final int OP_MUL = 2;

    /** Division. */
    static public final int OP_DIV = 3;

    /** Modulo (for integers or reals). */
    static public final int OP_MOD = 4;

    /** Power. */
    static public final int OP_POW = 5;

   /** Returns a String representation of an operator. */
    static public String opName(int op)
    {
        switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_POW: return "^";
       default: throw new IllegalArgumentException("unknown binary operation: "+op);
        }
    }

    /** Returns the precedence of an operator, used for printing. */
    static public int opPrecedence(int op)
    {
        switch (op) {
        case OP_ADD: 
        case OP_SUB: return 1;
        case OP_MUL: 
        case OP_DIV: 
        case OP_MOD:
        case OP_POW: return 2;
        default: throw new IllegalArgumentException("unknown binary operation: "+op);
        }
    }



    // Operations
    /////////////

    /**
     * Creates a new binary operator node, with the specified operator,
     * rounding type, rounding direction, left and right arguments.
     *
     * <p> The subtree arguments are referenced, not copied.
     */
    public Texpr0BinNode(int o, int rtyp, int rdi, Texpr0Node lAr, Texpr0Node rAr)
    {
        op = o; 
        rtype = rtyp;
        rdir = rdi; 
        lArg = lAr; 
        rArg = rAr; 
    }

    /**
     * Creates a new binary operator node, with the specified operator,
     * and left and right arguments, and no rounding.
     *
     * <p> The subtree arguments are referenced, not copied.
     */
    public Texpr0BinNode(int o, Texpr0Node lAr, Texpr0Node rAr)
    {
        this(o, RTYPE_REAL, RDIR_RND, lAr, rAr); 
    }

    public Texpr0Node shallowCopy()
    {
        return new Texpr0BinNode(op, rtype, rdir, lArg, rArg); 
    }

    public Texpr0Node deepCopy()
    {
        return new Texpr0BinNode(op, rtype, rdir, lArg.deepCopy(), rArg.deepCopy()); 
    }

    public Texpr1Node toTexpr1Node(Environment e)
    {
        return new Texpr1BinNode(op, rtype, rdir, lArg.toTexpr1Node(e), rArg.toTexpr1Node(e));
    }

    public String toString(Var[] names)
    { 
        String o = opName(op);
        String suf = (rtype==RTYPE_REAL) ? "" : ("_" + suffixRType(rtype) + "," +suffixRDir(rdir));
        String l = lArg.toString(names);
        String r = rArg.toString(names);
        if (lArg.getPrecedence() <  opPrecedence(op)) l = "(" + l + ")";
        if (rArg.getPrecedence() <= opPrecedence(op)) r = "(" + r + ")";
        return l + " " + o + suf + " " + r;
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

    public Texpr0Node getLeftArgument()
    {
        return lArg;
    }

    public void setLeftArgument(Texpr0Node n)
    {
        lArg = n;
    }

    public Texpr0Node getRightArgument()
    {
        return rArg;
    }

    public void setRightArgument(Texpr0Node n)
    {
        rArg = n;
    }

    public int getDepth()
    { 
        int m1 = lArg.getDepth();
        int m2 = rArg.getDepth();
        return ((m1>m2) ? m1 : m2) + 1;
    }

    public int getSize()
    {
        return lArg.getSize() + rArg.getSize() + 1;
    }

    public int maxDim()
    {
        int m1 = lArg.maxDim();
        int m2 = rArg.maxDim();
        return (m1>m2) ? m1 : m2;
    }

    public boolean hasDim(int var)
    {
        return lArg.hasDim(var) || rArg.hasDim(var);
    }

    protected void fillDims(boolean[] ar)
    {
        lArg.fillDims(ar);
        rArg.fillDims(ar);
    }

    public boolean isIntervalCst()
    {
        return lArg.isIntervalCst() && rArg.isIntervalCst();
    }

    public boolean isIntervalLinear()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_ADD:
        case OP_SUB:
            return lArg.isIntervalLinear() && rArg.isIntervalLinear();
        case OP_MUL:
            return (lArg.isIntervalLinear() && rArg.isIntervalCst()) ||
                   (rArg.isIntervalLinear() && lArg.isIntervalCst());
        case OP_DIV:
            return lArg.isIntervalLinear() && rArg.isIntervalCst();
        default: 
            return false;
        }
    }

    public boolean isIntervalPolynomial()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
            return lArg.isIntervalPolynomial() && rArg.isIntervalPolynomial();
        case OP_DIV:
            return lArg.isIntervalPolynomial() && rArg.isIntervalCst();
        case OP_POW:
            return lArg.isIntervalPolynomial() && rArg.isIntervalCst();
        default: 
            return false;
        }
    }

    public boolean isIntervalPolyfrac()
    {
        if (rtype!=RTYPE_REAL) return false;
        switch (op) {
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            return lArg.isIntervalPolyfrac() && rArg.isIntervalPolyfrac();
        case OP_POW:
            return lArg.isIntervalPolyfrac() && rArg.isIntervalCst();
       default: 
            return false;
        }
    }

    public boolean isScalar()
    {
        return lArg.isScalar() && rArg.isScalar();
    }

    public Texpr0Node substitute(int dim, Texpr0Node t)
    {
        lArg = lArg.substitute(dim, t);
        rArg = rArg.substitute(dim, t);
        return this;
    }

    public void addDimensions(Dimchange c)
    {
        lArg.addDimensions(c);
        rArg.addDimensions(c);
    }

    public Texpr0Node removeDimensions(Dimchange c)
    {
        lArg = lArg.removeDimensions(c);
        rArg = rArg.removeDimensions(c);
        return this;
    }

    public void permuteDimensions(Dimperm p)
    {
        lArg.permuteDimensions(p);
        rArg.permuteDimensions(p);
    }

    public int hashCode()
    {
        return lArg.hashCode() ^ rArg.hashCode() ^ rtype ^ rdir ^ op;
    }

    public boolean isEqual(Texpr0Node t)
    {
        if (t instanceof Texpr0BinNode) {
            Texpr0BinNode tt = (Texpr0BinNode) t;
            return 
                op==tt.op && rtype==tt.rtype && rdir==tt.rdir &&
                lArg.isEqual(tt.lArg) && rArg.isEqual(tt.rArg);
        }
        else return false;
    }

    public int getPrecedence()
    {
        return opPrecedence(op);
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr0BinNode) && (isEqual((Texpr0BinNode)x));
    }
}
