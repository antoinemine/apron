/*
 * Texpr1BinNode.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.*;
import java.io.*;

/**
 * Class of binary operator nodes in {@link apron.Texpr1Node} level 1
 * expression trees.
 */
public class Texpr1BinNode 
    extends Texpr1Node
    implements Cloneable, Serializable
{

    /** Operation ({@link #OP_ADD}, etc.). */
    public int op;

    /** Rounding type ({@link #RTYPE_REAL}, etc). */
    public int rtype;

    /** Rounding direction ({@link #RDIR_NEAREST}, etc.). */
    public int rdir;

    /** Left argument. */
    public Texpr1Node lArg;

    /** Right argument. */
    public Texpr1Node rArg;


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


    // Operations
    /////////////

    /**
     * Creates a new binary operator node, with the specified operator,
     * rounding type, rounding direction, left and right arguments.
     *
     * <p> The subtree arguments are referenced, not copied.
     */
    public Texpr1BinNode(int o, int rtyp, int rdi, Texpr1Node lAr, Texpr1Node rAr)
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
    public Texpr1BinNode(int o, Texpr1Node lAr, Texpr1Node rAr)
    { 
        this(o, RTYPE_REAL, RDIR_RND, lAr, rAr); 
    }

    public Texpr1Node shallowCopy()
    { 
        return new Texpr1BinNode(op, rtype, rdir, lArg, rArg); 
    }

    public Texpr1Node deepCopy()
    {
        return new Texpr1BinNode(op, rtype, rdir, lArg.deepCopy(), rArg.deepCopy());
    }

    public Texpr0Node toTexpr0Node(Environment e)
    {
        return new Texpr0BinNode(op, rtype, rdir, lArg.toTexpr0Node(e), rArg.toTexpr0Node(e));
    }

    public String toString()
    { 
        String o = Texpr0BinNode.opName(op);
        String suf = (rtype==RTYPE_REAL) ? "" : ("_" + Texpr0Node.suffixRType(rtype) + "," + Texpr0Node.suffixRDir(rdir));
        String l = lArg.toString();
        String r = rArg.toString();
        if (lArg.getPrecedence() <  Texpr0BinNode.opPrecedence(op))
            l = "(" + l + ")";
        if (rArg.getPrecedence() <= Texpr0BinNode.opPrecedence(op)) 
            r = "(" + r + ")";
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

    public Texpr1Node getLeftArgument()
    {
        return lArg;
    }

    public void setLeftArgument(Texpr1Node n)
    {
        lArg = n;
    }

    public Texpr1Node getRightArgument()
    {
        return rArg;
    }

    public void setRightArgument(Texpr1Node n)
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

    public boolean hasVar(Var var)
    {
        return lArg.hasVar(var) || rArg.hasVar(var);
    }

    protected void fillVars(HashSet<Var> set)
    {
        lArg.fillVars(set);
        rArg.fillVars(set);
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
    
    public Texpr1Node substitute(Var var, Texpr1Node t)
    {
        lArg = lArg.substitute(var, t);
        rArg = rArg.substitute(var, t);
        return this;
    }

    public int hashCode()
    {
        return lArg.hashCode() ^ rArg.hashCode() ^ rtype ^ rdir ^ op;
    }

    public boolean isEqual(Texpr1Node t)
    {
        if (t instanceof Texpr1BinNode) {
            Texpr1BinNode tt = (Texpr1BinNode) t;
            return 
                op==tt.op && rtype==tt.rtype && rdir==tt.rdir &&
                lArg.isEqual(tt.lArg) && rArg.isEqual(tt.rArg);
        }
        else return false;
    }

    public int getPrecedence()
    {
        return Texpr0BinNode.opPrecedence(op);
    }

    public boolean equals(Object x)
    {
        return (x instanceof Texpr1BinNode) && (isEqual((Texpr1BinNode)x));
    }
}
