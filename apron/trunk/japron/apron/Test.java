/*
 * Test.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package apron;

import java.math.BigInteger;
import java.io.*;
import java.util.Arrays;
import gmp.*;

/**
 * Simple test for the Java Apron binding.
 *
 * <p> Run with: java -ea -esa apron.Test
 */
public class Test
{

    /* Serialization test */
    /* ------------------ */

    public static Object testSerialize(Object o) 
    {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            ObjectOutputStream jout = new ObjectOutputStream(out);
            jout.writeObject(o);
            jout.close();
            System.out.println("OUT: "+o);
            ByteArrayInputStream in = new ByteArrayInputStream(out.toByteArray());
            ObjectInputStream jin = new ObjectInputStream(in);
            Object oo = jin.readObject();
            System.out.println("IN : "+oo);
            jin.close();
            return oo;
        }
        catch (Exception e) {
            System.out.println("caught exception " + e);
            return null;
        }
    }


    /* Abastract domain testing */
    /* ------------------------ */

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


    /* Main */
    /* ---- */

    
    /** Calling main performs a few API tests. */
    public static void main(String[] args)
         throws ApronException, CloneNotSupportedException
   {
       int i;
       for (i=0; i<10; i++) {
           mainx(args);
           System.gc();
           System.runFinalization();
       }
    }
    
    public static void mainx(String[] args)
        throws ApronException, CloneNotSupportedException
    {
        /* level 0 */

        System.out.println("Dimperm test");
        System.out.println("============");
        int[] permtab = { 1,2,3,4,5,6,0 };
        Dimperm perm1 = new Dimperm(permtab);
        Dimperm perm2 = new Dimperm(7);
        System.out.println("shift:      " + perm1);
        System.out.println("id:         " + perm2);
        System.out.println("inv shift:  " + perm1.invert());
        System.out.println("shift+id:   " + perm1.compose(perm2));
        perm2.setElem(0,1); perm2.setElem(1,0);
        assert perm2.getElem(0) == 1;
        assert perm2.getSize() == 7;
        System.out.println("swap:       " + perm2);
        System.out.println("shift+swap: " + perm1.compose(perm2));
        perm1.setId();
        System.out.println("id:         " + perm1);
        assert perm1.equals(testSerialize(perm1));

        System.out.println("");
        System.out.println("Dimchange test");
        System.out.println("==============");
        int[] chgtab = { 0,3,4 };
        Dimchange chg = new Dimchange(2,1,chgtab);
        System.out.println(chg);
        System.out.println(chg.getDimension() + ", " + Arrays.toString(chg.getContents()));
        assert chg.equals(testSerialize(chg));

        System.out.println("");
        System.out.println("Linexpr0 test");
        System.out.println("=============");
        Linexpr0 l1 = new Linexpr0();
        Linexpr0 l2 = new Linexpr0(10);
        l1.setCst(new MpqScalar(1, 2));
        l1.setCoeff(0, new MpfrScalar(1.23, Mpfr.RNDU));
        l1.setCoeff(1, new DoubleScalar(1.56));
        l1.setCoeff(2, new Interval(1,2 ,3,4));
        l1.setCoeff(3, new Interval(new Mpq(4,5), new Mpq(6,7)));
        l1.setCoeff(4, new Interval(new Mpfr(9.23, Mpfr.RNDU), new Mpfr(6.09, Mpfr.RNDU)));
        l1.setCoeff(5, new Interval(1.23, 4.56));
        Interval i = new Interval();
        i.setTop();
        l1.setCoeff(6, i);
        l2.setCoeff(2, new DoubleScalar(-2));
        l2.setCoeff(3, new MpqScalar(-2,3));
        try { l1.setCoeff(-9, i); assert false; } catch (IndexOutOfBoundsException e) { /* exception expected */ }
        try { l2.setCoeff(99, i); assert false; } catch (IndexOutOfBoundsException e) { /* exception expected */ }
        String[] names = { "a","b","c","d","e","f","g","h","i","j" };
        System.out.println("l1: " + l1);
        System.out.println("l2: " + l2.toString(names));
        l1.permuteDimensions(perm2);  
        System.out.println("permuted:  " + l1);
        l2.addDimensions(chg);       
        System.out.println("dim added: " + l2.toString(names));
        System.out.println("l1 terms: " + Arrays.toString(l1.getLinterms()));
        System.out.println("l2 terms: " + Arrays.toString(l2.getLinterms()));
        System.out.println("l1 coefs: " + Arrays.toString(l1.getCoeffs()));
        System.out.println("l2 coefs: " + Arrays.toString(l2.getCoeffs()));
        System.out.println("l1 hash: " + l1.hashCode());
        System.out.println("l2 hash: " + l2.hashCode());
        System.out.println("l1 pred: " + l1.isSparse() + ", " + l1.isInteger(3) + ", " + l1.isReal(3) + ", " + l1.isLinear() + ", " + l1.isQuasilinear());
        System.out.println("l2 pred: " + l2.isSparse() + ", " + l2.isInteger(3) + ", " + l2.isReal(3) + ", " + l2.isLinear() + ", " + l2.isQuasilinear());
        assert l1.isEqual(l1);  assert l2.isEqual(l2);
        assert !l1.isEqual(l2); assert !l2.isEqual(l1);
        assert l1.isEqual(new Linexpr0(l1));
        assert l1.equals(testSerialize(l1));
        assert l2.equals(testSerialize(l2));

        System.out.println("");
        System.out.println("Texpr0 test");
        System.out.println("===========");
        Texpr0Intern ti = new Texpr0Intern(l1);
        Texpr0Node tn = Texpr0Node.fromLinexpr0(l1);
        System.out.println("ti:     " + ti);
        System.out.println("ti cvt: " + ti.toTexpr0Node());
        System.out.println("tn:     " + tn);
        System.out.println("tn cvt: " + (new Texpr0Intern(tn)).toTexpr0Node());
        System.out.println("tn cpy: " + tn.deepCopy());
        System.out.println("depth: " + ti.getDepth() + ", size: " + ti.getSize() + ", max: " + ti.maxDim() + ", cst: " + ti.isIntervalCst() + ", lin: " + ti.isIntervalLinear() + ", poly: " + ti.isIntervalPolynomial() + ", frac: " + ti.isIntervalPolyfrac() + ", scalar: " + ti.isScalar() + ", hash: " + ti.hashCode() );
        System.out.println("depth: " + tn.getDepth() + ", size: " + tn.getSize() + ", max: " + tn.maxDim() + ", cst: " + tn.isIntervalCst() + ", lin: " + tn.isIntervalLinear() + ", poly: " + tn.isIntervalPolynomial() + ", frac: " + tn.isIntervalPolyfrac() + ", scalar: " + tn.isScalar() + ", hash: " + tn.hashCode());
        System.out.println("ti dims: " + Arrays.toString(ti.getDims()));
        System.out.println("tn dims: " + Arrays.toString(tn.getDims()));
        Texpr0Node tt = new Texpr0BinNode(Texpr0BinNode.OP_DIV, 
                                          Texpr0BinNode.RTYPE_SINGLE,
                                          Texpr0BinNode.RDIR_UP,
                                          new Texpr0DimNode(2),
                                          new Texpr0DimNode(12));
        assert !ti.isEqual(new Texpr0Intern(tt));
        assert !tn.isEqual(tt);
        ti.substitute(2, new Texpr0Intern(tt));
        tn.substitute(2, tt);
        System.out.println("ti susbt: " + ti);
        System.out.println("tn subst: " + tn);
        assert ti.isEqual(new Texpr0Intern(ti));
        assert tn.isEqual(tn.shallowCopy());
        assert tn.isEqual(tn.deepCopy());
        assert ti.isEqual(new Texpr0Intern(tn));
        assert tn.isEqual(ti.toTexpr0Node());
        ti.addDimensions(chg);
        tn.addDimensions(chg);
        System.out.println("ti add: " + ti);
        assert tn.isEqual(ti.toTexpr0Node());
        ti.removeDimensions(chg);
        tn.removeDimensions(chg);
        System.out.println("ti rem: " + ti);
        assert tn.isEqual(ti.toTexpr0Node());
        ti.removeDimensions(chg);
        tn.removeDimensions(chg);
        System.out.println("ti rem: " + ti);
        assert tn.isEqual(ti.toTexpr0Node());
        assert ti.equals(testSerialize(ti));
        assert tn.equals(testSerialize(tn));
        
        System.out.println("");
        System.out.println("Lincons0");
        System.out.println("========");
        Lincons0 lc1 = new Lincons0(Lincons0.SUPEQ, l1);
        Lincons0 lc2 = new Lincons0(false);
        Lincons0 lc3 = new Lincons0(Lincons0.EQMOD, l1, new MpqScalar(1,2));
        System.out.println("lc1: " + lc1);
        System.out.println("lc2: " + lc2);
        System.out.println("lc3: " + lc3);
        System.out.println("unsat: " + lc1.isUnsat() + ", " + lc2.isUnsat() + ", " + lc3.isUnsat());
        System.out.println("lc1 pred: " + lc1.isSparse() + ", " + lc1.isInteger(3) + ", " + lc1.isReal(3) + ", " + lc1.isLinear() + ", " + lc1.isQuasilinear());
        System.out.println("lc2 pred: " + lc2.isSparse() + ", " + lc2.isInteger(3) + ", " + lc2.isReal(3) + ", " + lc2.isLinear() + ", " + lc2.isQuasilinear());
        System.out.println("lc3 pred: " + lc3.isSparse() + ", " + lc3.isInteger(3) + ", " + lc3.isReal(3) + ", " + lc3.isLinear() + ", " + lc3.isQuasilinear());
        System.out.println("kind: " + lc1.getKind() + ", " +  lc2.getKind() + ", " + lc3.getKind());
        System.out.println("scalar: " + lc1.getScalar() + ", " +  lc2.getScalar() + ", " + lc3.getScalar());
        assert lc1.equals(testSerialize(lc1));
        assert lc2.equals(testSerialize(lc2));
        assert lc3.equals(testSerialize(lc3));


        /* level 1 */

        System.out.println("");
        System.out.println("Environments");
        System.out.println("============");
        Environment env1 = new Environment();
        String[] n1 = { "aa", "zz", "cc" };
        String[] n2 = { "zzz", "cz", "a0" };
        String[] n3 = { "kk", "ll" };
        Environment env2 = new Environment( n1, n2 );
        //env1.add(n2,n1);
        //env2.add(new String[0],new String[0]);
        //env2.remove(n1); env2.remove(n2);
        System.out.println("empty: " + env1);
        System.out.println("env: " + env2);
        System.out.println("env: " + Arrays.toString(env2.getVars()));
        System.out.println("dim: " + env2.getDimension());
        System.out.println("contains: " + env2.hasVar("zzz") + ", " + env2.hasVar("zzzz"));
        System.out.println("dim a0: " + env2.dimOfVar("a0"));
        System.out.println("isint: " + env2.isInt("zz") + ", " + env2.isInt("a0"));
        System.out.println("var 1: " + env2.varOfDim(1));
        System.out.println("hash: " + env2.hashCode());
        System.out.println("add:  " + env2.add(new String[0], n3));
        Dimperm[] ren = new Dimperm[1];
        System.out.println("add:  " + env2.addPerm(new String[0], n3, ren));
        System.out.println("perm: " + ren[0]);
        Environment env3 = env2.add(new String[0], n3);
        System.out.println("rem: " + env3.remove(n2));
        assert env3.isEqual(env2.add(new String[0], n3));
        assert !env3.isEqual(env2);
        assert env2.isIncluded(env3);
        assert !env3.isIncluded(env2);
        assert env3.cmp(env2) == 1;
        assert env3.remove(n3).isEqual(env2);
        Environment env4 = new Environment(n1,n3);
        System.out.println("lce: " + env2.lce(env4));
        Environment[] envs = { env1, env3, env4 };
        Environment env5 = Environment.lce(envs);
        System.out.println("lce: " + env5);
        String[] nm1 = { "aa", "a0" };
        String[] nm2 = { "z0", "z1" };
        System.out.println("rename: " + env2.rename(nm1,nm2));
        System.out.println("rename: " + env2.rename(nm1,nm2,ren));
        System.out.println("perm:   " + ren[0]);
        System.out.println("dimchange: " + env2.dimchange(env3));
        System.out.println("dimchange2: " + Arrays.toString(env2.dimchange2(env4)));
        assert env1.equals(testSerialize(env1));
        assert env2.equals(testSerialize(env2));
        assert env3.equals(testSerialize(env3));

        System.out.println("");
        System.out.println("Linexpr1 test");
        System.out.println("=============");
        Linexpr1 ll1 = new Linexpr1(env2);
        Linexpr1 ll2 = new Linexpr1(env2, 3);
        ll1.setCst(new MpqScalar(1, 2));
        ll1.setCoeff("aa", new MpfrScalar(1.23, Mpfr.RNDU));
        ll1.setCoeff("zz", new DoubleScalar(1.56));
        ll1.setCoeff("cc", new Interval(1,2 ,3,4));
        ll1.setCoeff("zzz", new Interval(new Mpq(4,5), new Mpq(6,7)));
        ll1.setCoeff("cz", new Interval(new Mpfr(9.23, Mpfr.RNDU), new Mpfr(6.09, Mpfr.RNDU)));
        ll1.setCoeff("a0", new Interval(1.23, 4.56));
        ll2.setCoeff("zz", new DoubleScalar(-2));
        ll2.setCoeff("zzz", new MpqScalar(-2,3));
        ll2.setCoeff("aa", new MpqScalar(0));
        ll2.setCoeff("cz", new Interval(new Mpfr(2.34, Mpfr.RNDU), new Mpfr(2.34, Mpfr.RNDU)));
       try { ll1.setCoeff("toto", i); assert false; } catch (IllegalArgumentException e) { /* exception expected */ }
        try { ll2.setCoeff("blurg", i); assert false; } catch (IllegalArgumentException e) { /* exception expected */ }
        System.out.println("ll1: " + ll1);
        System.out.println("ll2: " + ll2);
        ll2.minimize();
        System.out.println("minimized ll2: " + ll2);
        System.out.println("ll1 terms: " + Arrays.toString(ll1.getLinterms()));
        System.out.println("ll2 terms: " + Arrays.toString(ll2.getLinterms()));
        System.out.println("ll1 lvl0: " + ll1.getLinexpr0());
        System.out.println("ll2 lvl0: " + ll2.getLinexpr0());
        System.out.println("ll1 hash: " + ll1.hashCode());
        System.out.println("ll2 hash: " + ll2.hashCode());
        System.out.println("ll1 pred: " + ll1.isSparse() + ", " + ll1.isInteger() + ", " + ll1.isReal() + ", " + ll1.isLinear() + ", " + ll1.isQuasilinear());
        System.out.println("ll2 pred: " + ll2.isSparse() + ", " + ll2.isInteger() + ", " + ll2.isReal() + ", " + ll2.isLinear() + ", " + ll2.isQuasilinear());
        assert ll1.isEqual(ll1);  assert ll2.isEqual(ll2);
        assert !ll1.isEqual(ll2); assert !ll2.isEqual(ll1);
        assert ll1.isEqual(new Linexpr1(ll1));
        System.out.println("ll1 ext: " + ll1.extendEnvironmentCopy(env5));
        System.out.println("ll2 ext: " + ll2.extendEnvironmentCopy(env5));
        assert ll1.equals(testSerialize(ll1));
        assert ll2.equals(testSerialize(ll2));

        System.out.println("");
        System.out.println("Texpr1 test");
        System.out.println("===========");
        Texpr1Intern tti = new Texpr1Intern(ll1);
        Texpr1Node ttn = Texpr1Node.fromLinexpr1(ll1);
        System.out.println("tti:      " + tti);
        System.out.println("tti cvt:  " + tti.toTexpr1Node());
        System.out.println("ttn:      " + ttn);
        System.out.println("ttn cvt:  " + (new Texpr1Intern(env2, ttn)).toTexpr1Node());
        System.out.println("ttn cpy:  " + ttn.deepCopy());
        System.out.println("tti lvl0: " + tti.getTexpr0Intern());
        System.out.println("ttn lvl0: " + ttn.toTexpr0Node(env2));
        System.out.println("depth: " + tti.getDepth() + ", size: " + tti.getSize() + ", max: " + ", cst: " + tti.isIntervalCst() + ", lin: " + tti.isIntervalLinear() + ", poly: " + tti.isIntervalPolynomial() + ", frac: " + tti.isIntervalPolyfrac() + ", scalar: " + tti.isScalar() + ", hash: " + tti.hashCode());
        System.out.println("depth: " + ttn.getDepth() + ", size: " + ttn.getSize() + ", max: " + ", cst: " + ttn.isIntervalCst() + ", lin: " + ttn.isIntervalLinear() + ", poly: " + ttn.isIntervalPolynomial() + ", frac: " + ttn.isIntervalPolyfrac() + ", scalar: " + ttn.isScalar() + ", hash: " + ttn.hashCode());
        System.out.println("tti vars: " + Arrays.toString(tti.getVars()));
        System.out.println("ttn vars: " + Arrays.toString(ttn.getVars()));
        Texpr1Node ttt = new Texpr1BinNode(Texpr1BinNode.OP_DIV, 
                                          Texpr1BinNode.RTYPE_SINGLE,
                                          Texpr1BinNode.RDIR_UP,
                                          new Texpr1VarNode("aa"),
                                          new Texpr1VarNode("zz"));
        assert !tti.isEqual(new Texpr1Intern(env2, ttt));
        assert !ttn.isEqual(ttt);
        tti.substitute("zz", new Texpr1Intern(env2, ttt));
        ttn.substitute("zz", ttt);
        System.out.println("tti subst: " + tti);
        System.out.println("ttn subst: " + ttn);
        assert tti.isEqual(new Texpr1Intern(tti));
        assert ttn.isEqual(ttn.shallowCopy());
        assert ttn.isEqual(ttn.deepCopy());
        assert tti.isEqual(new Texpr1Intern(env2, ttn));
        assert ttn.isEqual(tti.toTexpr1Node());
        tti.extendEnvironment(env5);
        System.out.println("tti ext: " + tti);
        assert ttn.isEqual(tti.toTexpr1Node());
        assert tti.equals(testSerialize(tti));
        assert ttn.equals(testSerialize(ttn));
        
        System.out.println("");
        System.out.println("Lincons1");
        System.out.println("========");
        Lincons1 llc1 = new Lincons1(Lincons0.SUPEQ, ll1);
        Lincons1 llc2 = new Lincons1(env2, false);
        Lincons1 llc3 = new Lincons1(Lincons0.EQMOD, ll1, new MpqScalar(1,2));
        System.out.println("llc1: " + llc1);
        System.out.println("llc2: " + llc2);
        System.out.println("llc3: " + llc3);
        System.out.println("llc1 lvl0: " + llc1.getLincons0());
        System.out.println("llc2 lvl0: " + llc2.getLincons0());
        System.out.println("llc3 lvl0: " + llc3.getLincons0());
        System.out.println(llc1.isUnsat() + ", " + llc2.isUnsat() + ", " + llc3.isUnsat());
        System.out.println("llc1 pred: " + llc1.isSparse() + ", " + llc1.isInteger() + ", " + llc1.isReal() + ", " + llc1.isLinear() + ", " + llc1.isQuasilinear());
        System.out.println("llc2 pred: " + llc2.isSparse() + ", " + llc2.isInteger() + ", " + llc2.isReal() + ", " + llc2.isLinear() + ", " + llc2.isQuasilinear());
        System.out.println("llc3 pred: " + llc3.isSparse() + ", " + llc3.isInteger() + ", " + llc3.isReal() + ", " + llc3.isLinear() + ", " + llc3.isQuasilinear());
        System.out.println("kind: " + llc1.getKind() + ", " +  llc2.getKind() + ", " + llc3.getKind());
        System.out.println("scalar: " + llc1.getScalar() + ", " +  llc2.getScalar() + ", " + llc3.getScalar());
        assert llc1.equals(testSerialize(llc1));
        assert llc2.equals(testSerialize(llc2));
        assert llc3.equals(testSerialize(llc3));


        /* abstract domains */

        System.out.println("");
        System.out.println("");
        System.out.println("Box");
        System.out.println("=========");
        testDomain(new Box());

        System.out.println("");
        System.out.println("Octagons");
        System.out.println("=========");
        testDomain(new Octagon());

        System.out.println("");
        System.out.println("Polyhedra");
        System.out.println("=========");
        testDomain(new Polka(false));
        System.out.println("");
        testDomain(new Polka(true));
        /*
        System.out.println("");
        System.out.println("PPL Polyhedra");
        System.out.println("=============");
        testDomain(new PplPoly(false));
        System.out.println("");
        testDomain(new PplPoly(true));

        System.out.println("");
        System.out.println("PPL Grids");
        System.out.println("=========");
        testDomain(new PplGrid());

        System.out.println("");
        System.out.println("Polyhedra + PPL Grids");
        System.out.println("=====================");
        testDomain(new PolkaGrid(false));
        System.out.println("");
        testDomain(new PolkaGrid(true));
        */
    }

}
