/*
 * Texpr1Node.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.util.*;
import java.io.*;

/**
 * Class of level 1 concrete expression trees.
 *
 * <p> A Texpr1Node represents an expression tree, which can be
 * constructed and inspected in Java.
 * Each node is actually a binary operator ({@link apron.Texpr1BinNode}), 
 * a unary operator ({@link apron.Texpr1UnNode}), a variable represented
 * by name ({@link apron.Texpr1VarNode}), or a constant 
 * ({@link apron.Texpr1CstNode}).
 *
 * <p> An expression tree must be converted to and from a
 * {@link apron.Texpr1Intern} to be used in abstract domain transfer 
 * functions.
 * Unlike other level 1 objects, level 1 concrete expression trees are not
 * defined with respect to a given environment. The environment
 * is specified during the conversion to an opaque level 1 expression tree.
 */
public abstract class Texpr1Node 
    implements Cloneable, Serializable
{

    /** Shallow copy. */
    public abstract Texpr1Node shallowCopy();

    /** Deep copy. */
    public abstract Texpr1Node deepCopy();

    /** No constructor: use constructors from a sub-class. */
    protected Texpr1Node() { }


    /**
     * Constructs an expression tree from a linear expression.
     *
     * <p> All coefficients are copied.
     */
    static public Texpr1Node fromLinexpr1(Linexpr1 c)
    {
        Texpr1Node r = null;
        Coeff cst = c.getCst();
        if (!cst.isZero()) r = new Texpr1CstNode(cst.copy());
        Linterm1[] l = c.getLinterms();
        for (int i=0; i<l.length; i++) {
            if (l[i].coeff.isZero()) continue;
            Texpr1Node rr = 
                new Texpr1BinNode(Texpr1BinNode.OP_MUL,
                                  new Texpr1CstNode(l[i].coeff.copy()),
                                  new Texpr1VarNode(l[i].var));
            r = (r==null) ? rr :
                new Texpr1BinNode(Texpr0BinNode.OP_ADD, r, rr);
        }
        if (r==null) return new Texpr1CstNode();
        else return r;
    }

    /**
     * Converts the level 1 tree to a level 0 tree in the given
     * environment. 
     */
    public abstract Texpr0Node toTexpr0Node(Environment e);

    /** Converts the expression tree to a String. */
    public abstract String toString();

    /** Returns the depth of the expression tree. */
    public abstract int getDepth();

    /** Returns the number of internal nodes in the tree. */
    public abstract int getSize();

    /** Whether the variable var occurs in the tree. */
    public abstract boolean hasVar(Var var);

    public boolean hasVar(String var)
    {
        return hasVar(new StringVar(var));
    }

    /** Used internally for getVars. */
    protected abstract void fillVars(HashSet<Var> set);

    /**
     * Returns the list of all occurring variables. 
     *
     * <p> The ordering is unspecified.
     **/
    public Var[] getVars()
    {
        HashSet<Var> set = new HashSet<Var>();
        fillVars(set);
        Var[] s = new Var[set.size()];
        Iterator<Var> i = set.iterator();
        for (int nb=0; i.hasNext(); nb++)
            s[nb] = i.next();
        return s;
    }

    /** Whether the expression is constant. */
    public abstract boolean isIntervalCst();

    /** 
     * Whether the expression is linear with interval or scalar coefficients, 
     * and there is no rounding. 
     */
    public abstract boolean isIntervalLinear();

    /**
     * Whether the expression is polynomial, with interval or scalar 
     * coefficients, and there is no rounding. 
     */
    public abstract boolean isIntervalPolynomial();

    /** 
     * Whether the expression is a polynomial fraction, with interval or 
     * scalar coefficients, and there is no rounding. 
     */
    public abstract boolean isIntervalPolyfrac();

    /** Whether all coefficients are scalar (non interval). */
    public abstract boolean isScalar();
   
    /** 
     * Substitutes all occurrences of the variable var with the tree t.
     *
     * <p> this is modified in-place.
     * <p> As some nodes can change their kind (a node for a variable var
     * becomes the root of the tree t), a new root node is returned.
     */
    public abstract Texpr1Node substitute(Var var, Texpr1Node t);

    public Texpr1Node substitute(String var, Texpr1Node t)
    {
        return substitute(new StringVar(var), t);
    }

    /**
     * Returns a copy of this where all occurrences of the variable var
     * are substituted with the expression tree t. 
     */
    public Texpr1Node substituteCopy(Var var, Texpr1Node t)
    {
        Texpr1Node x = this.deepCopy();
        return x.substitute(var, t);
    }

    public Texpr1Node substituteCopy(String var, Texpr1Node t)
    {
        return substituteCopy(new StringVar(var), t);
    }

    /** Recursive hashing. */
    public abstract int hashCode();

    /** Structural (recursive) equality. */
    public abstract boolean isEqual(Texpr1Node t);

    /** Precedence of the node, used when printing. */
    public abstract int getPrecedence();


    // Rounding format
    //////////////////

    /** Real (no rounding). */
    static public final int RTYPE_REAL = 0;

    /** Integer rounding. */
    static public final int RTYPE_INT = 1;

    /** Single-precision rounding: 32-bit IEEE 754 (float). */
    static public final int RTYPE_SINGLE = 2;

    /** Double-precision rounding: 64-bit IEEE 754 (double). */
    static public final int RTYPE_DOUBLE = 3;

    /** Extended-precision rounding: 80-bit (Intel's long double). */
    static public final int RTYPE_EXTENDED = 4;

    /** Quad-precision rounding: 128-bit (Motorola's long double). */
    static public final int RTYPE_QUAD = 5;


    // Rounding direction
    /////////////////////
    
    /** Rounds to nearest. */
    static public final int RDIR_NEAREST = 0;
    
    /** Rounds towards 0 (truncation). */
    static public final int RDIR_ZERO = 1;
    
    /** Rounds towards +oo. */
    static public final int RDIR_UP = 2;

    /** Rounds towards -oo. */
    static public final int RDIR_DOWN = 3;

    /** Non-deterministic rounding (all four modes considered). */
    static public final int RDIR_RND = 4;

    /** Whether x is a Texpr1 tree and structurally equal to this. */
    public abstract boolean equals(Object x);

    /** Returns a deep copy of this. */
    public Texpr1Node clone()
    {
        return deepCopy();
    }

}
