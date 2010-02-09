/*
 * Test.java
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

package gmp;

import java.math.BigInteger;
import java.io.*;

/**
 * Simple test for the Java GMP / MPFR binding.
 *
 * <p> Run with: java -ea -esa -Xcheck:jni gmp.Test
 */
public class Test
{

    public static Object testSerialize(Object o) 
    {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            ObjectOutputStream jout = new ObjectOutputStream(out);
            jout.writeObject(o);
            jout.close();
            ByteArrayInputStream in = new ByteArrayInputStream(out.toByteArray());
            ObjectInputStream jin = new ObjectInputStream(in);
            Object oo = jin.readObject();
            jin.close();
            return oo;
        }
        catch (Exception e) {
            System.out.println("caught exception " + e);
            return null;
        }
    }

    /** Calling main performs a few API tests. */
    public static void main(String[] args)
        throws CloneNotSupportedException
    {
        System.out.println("Mpz test");
        System.out.println("========");
        System.out.println("2 -> " + new Mpz(2));
        System.out.println("2.3 -> " + new Mpz(2.3));
        System.out.println("-2.3 -> " + new Mpz(-2.3));

        BigInteger b = BigInteger.valueOf(99);
        for (int i=0;i<4;i++) b = b.multiply(b);
        System.out.println(b + " -> " + new Mpz(b));
        System.out.println(b.negate() + " -> " + new Mpz(b.negate()));

        System.out.println("98765 -> " + new Mpz("98765"));
        System.out.println("-98765 -> " + new Mpz("-98765",10));
        System.out.println(0xdeadbeefL + " -> " + new Mpz("deadbeef",16));

        Mpz x = new Mpz(2);
        x.set(1); System.out.println("1-> " + x);
        x.set(-0.5); System.out.println("-0.5 -> " + x);

        x.fib(128);
        Mpz xx = (Mpz) testSerialize(x);
        System.out.println(x + " -> " + xx);
        assert x.equals(xx) : "Mpz serialization failed";

        assert x.clone().equals(x);

        ///////////////////////////
        System.out.println("");                 
        ///////////////////////////

        System.out.println("Mpq test");
        System.out.println("========");
        System.out.println("1/2 -> " + new Mpq(1,2));
        Mpq y = new Mpq(12,99);
        System.out.println(y + " = " + y.getNum() + " / " + y.getDen() );
        y.numRef().add(2); System.out.println(y);
        y.denRef().sub(2); System.out.println(y);

        y.set(99,132);
        Mpq yy = (Mpq) testSerialize(y);
        System.out.println(y + " -> " + yy);
        assert y.equals(yy) : "Mpq serialization failed";

        //testSerialize(y.numRef());
        //assert yy.numRef().clone().equals(yy.numRef());

        assert y.clone().equals(y);

        ///////////////////////////
        System.out.println("");                 
        ///////////////////////////

        System.out.println("Mpfr test");
        System.out.println("=========");
        System.out.println("version: " + Mpfr.getVersion());
        System.out.println("patches: " + Mpfr.getPatches());
        Mpfr pi = new Mpfr(1024); pi.constPi(Mpfr.RNDN);
        System.out.println("pi " + pi);
        System.out.println("pi " + pi.toString(10,10,Mpfr.RNDN));
        System.out.println("pi " + pi.toString(16,5,Mpfr.RNDN));

        Mpfr pii = (Mpfr) testSerialize(pi);
        System.out.println(pi + " -> " + pii);
        assert pi.equals(pii) : "Mpfr serialization failed";

        assert pi.clone().equals(pi);
   }
}
