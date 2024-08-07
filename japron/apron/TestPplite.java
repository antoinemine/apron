/*
 * TestPplite.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 * Copyright (C) Enea Zaffanella 2018-2023
 */

package apron;

import java.math.BigInteger;
import java.io.*;
import java.util.Arrays;
import gmp.*;

/**
 * Test ad-hoc PPLite functionality in Java binding
 *
 * <p> Run with: java -ea -esa apron.TestPplite
 */
public class TestPplite
{

    public static void testDomain(Manager man)
        throws ApronException
    {
        /* build some expressions */

        /* level 0 */
        Interval[] box = { new Interval(1,2), new Interval(-3,5), new Interval(3,4,6,5) };
        Linterm0[] ltrms =
            { new Linterm0(1, new MpqScalar(-5)),
              new Linterm0(0, new Interval(0.1,0.6)),
              new Linterm0(2, new MpfrScalar(0.1, Mpfr.RNDU))
            };
        Linexpr0 linexp = new Linexpr0(ltrms, new MpqScalar(2));
        Linterm0[] ltrms2 =
            { new Linterm0(1, new MpqScalar(-5)),
              new Linterm0(0, new MpqScalar(1,2))
            };
        Linexpr0 linexp2 = new Linexpr0(ltrms2, new MpqScalar(2));
        Texpr0Node txpr =
            new Texpr0BinNode(Texpr0BinNode.OP_ADD,
                              new Texpr0BinNode(Texpr0BinNode.OP_MUL,
                                                new Texpr0DimNode(0),
                                                new Texpr0DimNode(1)),
                              new Texpr0BinNode(Texpr0BinNode.OP_DIV,
                                                new Texpr0DimNode(2),
                                                new Texpr0CstNode(new MpqScalar(2))));
        Texpr0Intern texp = new Texpr0Intern(txpr);
        Lincons0 lincons = new Lincons0( Lincons0.SUPEQ, linexp);
        Lincons0 lincons2 = new Lincons0( Lincons0.EQ, linexp2);
        Lincons0[] linconss = { lincons, lincons2 };
        Tcons0 tcons = new Tcons0(Tcons0.SUPEQ, texp);
        Generator0 gen = new Generator0(Generator0.RAY, linexp2);
        Generator0[] gens = { gen };

        int[] chgtab = { 0,1,2 };
        Dimchange chg = new Dimchange(2,1,chgtab);

        int[] permtab = { 1,0,2 };
        Dimperm perm = new Dimperm(permtab);

        String[] names = { "me", "myself", "I" };

        /* level 1 */

        String[] inames = { "z", "a" };
        String[] inames2 = { "z", "t" };
        String[] rnames = { "b" };
        String[] bnames = { "a", "b", "z" };
        Environment env = new Environment(inames, rnames);
        Environment env2 = new Environment(inames2, rnames);
        Linterm1[] xltrms =
            { new Linterm1("z", new MpqScalar(-5)),
              new Linterm1("b", new Interval(0.1,0.6)),
              new Linterm1("a", new MpfrScalar(0.1, Mpfr.RNDU))
            };
        Linexpr1 xlinexp = new Linexpr1(env, xltrms, new MpqScalar(2));
        Linterm1[] xltrms2 =
            { new Linterm1("a", new MpqScalar(-5)),
              new Linterm1("z", new MpqScalar(1,2))
            };
        Linexpr1 xlinexp2 = new Linexpr1(env, xltrms2, new MpqScalar(2));
        Texpr1Node xtxpr =
            new Texpr1BinNode(Texpr1BinNode.OP_ADD,
                              new Texpr1BinNode(Texpr1BinNode.OP_MUL,
                                                new Texpr1VarNode("a"),
                                                new Texpr1VarNode("z")),
                              new Texpr1BinNode(Texpr1BinNode.OP_DIV,
                                                new Texpr1VarNode("b"),
                                                new Texpr1CstNode(new MpqScalar(2))));
        Texpr1Intern xtexp = new Texpr1Intern(env, xtxpr);
        Lincons1 xlincons = new Lincons1( Lincons1.SUPEQ, xlinexp);
        Lincons1 xlincons2 = new Lincons1( Lincons1.EQ, xlinexp2);
        Lincons1[] xlinconss = { xlincons, xlincons2 };
        Tcons1 xtcons = new Tcons1(Tcons1.SUPEQ, xtexp);
        Generator1 xgen = new Generator1(Generator1.RAY, xlinexp2);
        Generator1[] xgens = { xgen };


        /* manager test */
        System.out.println("lib: " + man.getLibrary());
        System.out.println("ver: " + man.getVersion());

        /* level 0 abstract elements */
        Abstract0 full = new Abstract0(man, 2, 1);
        Abstract0 empty = new Abstract0(man, 2, 1, true);
        Abstract0 a0 = new Abstract0(man, 2, 1, box);
        System.out.println("full: " + full);
        System.out.println("empty: " + empty);
        System.out.println("a0: " + a0);
        System.out.println("a0: " + a0.toString(man, names));
        assert !full.isBottom(man); assert full.isTop(man);
        assert empty.isBottom(man); assert !empty.isTop(man);
        assert !a0.isBottom(man); assert !a0.isTop(man);
        assert a0.isEqual(man, a0);
        assert empty.isEqual(man, empty);
        assert full.isEqual(man, full);
        assert empty.isIncluded(man, a0);
        assert a0.isIncluded(man, full);
        assert a0.isIncluded(man, a0);
        assert !a0.isIncluded(man, empty);
        assert !full.isIncluded(man, a0);
        System.out.println("size: " + a0.getSize(man));
        System.out.println("hash: " + a0.hashCode(man));
        System.out.println("dim:  " + a0.getDimension(man));
        Manager man2 = a0.getCreationManager();
        assert man.getLibrary().equals(man2.getLibrary());
        a0.isBottom(man2);
        System.out.println("to-lcons: " + Arrays.toString(a0.toLincons(man)));
        System.out.println("to-box: " + Arrays.toString(a0.toBox(man)));
        System.out.println("to-tcons: " + Arrays.toString(a0.toTcons(man)));
        try { System.out.println("to-gen: " + Arrays.toString(a0.toGenerator(man))); }
        catch (ApronException e) { System.out.println("got exception: " + e); }
        System.out.println("bound 0:   " + a0.getBound(man, 0));
        System.out.println("bound lin: " + linexp + " -> " + a0.getBound(man, linexp));
        System.out.println("bound t:  " + texp + " -> " + a0.getBound(man, texp));
        System.out.println("sat lin:  " + a0.satisfy(man, lincons));
        System.out.println("sat t:    " + a0.satisfy(man, tcons));
        System.out.println("sat 0:    " + box[0] + " -> " + a0.satisfy(man, 0, box[0]));
        System.out.println("sat 1:    " + box[0] + " -> " + a0.satisfy(man, 1, box[0]));
        System.out.println("uncons 0: " + a0.isDimensionUnconstrained(man, 0));

        Abstract0 a1 = new Abstract0(man, a0);
        assert a0.isEqual(man, a1);
        a1.addRay(man, gen);
        System.out.println("+ gen: " + gen + " -> " + a1);
        assert !a0.isEqual(man, a1);
        assert a0.isIncluded(man, a1); assert !a1.isIncluded(man, a0);
        assert a1.isEqual(man, a0.addRayCopy(man, gen));

        Abstract0 ac = new Abstract0(man, a0);
        System.out.println("assign-lexp: " + a0.assignCopy(man, 0, linexp, null));
        System.out.println("assign-texp: " + a0.assignCopy(man, 0, texp, null));
        System.out.println("subst-lexp: " + a0.substituteCopy(man, 0, linexp, null));
        System.out.println("subst-texp: " + a0.substituteCopy(man, 0, texp, null));
        assert a0.isEqual(man, ac);
        ac.assign(man, 0, linexp, null);
        assert ac.isEqual(man, a0.assignCopy(man, 0, linexp, null));
        assert !ac.isEqual(man, a0);
        ac.assign(man, 0, texp, null);
        ac.substitute(man, 0, linexp, null);
        ac.substitute(man, 0, texp, null);


        assert a0.meetCopy(man, full).isEqual(man, a0);
        assert a0.joinCopy(man, empty).isEqual(man, a0);
        assert a0.meetCopy(man, empty).isEqual(man, empty);
        assert a0.joinCopy(man, full).isEqual(man, full);
        assert a0.meetCopy(man, a0).isEqual(man, a0);
        assert a0.joinCopy(man, a0).isEqual(man, a0);

        assert a0.meetCopy(man, lincons).isIncluded(man, a0);
        assert a0.meetCopy(man, lincons2).isIncluded(man, a0);
        assert a0.meetCopy(man, tcons).isIncluded(man, a0);
        System.out.println("+ const: " + lincons2 + " -> " + a0.meetCopy(man, lincons2));
        Abstract0 w = full.meetCopy(man, lincons2);
        System.out.println("widen: "+ a0.widening(man, w));
        System.out.println("widen threshold: "+ a0.wideningThreshold(man, w, linconss));

        Abstract0 ac2 = new Abstract0(man, a0);
        ac2.meet(man, linconss);
        ac2.assign(man, 0, linexp, a0);
        ac2.substitute(man, 0, linexp2, null);
        ac2.meet(man, tcons);
        ac2.join(man, a0);
        Abstract0[] aa = { a0, ac, ac2, empty, full };
        assert Abstract0.join(man, aa).isTop(man);
        assert Abstract0.meet(man, aa).isBottom(man);

        try { ac2.minimize(man); } catch (NotImplementedException e) { }
        try { ac2.canonicalize(man); } catch (NotImplementedException e) { }
        try { ac2.approximate(man, 12); } catch (NotImplementedException e) { }

        System.out.println("forget: "+ a0.forgetCopy(man, 0, true));
        Abstract0 ac3 = new Abstract0(man, a0);
        ac3.forget(man, 0, false);
        System.out.println("forget: "+ ac3);

        System.out.println("add-dim: " + a0.addDimensionsCopy(man, chg, true));
        Abstract0 ac4 = new Abstract0(man, a0);
        ac4.addDimensions(man, chg, false);
        assert ac4.isEqual(man, a0.addDimensionsCopy(man, chg, false));
        chg.addInvert();
        assert a0.isEqual(man, ac4.removeDimensionsCopy(man, chg));
        ac4.removeDimensions(man, chg);
        assert ac4.isEqual(man, a0);

        System.out.println("permute: " + a0.permuteDimensionsCopy(man, perm));
        Abstract0 ac5 = new Abstract0(man, a0);
        ac5.permuteDimensions(man, perm);
        assert ac5.isEqual(man, a0.permuteDimensionsCopy(man, perm));
        perm.invert();
        assert a0.isEqual(man, ac5.permuteDimensionsCopy(man, perm));
        ac5.permuteDimensions(man, perm);
        assert ac5.isEqual(man, a0);

        System.out.println("expand: " + a0.expandCopy(man, 0, 2));
        Abstract0 ac6 = new Abstract0(man, a0);
        ac6.expand(man, 0, 2);
        assert ac6.isEqual(man, a0.expandCopy(man, 0, 2));
        int[] fold = { 1,2,3 };
        assert a0.isEqual(man, ac6.foldCopy(man, fold));
        ac6.fold(man, fold);
        assert ac6.isEqual(man, a0);

        Abstract0 ac7 = new Abstract0(man, a0);
        ac7.closure(man);
        assert ac7.isEqual(man, a0.closureCopy(man));

        System.out.println("--------");

         /* level 1 abstract elements */
        Abstract1 xfull = new Abstract1(man, env);
        Abstract1 xempty = new Abstract1(man, env, true);
        Abstract1 xa0 = new Abstract1(man, env, bnames, box);
        System.out.println("full: " + xfull);
        System.out.println("empty: " + xempty);
        System.out.println("a0:  " + xa0);
        System.out.println("a0:  " + xa0.toString(man));
        assert !xfull.isBottom(man); assert xfull.isTop(man);
        assert xempty.isBottom(man); assert !xempty.isTop(man);
        assert !xa0.isBottom(man); assert !xa0.isTop(man);
        assert xa0.isEqual(man, xa0);
        assert xempty.isEqual(man, xempty);
        assert xfull.isEqual(man, xfull);
        assert xempty.isIncluded(man, xa0);
        assert xa0.isIncluded(man, xfull);
        assert xa0.isIncluded(man, xa0);
        assert !xa0.isIncluded(man, xempty);
        assert !xfull.isIncluded(man, xa0);
        System.out.println("size: " + xa0.getSize(man));
        System.out.println("hash: " + xa0.hashCode(man));
        System.out.println("env:  " + xa0.getEnvironment());
        System.out.println("lvl0: " + xa0.getAbstract0(man));
        Manager xman2 = xa0.getCreationManager();
        assert man.getLibrary().equals(xman2.getLibrary());
        xa0.isBottom(xman2);
        System.out.println("to-lcons: " + Arrays.toString(xa0.toLincons(man)));
        System.out.println("to-box: " + Arrays.toString(xa0.toBox(man)));
        System.out.println("to-tcons: " + Arrays.toString(xa0.toTcons(man)));
        try { System.out.println("to-gen: " + Arrays.toString(xa0.toGenerator(man))); }
        catch (ApronException e) { System.out.println("got exception: " + e); }

        System.out.println("bound a:   " + xa0.getBound(man, "a"));
        System.out.println("bound lin: " + xlinexp + " -> " + xa0.getBound(man, xlinexp));
        System.out.println("bound t:  " + xtexp + " -> " + xa0.getBound(man, xtexp));
        System.out.println("sat lin:  " + xa0.satisfy(man, xlincons));
        System.out.println("sat t:    " + xa0.satisfy(man, xtcons));
        System.out.println("sat a:    " + box[0] + " -> " + xa0.satisfy(man, "a", box[0]));
        System.out.println("sat z:    " + box[0] + " -> " + xa0.satisfy(man, "z", box[0]));
        System.out.println("uncons a: " + xa0.isDimensionUnconstrained(man, "a"));

        Abstract1 xa1 = new Abstract1(man, xa0);
        assert xa0.isEqual(man, xa1);
        xa1.addRay(man, xgen);
        System.out.println("+ gen: " + xgen + " -> " + xa1);
        assert !xa0.isEqual(man, xa1);
        assert xa0.isIncluded(man, xa1); assert !xa1.isIncluded(man, xa0);
        assert xa1.isEqual(man, xa0.addRayCopy(man, xgen));

        Abstract1 xac = new Abstract1(man, xa0);
        System.out.println("assign-lexp: " + xa0.assignCopy(man, "a", xlinexp, null));
        System.out.println("assign-texp: " + xa0.assignCopy(man, "a", xtexp, null));
        System.out.println("subst-lexp: " + xa0.substituteCopy(man, "a", xlinexp, null));
        System.out.println("subst-texp: " + xa0.substituteCopy(man, "a", xtexp, null));
        assert xa0.isEqual(man, xac);
        xac.assign(man, "a", xlinexp, null);
        assert xac.isEqual(man, xa0.assignCopy(man, "a", xlinexp, null));
        assert !xac.isEqual(man, xa0);
        xac.assign(man, "z", xtexp, null);
        xac.substitute(man, "z", xlinexp, null);
        xac.substitute(man, "z", xtexp, null);


        assert xa0.meetCopy(man, xfull).isEqual(man, xa0);
        assert xa0.joinCopy(man, xempty).isEqual(man, xa0);
        assert xa0.meetCopy(man, xempty).isEqual(man, xempty);
        assert xa0.joinCopy(man, xfull).isEqual(man, xfull);
        assert xa0.meetCopy(man, xa0).isEqual(man, xa0);
        assert xa0.joinCopy(man, xa0).isEqual(man, xa0);

        assert xa0.meetCopy(man, xlincons).isIncluded(man, xa0);
        assert xa0.meetCopy(man, xlincons2).isIncluded(man, xa0);
        assert xa0.meetCopy(man, xtcons).isIncluded(man, xa0);
        System.out.println("+ const: " + xlincons2 + " -> " + xa0.meetCopy(man, xlincons2));
        Abstract1 xw = xfull.meetCopy(man, xlincons2);
        System.out.println("widen: "+ xa0.widening(man, xw));
        System.out.println("widen threshold: "+ xa0.wideningThreshold(man, xw, xlinconss));

        Abstract1 xac2 = new Abstract1(man, xa0);
        xac2.meet(man, xlinconss);
        xac2.assign(man, "a", xlinexp, xa0);
        xac2.substitute(man, "a", xlinexp2, null);
        xac2.meet(man, xtcons);
        xac2.join(man, xa0);
        Abstract1[] xaa = { xa0, xac, xac2, xempty, xfull };
        assert Abstract1.join(man, xaa).isTop(man);
        assert Abstract1.meet(man, xaa).isBottom(man);

        try { xac2.minimize(man); } catch (NotImplementedException e) { }
        try { xac2.canonicalize(man); } catch (NotImplementedException e) { }
        try { xac2.approximate(man, 12); } catch (NotImplementedException e) { }

        System.out.println("forget: "+ xa0.forgetCopy(man, "z", true));
        Abstract1 xac3 = new Abstract1(man, xa0);
        xac3.forget(man, "a", false);
        System.out.println("forget: "+ xac3);

        String[] xexp = { "a0", "z0" };
        System.out.println("expand: " + xa0.expandCopy(man, "a", xexp));
        Abstract1 xac6 = new Abstract1(man, xa0);
        xac6.expand(man, "z", xexp);
        assert xac6.isEqual(man, xa0.expandCopy(man, "z", xexp));
        String[] xfold = { "z", "z0", "a0" };
        assert xa0.isEqual(man, xac6.foldCopy(man, xfold));
        String[] xfold2 = {  "z0", "a0", "z" };
        System.out.println("fold: " + xac6.foldCopy(man, xfold2));
        xac6.fold(man, xfold);
        assert xac6.isEqual(man, xa0);

        Abstract1 xac7 = new Abstract1(man, xa0);
        xac7.closure(man);
        assert xac7.isEqual(man, xa0.closureCopy(man));

        Abstract1 xac8 = new Abstract1(man, env2);
        try { xa0.isEqual(man, xac8); assert false; } catch (IllegalArgumentException e) { /* expected */ }
        Abstract1 xac9 = new Abstract1(man, xa0);
        xac9.changeEnvironment(man, env2, true);
        System.out.println("chg-env: " + xac9);
        assert xac9.isEqual(man, xa0.changeEnvironmentCopy(man, env2, true));
        try { xa0.isEqual(man, xac9); assert false; } catch (IllegalArgumentException e) { /* expected */ }
        System.out.println("unify: " + xac9.unifyCopy(man, xa0));
        Abstract1 xac10 = new Abstract1(man, xa0);
        xac10.unify(man, xac9);
        assert xac9.unifyCopy(man, xa0).isEqual(man, xac10);

        String[] xorg = { "a" };
        String[] xdst = { "zz99" };
        Abstract1 xac12 = new Abstract1(man, xa0);
        xac12.rename(man, xorg, xdst);
        System.out.println("rename: " + xac12);
        assert xac12.isEqual(man, xa0.renameCopy(man, xorg, xdst));

        Abstract1 xa00 = xa0.forgetCopy(man, "z", false);
        Abstract1 xac11 = new Abstract1(man, xa00);
        xac11.minimizeEnvironment(man);
        System.out.println("min-env: " + xac11.getEnvironment() + " : " + xac11);
        assert xac11.isEqual(man, xa00.minimizeEnvironmentCopy(man));

        /* serialization test */
        try {
            byte[] raw = a0.serialize(man);
            assert a0.isEqual(man, Abstract0.deserialize(man, raw));
        }
        catch (NotImplementedException e) { }
        try {
            byte[] xraw = xa0.serialize(man);
            assert xa0.isEqual(man, Abstract1.deserialize(man, env, xraw));
        }
        catch (NotImplementedException e) { }
    }

