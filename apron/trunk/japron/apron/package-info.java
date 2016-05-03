/*
 * packate-info.java
 *
 * package information
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

/**
 * Java binding for the Apron library.
 *
 * <p> The binding is based on Apron 0.9.10.
 *
 * <h2>Short introduction</h2>
 * 
 * <p> To start with Apron, you should first choose a numerical abstract 
 * domain. i.e., any subclass of {@link apron.Manager}. For instance,
 * {@link apron.Polka} corresponds to the convex polyhedra domain.
 * The constructor {@link apron.Polka#Polka} allows you to construct one
 * (or several, which is mainly useful in a multi-threaded application)
 * abstract domain instance.
 *
 * <p> Then, you need choose whether to use level 0 or level 1 operations.
 * In level 0 operations, abstract elements are sets of points in a
 * Cartesian space Z^n * R^m, and all data-structures (expressions,
 * constraints, etc.) consider variables as dimensions, indexed from 0.
 * In level 1 operations, abstract elements are sets of points in
 * an {@link apron.Environment} (i.e., set of variables) and variables 
 * are abstract {@link apron.Var} (possibly simple strings, using
 * {@link apron.StringVar}). The main advantage of level 1 is that you do not
 * need to bother with the mapping from program variables to dimensions in your
 * static analysis (which could be tricky if variables are not created and
 * destroyed in a last in / first out manner): the library does it for you.
 * Actually, the level 1 is a implemented fully in Java, on top of the
 * native level 0 code.
 * Apron checks that dimension spaces (in level 0) or 
 * environments (in level 1) for all the arguments are compatible.
 *
 * <p> With your {@link apron.Manager} instance, you can now create
 * and manipulate abstract elements using either the {@link apron.Abstract0} 
 * or the {@link apron.Abstract1} class, depending on which level you prefer.
 * Note that the {@link apron.Manager} needs to be passed as first argument
 * of almost all functions in these classes as it points back to the
 * actual abstract domain implementation instance.
 *
 * <p> To apply transfer functions such as assignments or constraint addition,
 * you need to create extra objects to denote expressions and constraints.
 * Apron has linear expressions and arbitrary expression trees,
 * respectively {@link apron.Linexpr0}, {@link apron.Linexpr1}, 
 * {@link apron.Texpr0Intern}, {@link apron.Texpr1Intern}, 
 * depending on the level you chose.
 * Linear expressions are akin arrays but, as they are backed by native
 * Apron C objects, they must be accessed with a set/get paradigm.
 * Expression trees are a bit more complex: the internal native C 
 * data-structures are a bit too complex to be accessed directly. 
 * Instead, the Java binding provides pure
 * Java expression trees ({@link apron.Texpr0Node}, {@link apron.Texpr1Node} 
 * and subclasses), and wrappers for opaque Apron C expression trees
 * ({@link apron.Texpr0Intern}, {@link apron.Texpr1Intern}).
 * The later are used for {@link apron.Abstract0} and {@link apron.Abstract1}
 * arguments and return values; and can be converted to and from the former
 * for easier construction and inspection on the Java side.
 * Each expression type (linear / tree, level 0 / level 1) has a corresponding
 * constraint type, using the expression as left argument, 0 as right
 * argument, and some comparison operator (plus an optional extra number
 * to denote modulo).
 *
 * <p> All expressions and constraints use a generic {@link apron.Coeff}
 * class to denote constants. These can be simple scalars ({@link apron.Scalar}),
 * or intervals with constant scalar bounds ({@link apron.Interval}).
 * Scalars can in turn be either represented by machine floating-point
 * numbers ({@link apron.DoubleScalar}), arbitrary-precision fractions
 * ({@link apron.MpqScalar}), or arbitrary-precision floating-point
 * numbers ({@link apron.MpfrScalar}).
 * You can choose whichever suites you to talk to Apron and, when Apron
 * gives you back some {@link apron.Scalar}, you can convert it back
 * to a double, a Mpq, or a Mpfr.
 *
 * <p> Remember that abstract operations may be inexact and imprecise.
 * The contract states that abstract domain implementations can perform
 * approximations but must always respect soundness: a result abstract
 * element or interval may be an over-approximation. Likewise, a predicate
 * is definitely true if the function returns true, but is unknown if the
 * function returns false.
 *
 * <h2>Warning</h2>
 *
 * <p> The Apron library changes the FPU rounding-mode towards +oo, to ensure
 * the soundness of all computations.
 * This can confuse some JVMs.
 * For instance, this causes an error on Sun's JDK 1.6 when the option
 * {@code -Xcheck:jni} is used.
 *
 * @see <a href="http://apron.cri.ensmp.fr/library/">http://apron.cri.ensmp.fr/library</a> 
 * @author Antoine Miné
 * @version 0.1
 */
package apron;