    public static String safe_toString(Pplite man, Abstract0 a)
        throws ApronException
    {
        try {
            int nd = man.getNumDisjuncts(a);
            if (nd <= 1)
                return a.toString();
            Var[] names = null;
            StringBuffer buf = new StringBuffer();
            for (int d = 0; d < nd; ++d) {
                Lincons0[] lc = man.toDisjunctLincons(a, d);
                if (d == 0) buf.append("{"); else buf.append(" || {");
                for (int i = 0; i < lc.length; ++i) {
                    if (i == 0) buf.append(" "); else buf.append("; ");
                    buf.append(lc[i].toString(names));
                }
                buf.append(" }");
            }
            return buf.toString();
        }
        catch (ApronException e) {
            return "<caught exception: " + e.getMessage() + ">";
        }
    }

    /* Main */
    /* ---- */

    /** Calling main performs a few API tests. */
    public static void main(String[] args)
         throws ApronException, CloneNotSupportedException
    {
       for (int i=0; i<10; i++) {
           mainx(args);
           System.gc();
           System.runFinalization();
       }
    }

    public static void mainx(String[] args)
        throws ApronException, CloneNotSupportedException
    {
        System.out.println("");
        System.out.println("PPLite Polyhedra");
        System.out.println("================");
        Pplite man = new Pplite(false);

        man.setKind("Poly");
        testDomain(man);
        System.out.println("");
        System.out.println("Tested PPLite's Java wrapper for kind "
                           + man.getKind() + ".");

        man.setKind("F_Poly");
        testDomain(man);
        System.out.println("");
        System.out.println("Tested PPLite's Java wrapper for kind "
                           + man.getKind() + ".");
    }

}
