/*
 * apronxx_test.cc
 *
 * APRON Library / Test-suite for C++ class wrappers
 *
 * Copyright (C) Antoine Mine' 2007
 *
 */

/* This file is part of the APRON Library, released under GPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include <iostream>
#include <stdio.h>
#include "apronxx.hh"

#include "apxx_box.hh"
#include "apxx_oct.hh"
#include "apxx_polka.hh"
#include "apxx_t1p.hh"

#ifdef HAS_PPL
#include "apxx_ppl.hh"
#include "apxx_ppl_product.hh"
#endif

using namespace std;
using namespace apron;

mpfr_t mpfr1,mpfr2,mpfr3;

void test_scalar()
{
  cout << endl << "scalars" 
       << endl << "=======" << endl << endl;

  // constructors
  cout << "construct 2:              " << scalar(2) << endl;
  cout << "construct 2L:             " << scalar(2L) << endl;
  cout << "construct 2.:             " << scalar(2.) << endl;
  cout << "construct frac(1,5):      " << scalar(frac(1,5)) << endl;
  cout << "construct infty(-1):      " << scalar(infty(-1)) << endl;
  cout << "construct mpq_class(1,5): " << scalar(mpq_class(1,5)) << endl;
  cout << "construct mpfr(1.25):     " << scalar(mpfr1) << endl;

  // assignments 
  scalar s = 12;
  scalar cp = s;  cout << "copy   12:        " << cp << endl;
  cp = 2;         cout << "assign 2:         " << cp << endl;
  cp = 2.;        cout << "assign 2.:        " << cp << endl;
  cp = frac(1,5); cout << "assign frac(1,5): " << cp << endl;
  cp = s;         cout << "copy   12:        " << s  << endl;
  cp = 5;         cout << "before swap 12 5: " << s << " " << cp << endl;
  swap(cp,s);     cout << "after swap  5 12: " << s << " " << cp << endl;
  cp = s = 2;     cout << "chain assign 2:   " << s << " " << cp << endl;
  cp = s = 2.;    cout << "chain assign 2.:  " << s << " " << cp << endl;

  // tests
  cp = s = 12;
  assert(cp==s && cp>=s && cp<=s && !(cp<s) && !(cp>s) && !(cp!=s));
  assert(cp==12 && cp>=11 && cp<=13 && !(4>cp) && !(cp>49));

  // access
  scalar::order o;
  cp = 2;
  assert(cp.get_discr()==AP_SCALAR_MPQ);
  assert(cp.get_mpq()==2);
  assert(cp!=scalar(mpfr1));
  try { cp.get_double(); assert(0); } catch (bad_discriminant& b) {};
  assert(cp.to_mpq(GMP_RNDN,o)==2);
  assert(cp.to_double(GMP_RNDN,o)==2);
  //assert((mpq_class)cp==mpq_class(2));
  assert(double(cp)==2);
  assert(cp!=scalar(mpfr1));
  cp = mpfr1;
  assert(cp.get_discr()==AP_SCALAR_MPFR);
  assert(cp==mpfr1);
  assert(cp!=mpfr2);
  assert(cp==scalar(1.25));
  assert(cp!=scalar(1.26));
  assert(cp==scalar(mpq_class(125,100)));
  assert(cp!=scalar(mpq_class(126,100)));
  cp = 2.;
  assert(cp.get_discr()==AP_SCALAR_DOUBLE);
  assert(cp.get_double()==2);
  try { cp.get_mpq(); assert(0); } catch (bad_discriminant& b) {};
  assert(cp.to_mpq(GMP_RNDN,o)==2);
  assert(cp.to_double(GMP_RNDN,o)==2);
  cp.to_mpfr(mpfr3,GMP_RNDN,o);
  assert(cp==scalar(mpfr3));
  //assert(mpq_class(cp)==mpq_class(2));
  assert(double(cp)==2);
  cp.get_double() = 15;
  cout << "get_double 15.: " << cp << endl;

  // new
  scalar*ps = new scalar(12);
  scalar*pa = new scalar[2];
  pa[1] = *ps;
  cout << "array 12 0 12: " << *ps << " " << pa[0] << " " << pa[1] << endl;
  delete ps;
  delete[] pa;  

  // operations
  scalar a = 12;
  scalar b = -a;
  a.neg();
  cout << "neg -12 -12: " << a << " " << b << endl;
  b = ~a;
  a.inv();
  cout << "inv -1/12 -1/12: " << a << " " << b << endl;  
  printf("print: "); a.print(); printf("\n");
  cout << "hash: " << a.hash() << endl;
}

void test_interval()
{
  cout << endl << "intervals" 
       << endl << "=========" << endl << endl;

  // constructors
  cout << "construct 0:                     " << interval() << endl;
  cout << "construct scalar(12):            " << interval(scalar(12), scalar(15)) << endl;
  cout << "construct [12,15]:               " << interval(12,15) << endl;
  cout << "construct [12L,15L]:             " << interval(12L,15L) << endl;
  cout << "construct [5.,18.]:              " << interval(5.,18.) << endl;
  cout << "construct [frac(1,2),frac(5,4)]: " << interval(frac(1,2),frac(5,4)) << endl;
  cout << "construct [mpq(15,3),15]:        " << interval(mpq_class(15,3),15) << endl;
  cout << "construct [mpfr(1.25),mpfr(99)]: " << interval(mpfr1,mpfr2) << endl;
  cout << "construct top:                   " << interval(top()) << endl;
  cout << "construct bottom:                " << interval(bottom()) << endl;
  
  // assignments 
  interval i = interval(-5,5);
  interval c = i;                    cout << "copy [-5,5]:                " << c << endl;
  c = top();                         cout << "assign top:                 " << c << endl;
  c = bottom();                      cout << "assign bottom:              " << c << endl;
  c = i;                             cout << "copy [-5,5]:                " << c << endl;
  c.set(scalar(5),scalar(19));       cout << "set [scalar(5),scalar(19)]: " << c << endl;
  c.set(12,18);                      cout << "set [12,18]:                " << c << endl;
  c.set(12.,18.);                    cout << "set [12.,18.]:              " << c << endl;
  c.set(frac(1,2),frac(3,4));        cout << "set [frac(1,2),frac(3,4)]:  " << c << endl;
  c.set(mpq_class(8),mpq_class(10)); cout << "set [mpq(8),mpq(10)]:       " <<  c << endl;
c.set(mpfr1,mpfr2);                  cout << "set [mpfr(1.25),mpfr(99)]:  " <<  c << endl;
  c.set(top());                      cout << "set top:                    " << c << endl;
  c.set(bottom());                   cout << "set bottom:                 " << c << endl;
  c.set(i);                          cout << "set interval(-5,5):         " << c << endl;
  i = top();  swap(i,c);             cout << "after swap top [-5,5]:      " << c << " " << i << endl;
  c = i.set(1,2);                    cout << "chain assign [1,2]:         " << c << " " << i << endl;
  c = i = top();                     cout << "chain assign top:           " << c << " " << i << endl;

  // access
  c = interval(1,2);
  c.get_inf() = -1;        cout << "get_inf: [-1,2]:  " << c << endl;
  c.get_sup() = frac(1,2); cout << "get_sup [-1,1/2]: " << c << endl;

  // tests
  assert(interval(top()).is_top());
  assert(!interval(bottom()).is_top());
  assert(interval(scalar(infty(-1)),scalar(infty(1))).is_top());
  assert(!interval(1,2).is_top());
  assert(interval(bottom()).is_bottom());
  assert(!interval(top()).is_bottom());
  assert(interval(5,-10).is_bottom());
  assert(interval(1,2)<=interval(1,3));
  assert(interval(1,4)>interval(2,3));
  assert(interval(1,2)==interval(1,2));
  assert(interval(1,2)==interval(1.,2.));
  assert(interval(scalar(infty(-1)),scalar(infty(1)))==interval(top()));
  assert(interval(5,-10)==interval(bottom()));
  assert(cmp(interval(1,2),interval(1,3))==interval::INCLUDED);
  assert(cmp(interval(1,3),interval(1,3))==interval::EQUAL);
  assert(cmp(interval(0,3),interval(1,3))==interval::CONTAINS);
  assert(cmp(interval(1,3),interval(2,4))==interval::LESS);
  assert(cmp(interval(5,6),interval(1,2))==interval::GREATER);
 
  // new
  interval*pi = new interval(12,13);
  interval*pa = new interval[2];
  pa[1] = *pi;
  cout << "array [12,13] 0 [12,13]: " << *pi << " " << pa[0] << " " << pa[1] << endl;
  delete pi;
  delete[] pa;  

  // operations
  interval a = interval(1,2);
  interval b = -a;
  a.neg();
  cout << "neg [-2,-1] [-2,-1]: " << a << " " << b << endl;
  printf("print: "); a.print(); printf("\n");
  cout << "hash: " << a.hash() << endl;
}

void test_interval_array()
{
  cout << endl << "interval arrays"
       << endl << "===============" << endl << endl;
  interval_array a = interval_array(4);
   a[1] = interval(1,2);
  a.get(2) = interval(1.,2.);
  *(a.contents())[3] = interval(top());
  try { a.get(4); assert(0); } catch (out_of_range& x) {};
  interval_array b = a;
  interval_array c = interval_array(3);
  cout << "construct a:    " << a.size() << " " << a << endl;
  cout << "copy a:         " << b.size() << " " << b << endl;
  cout << "empty:          " << c.size() << " " << c << endl;
  c = a;
  cout << "copy a:         " << c.size() << " " << c << endl;
  vector<interval> v = vector<interval>(3);
  v[0] = interval(1,2);
  interval_array d = v;
  c = v;
  cout << "from vector v:  " << d.size() << " " << d << endl;
  cout << "from vector v:  " << c.size() << " " << c << endl;
  c = b = a;
  cout << "chain assign a: " << c.size() << " " << c << endl;
  vector<interval> vv = d;
  interval_array dd = vv;
  cout << "to vector v:    " << dd.size() << " " << dd << endl;
  printf("print v: "); dd.print(); printf("\n");  
}

void test_coeff()
{
  cout << endl << "coeff" 
       << endl << "=====" << endl << endl;

  // constructors
  cout << "construct 0:                     " << coeff() << endl;
  cout << "construct 2:                     " << coeff(coeff(2)) << endl;
  cout << "construct scalar(frac(2,3)):     " << coeff(scalar(frac(2,3))) << endl;
  cout << "construct 2:                     " << coeff(2) << endl;
  cout << "construct 2L:                    " << coeff(2L) << endl;
  cout << "construct 2.5:                   " << coeff(2.5) << endl;
  cout << "construct frac(2,3):             " << coeff(frac(2,3)) << endl;
  cout << "construct mpq(2,3):              " << coeff(mpq_class(2,3)) << endl;
  cout << "construct mpfr(1.25):            " << coeff(mpfr1) << endl;
  cout << "construct interval(1,2):         " << coeff(interval(1,2)) << endl;
  cout << "construct [scalar(1),scalar(2)]: " << coeff(scalar(1),scalar(2)) << endl;
  cout << "construct [1,2]:                 " << coeff(1,2) << endl;
  cout << "construct [1L,2L]:               " << coeff(1L,2L) << endl;
  cout << "construct [1.1,2.2]:             " << coeff(1.1,2.2) << endl;
  cout << "construct [frac(1,2),frac(3,4)]: " << coeff(frac(1,2),frac(3,4)) << endl;
  cout << "construct [mpq(1,2),mpq(3,4)]:   " << coeff(mpq_class(1,2),mpq_class(3,4)) << endl;
  cout << "construct [mpfr(1.25),mpfr(99)]: " << coeff(mpfr1,mpfr2) << endl;
  cout << "construct top:                   " << coeff(top()) << endl;
  cout << "construct bottom:                " << coeff(bottom()) << endl;

  // assignments
  coeff a;
  a = coeff(2);                         cout << "assign coeff(2):           " << a << endl;
  a = scalar(2);                        cout << "assign scalar(2):          " << a << endl;
  a = 2;                                cout << "assign 2:                  " << a << endl;
  a = 2L;                               cout << "assign 2L:                 " << a << endl;
  a = 2.5;                              cout << "assign 2.5:                " << a << endl;
  a = frac(1,2);                        cout << "assign frac(1,2):          " << a << endl;
  a = mpq_class(1,2);                   cout << "assign mpq(1,2):           " << a << endl;
  a = mpfr1;                            cout << "assign mpfr(1.25):         " << a << endl;
  a = interval(1,2);                    cout << "assign interval(1,2):      " << a << endl;
  a = top();                            cout << "assign top:                " << a << endl;
  a = bottom();                         cout << "assign bottom:             " << a << endl;
  a.set(coeff(2));                      cout << "set coeff(2):              " << a << endl;
  a.set(scalar(2));                     cout << "set scalar(2):             " << a << endl;
  a.set(2);                             cout << "set 2:                     " << a << endl;
  a.set(2L);                            cout << "set 2L:                    " << a << endl;
  a.set(2.);                            cout << "set 2.:                    " << a << endl;
  a.set(frac(1,2));                     cout << "set frac(1,2):             " << a << endl;
  a.set(mpq_class(1,2));                cout << "set mpq(1,2):              " << a << endl;
  a.set(mpfr1);                         cout << "set mpfr(1.25):            " << a << endl;
  a.set(interval(1,2));                 cout << "set interval(1,2):         " << a << endl;
  a.set(scalar(1),scalar(2));           cout << "set [scalar(1),scalar(2)]: " << a << endl;
  a.set(1,2);                           cout << "set [1,2]:                 " << a << endl;
  a.set(1L,2L);                         cout << "set [1L,2L]:               " << a << endl;
  a.set(1.1,2.2);                       cout << "set [1.1,2.2]:             " << a << endl;
  a.set(frac(1,2),frac(3,4));           cout << "set [frac(1,2),frac(3,4)]: " << a << endl;
  a.set(mpq_class(1,2),mpq_class(3,4)); cout << "set [mpq(1,2),mpq(3,4)]:   " << a << endl;
  a.set(mpfr1,mpfr2);                   cout << "set [mpfr(1.25),mpfr(99)]: " << a << endl;
  a.set(top());                         cout << "set top:                   " << a << endl;
  a.set(bottom());                      cout << "set bottom:                " << a << endl;
  a = 2;
  coeff b = 3;
  cout << "before swap 2 3: " << a << " " << b << endl;
  swap(a,b);
  cout << "after swap  3 2: " << a << " " << b << endl;
  a = b = frac(1,2);  cout << "chain assign frac(1,2):      " << a << " " << b << endl;
  a = b.set(1.,2.);   cout << "chain assign [1.,2.]:        " << a << " " << b << endl;

  // access
  coeff x = scalar(2);
  assert(x.get_discr()==AP_COEFF_SCALAR);
  x.get_scalar() = 3; cout << "get_scalar 3: " << x << endl;
  try { x.get_interval(); assert(0); } catch (bad_discriminant& b) {}
  x = interval(1,2);
  assert(x.get_discr()==AP_COEFF_INTERVAL);
  x.get_interval().get_inf() = 0; cout << "get_interval [0,2]: " << x << endl;
  try { x.get_scalar(); assert(0); } catch (bad_discriminant& b) {}

  // tests
  assert(coeff(0).is_zero()); assert(!(coeff(1).is_zero()));
  assert(coeff(0.).is_zero()); assert(!(coeff(1.).is_zero()));
  assert(coeff(0,0).is_zero()); assert(!(coeff(0,1).is_zero()));
  assert(coeff(0.,0.).is_zero()); assert(!(coeff(0.,1.).is_zero()));
#if 0 // overloaded by lincons0
  assert(coeff(0)<=coeff(0,1)); assert(!(coeff(0)<=coeff(1,2)));
  assert(coeff(0,1)>=coeff(1)); assert(!(coeff(0,1)>=coeff(2)));
  assert(coeff(0)<coeff(0,1)); assert(!(coeff(0)<coeff(0)));
  assert(coeff(0)>coeff(bottom())); assert(!(coeff(0)>coeff(top())));
#endif
  assert(coeff(0,1)==coeff(interval(0.,1.)));
  assert(!(coeff(0)==coeff(0,1)));
  assert(coeff(10,-10)==coeff(bottom()));
  assert(coeff(0)!=coeff(0,1));

  // operations
  coeff cc = interval(1,1);
  cout << "before reduce [1,1]: " << cc << endl;
  cc.reduce();
  cout << "after reduce 1:      " << cc << endl;
  cc = interval(3,5);
  coeff dd = -cc;
  cc.neg();
  cout << "neg [-5,-3] [-5,-3]: " << cc << " " << dd << endl;
  cout << "hash: " << cc.hash() << endl;
  printf("print: "); cc.print(); printf("\n");
}

ap_dim_t dim123[] = {1,2,3};
ap_dim_t dim102[] = {1,0,2};
ap_dim_t dim12[] = {1,2};
ap_dim_t dim02[] = {0,2};

void test_dimchange()
{
  cout << endl << "dimchange" 
       << endl << "=========" << endl << endl;
  dimchange d = dimchange(1,2,dim123);
  cout << "from array 1 2 3: " << d;
  d = dim123;
  cout << "from array 3 2 1: " << d;
  cout << d.get_intdim() << " " << d.get_realdim() << " " << d.get(0) << " " << d[1] << endl;
  try { d.get(3); assert(0); } catch (out_of_range& b) {}
  dimchange e = d;
  d[0] = 12;
  cout << "copy 3 2 1: " << e;
  e = d;
  cout << "copy 12 2 1: " << e;
  vector<ap_dim_t> x = vector<ap_dim_t>(3,0);
  x[0] = 4; x[1] = 5; x[2] = 6;
  dimchange f = dimchange(1,2,x);
  cout << "from vector 4 5 6: " << f;
  d = x;
  cout << "from vector 4 5 6: " << d;
  dimchange g = dimchange(1,3);
  try { g = x; assert(0); } catch (invalid_argument& a) {}
  try { dimchange gg = dimchange(1,3,x); assert(0); } catch (invalid_argument& a) {}
  dimchange h = dimchange(d,true);
  dimchange i = -d;
  d.add_invert();
  cout << "invert: " << d;
  cout << "invert: " << h;
  cout << "invert: " << i;
  printf("print: "); d.print();
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << d << varname(vector<string>(0)) << endl;
}

void test_dimperm()
{
  cout << endl << "dimperm" 
       << endl << "=======" << endl << endl;
  dimperm i = id(3);
  cout << "identity:         " << i << endl;
  dimperm d = dimperm(3,dim102);
  cout << "from array 1 0 2: " << d;
  d = id(3);
  cout << "identity:         " << d;  
  d = dim102;
  cout << "from array 1 2 0: " << d;
  cout << d.size() << " " << d.get(0) << " " << d[1] << endl;
  try { d.get(3); assert(0); } catch (out_of_range& b) {}
  dimperm e = d;
  d[0] = 12;
  cout << "copy 1 2 0: " << e;
  e = d;
  cout << "copy 12 2 0: " << e;
  vector<ap_dim_t> x = vector<ap_dim_t>(3,0);
  x[0] = 2; x[1] = 0; x[2] = 1;
  dimperm f = x;
  cout << "from vector 2 0 1: " << f;
  d = x;
  cout << "from vector 2 0 1: " << d;
  dimperm g = dimperm(4);
  g = x;
  cout << "from vector 2 0 1: " << d;
  dimperm h = dimperm(g,true);
  g = -d;
  d.invert();
  cout << "invert 1 2 0:     " << d;
  cout << "invert 1 2 0:     " << g;
  cout << "invert 1 2 0:     " << h;
  g = g*g;
  cout << "compose 2 0 1:     " << g;
  d *= d;
  cout << "compose 2 0 1:     " << d;
  dimperm j = dimperm(d,d);
  cout << "invert 1 2 0:      " << j;
  printf("print: "); j.print();
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << j << varname(vector<string>(0)) << endl;
}


void test_linexpr0()
{
  dimchange dim = dimchange(1,1,dim02);
  dimperm perm  = dimperm(3,dim102);
  dimperm perm2 = -perm;

  // dense
  cout << endl << "dense linexpr0" 
       << endl << "==============" << endl << endl;
  linexpr0 l = linexpr0(AP_LINEXPR_DENSE,5);
  assert(l.get_discr()==AP_LINEXPR_DENSE);
  cout << "construct:                                   " << l.size() << ", " << l << endl;
  l[0] = frac(1,4);
  l[3] = -2.;
  assert(l.is_linear() && l.is_quasilinear());
  l.get_cst() = interval(1,2);
  assert(!l.is_linear() && l.is_quasilinear());
  cout << "get_ 1/4x0 - 2x3 + [1,2]:                    " << l.size() << ", " << l << endl;
  linexpr0 m = l;
  assert(equal(m,l));
  assert(m.get_discr()==AP_LINEXPR_DENSE);
  m[2] = top();
  assert(!equal(m,l));
  cout << "copy 1/4x0 + [-oo,+oo]x2 - 2x3 + [1,2]:      " << m.size() << ", " << m << endl;
  cout << "copy 1/4x0 - 2x3 + [1,2]:                    " << l.size() << ", " << l << endl;
  cout << "[1]: " << l[1] << endl;
  coeff a[] = { 1,2,interval(0,1) };
  linexpr0 n = linexpr0(3,a,2,AP_LINEXPR_DENSE);
  cout << "from array 1x0 + 2x1 + [0,1]x2 + 2:          " << n.size() << ", " << n << endl;  
  m = linexpr0();
  n = m = l;
  cout << "chained copy 1/4x0 - 2x3 + [1,2]:            " << n.size() << ", " << n << endl;  
  assert(equal(m,l));
  l[2] = interval(0,1);
  assert(!equal(m,l));
  cout << "[]   1/4x0 + [0,1]x2 - 2x3 + [1,2]:          " << l.size() << ", " << l << endl;
  cout << "copy 1/4x0 - 2x3 + [1,2]:                    " <<  m.size() << ", " << m << endl;
  swap(m,l);
  assert(!equal(m,l));
  cout << "after swap:                                  " << l.size() << ", " << l << endl;
  cout << "after swap:                                  " << m.size() << ", " << m << endl;
  cout << "const iterator:";
  for (linexpr0::const_iterator i = m.begin();i.valid();++i)
    cout << " " << i.get_coeff();
  cout << endl;
  for (linexpr0::iterator i = m.begin();i.valid();++i)
    i.get_coeff().neg();
  cout << "iterate neg -1/4x0 + [-1,0]x2 + 2x3 + [1,2]: " << m.size() << ", " << m << endl; 
  try { l[5]; assert(0); } catch (out_of_range &r) {}
  l.resize(3);
  cout << "resize 1/4x0 + [1,2]:                        " << l.size() << ", " << l << endl;
  l[0] = interval(3,3);
  cout << "[]     [3,3]x0 + [1,2]:                      " << l.size() << ", " << l << endl;
  l.minimize();
  cout << "minimize 3x0 + [1,2]:                        " << l.size() << ", " << l << endl;
  linexpr0 l2 = linexpr0(l,dim);
  cout << "add dims: 3x1 + [1,2]:                       " << l2 << endl;
  linexpr0 l3 = l;
  l3.add_dimensions(dim);
  cout << "add_dims: 3x1 + [1,2]:                       " << l3 << endl;
  l[1] = interval(3,4);
  linexpr0 l4 = linexpr0(l,perm);
  cout << "permute: [3,4]x0 + 3x1 + [1,2]:              " << l4 << endl;
  l4.permute_dimensions(perm2);
  cout << "permute: 3x0 + [3,4]x1 + [1,2]:              " << l4 << endl;
  cout << "hash: " << l.hash() << endl;
  printf("print: "); l.print(); printf("\n");
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << l << varname(vector<string>(0)) << endl;

  // sparse
  cout << endl << "sparse linexpr0" 
       << endl << "===============" << endl << endl;
  linexpr0 ll = linexpr0(AP_LINEXPR_SPARSE,5);
  assert(ll.get_discr()==AP_LINEXPR_SPARSE);
  cout << "construct:                                   " << ll.size() << ", " << ll << endl;
  ll[0] = frac(1,4);
  ll[3] = -2.;
  assert(ll.is_linear() && ll.is_quasilinear());
  ll.get_cst() = interval(1,2);
  assert(!ll.is_linear() && ll.is_quasilinear());
  cout << "get_ 1/4x0 - 2x3 + [1,2]:                    " << ll.size() << ", " << ll << endl;
  linexpr0 mm = ll;
  assert(equal(mm,ll));
  assert(mm.get_discr()==AP_LINEXPR_SPARSE);
  mm[2] = top();
  assert(!equal(mm,ll));
  cout << "copy 1/4x0 + [-oo,+oo]x2 - 2x3 + [1,2]:      " << mm.size() << ", " << mm << endl;
  cout << "copy 1/4x0 - 2x3 + [1,2]:             :      " << ll.size() << ", " << ll << endl;
  cout << "[1] " << ll[1] << endl;
  mm = linexpr0(AP_LINEXPR_DENSE,5);
  assert(mm.get_discr()==AP_LINEXPR_DENSE);
  coeff aa[] = { 1,2,interval(0,1) };
  linexpr0 nn = linexpr0(3,aa,2,AP_LINEXPR_SPARSE);
  cout << "from array 1x0 + 2x1 + [0,1]x2 + 2:          " << nn.size() << ", " << nn << endl;  
  mm = linexpr0();
  nn = mm = ll;
  cout << "chained copy 1/4x0 - 2x3 + [1,2]:            " << nn.size() << ", " << nn << endl;  
  assert(mm.get_discr()==AP_LINEXPR_SPARSE);
  assert(equal(mm,ll));
  assert(nn.get_discr()==AP_LINEXPR_SPARSE);
  assert(equal(nn,ll));
  ll[2] = interval(0,1);
  assert(!equal(mm,ll));
  cout << "copy 1/4x0 + [0,1]x2 - 2x3 + [1,2]:          " << ll.size() << ", " << ll << endl;
  cout << "copy 1/4x0 - 2x3 + [1,2]:                    " << mm.size() << ", " << mm << endl;
  swap(mm,ll);
  assert(!equal(mm,ll));
  cout << "after swap:                                  " << ll.size() << ", " << ll << endl;
  cout << "after swap:                                  " << mm.size() << ", " << mm << endl;
  cout << "const iterator:";
  for (linexpr0::const_iterator i = mm.begin();i.valid();++i)
    cout << " " << i.get_coeff();
  cout << endl;
  for (linexpr0::iterator i = mm.begin();i.valid();++i)
    i.get_coeff().neg();
  cout << "iterate neg -1/4x0 + [-1,0]x2 + 2x3 + [1,2]: " << mm.size() << ", " << mm << endl; 
  ll[4] = frac(1,2);
  ll[5] = interval(0,1);
  cout << "get_ [] 1/4x0 - 2x3 + 1/2x4 + [0,1]x5 + [1,2]: " << ll.size() << ", " << ll << endl;
  ll[6] = 5; 
  ll[7] = 5;
  ll[1] = 6;
  cout << "get_ [] 1/4x0 + 6x1 - 2x3 + 1/2x4 + [0,1]x5 + 5x6 + 5x7 + [1,2]: " << ll.size() << ", " << ll << endl;
  ll[0] = interval(3,3);
  ll[1] = 0;
  cout << "before minimize:   " << ll.size() << ", " << ll << endl;
  ll.minimize();
  cout << "after minimize:    " << ll.size() << ", " << ll << endl;
  linexpr0 ll2 = linexpr0(ll,dim);
  cout << "add dims 3x1 - 2x5 + 1/2x6 + [0,1]x7 + 5x8 + 5x9 + [1,2]: " << ll2 << endl;
  linexpr0 ll3 = ll;
  ll3.add_dimensions(dim);
  cout << "add_dims 3x1 - 2x5 + 1/2x6 + [0,1]x7 + 5x8 + 5x9 + [1,2]: " << ll3 << endl;
  perm = id(8);
  perm[0] = 3; perm[3] = 0;
  linexpr0 ll4 = linexpr0(ll,perm);
  cout << "permute -2x0 + 3x3 + 1/2x4 + [0,1]x5 + 5x6 + 5x7 + [1,2]: " << ll4 << endl;
  perm2 = -perm;
  ll4.permute_dimensions(perm2);
  cout << "permute 3x0 - 2x3 + 1/2x4 + [0,1]x5 + 5x6 + 5x7 + [1,2]:  " << ll4 << endl;
  cout << "hash: " << ll.hash() << endl;
  printf("print: "); ll.print(); printf("\n");
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << ll << varname(vector<string>(0)) << endl;
}

void test_lincons0()
{
  cout << endl << "lincons0" 
       << endl << "========" << endl << endl;
  coeff a[] = {1,2,3};
  coeff b[] = {4,3,2,1};
  cout << "construct:                               " << lincons0() << endl;
  cout << "construct 1x0 + 2x1 + 3x2 + 4 = 0:       " << lincons0(AP_CONS_EQ,linexpr0(3,a,4)) << endl;
  cout << "construct 1x0 + 2x1 + 3x2 + 4 >= 0:      " << lincons0(AP_CONS_SUPEQ,linexpr0(3,a,4)) << endl;
  cout << "construct 1x0 + 2x1 + 3x2 + 4 > 0:       " << lincons0(AP_CONS_SUP,linexpr0(3,a,4)) << endl;
  cout << "construct 1x0 + 2x1 + 3x2 + 4 != 0:      " << lincons0(AP_CONS_DISEQ,linexpr0(3,a,4)) << endl;
  cout << "construct 1x0 + 2x1 + 3x2 + 4 = 0 mod 5: " << lincons0(AP_CONS_EQMOD,linexpr0(3,a,4),5) << endl;
  lincons0 l = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  lincons0 m = l;
  assert(!l.is_unsat() && l.is_quasilinear() && l.is_linear());
  cout << "copy 1x0 + 2x1 + 3x2 + 4 = 0:            " << m << endl;
  ap_dim_t na[] = {0,2};
  lincons0 n = lincons0(m,dimchange(2,0,na));
  assert(!n.is_unsat() && n.is_quasilinear() && n.is_linear());
  cout << "dimchange 1x1 + 2x2 + 3x4 + 4 = 0:       " << n << endl;
  ap_dim_t oa[] = {1,0,2};
  lincons0 o = lincons0(m,dimperm(3,oa));
  assert(!o.is_unsat() && o.is_quasilinear() && o.is_linear());
  cout << "dimperm 2x0 + 1x1 + 3x2 + 4 = 0:         " << o << endl;
  cout << "unsat:                                   " << lincons0(unsat()) << endl;
  l = lincons0(AP_CONS_SUPEQ,linexpr0(4,b,5));
  cout << "copy 4x0 + 3x1 + 2x2 + 1x3 + 5 >= 0:     " << l << endl;
  l = unsat();
  assert(l.is_unsat());
  cout << "unsat:                                   " << l << endl;
  o = n = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  cout << "chained copy 1x0 + 2x1 + 3x2 + 4 = 0:    " << o << endl;
  o.add_dimensions(dimchange(2,0,na));
  cout << "dimchange 1x1 + 2x2 + 3x4 + 4 = 0:       " << o << endl;
  n.permute_dimensions(dimperm(3,oa));
  cout << "dimperm 2x0 + 1x1 + 3x2 + 4 = 0:         " << n << endl;
  n.resize(2);
  cout << "resize 2x0 + 1x1 + 4 = 0:                " << n << endl;
  assert(n.size()==2);
  assert(n.get_constyp()==AP_CONS_EQ);
  n.get_constyp()=AP_CONS_SUP;
  assert(n.get_constyp()==AP_CONS_SUP);
  cout << "get_constyp 2x0 + 1x1 + 4 > 0:           " << n << endl;  
  assert(!n.has_modulo());
  assert(n.has_linexpr());
  try { n.get_modulo(); assert(0); } catch (invalid_argument& b) {}
  n.set_modulo(2);
  n.set_modulo(3);
  assert(n.has_modulo());
  n.get_constyp()=AP_CONS_EQMOD; 
  cout << "set_modulo 2x0 + 1x1 + 4 = 0 mod 3:      " << n << endl;  
  assert(n.has_modulo());
  assert(n.get_modulo()==3);
  n.set_linexpr(linexpr0(3,a,4));
  cout << "set_linexpr 1x0 + 2x1 + 3x2 + 4 = 0 mod 3:               " << n << endl;  
  n.get_linexpr() = linexpr0(4,b,5);
  cout << "get_linexpr 4x0 + 3x1 + 2x2 + 1x3 + 5 = 0 mod 3:         " << n << endl;
  n.get_linexpr().get_cst() = interval(1,2);
  assert(!n.is_unsat() && !n.is_linear() && n.is_quasilinear());
  cout << "get_linexpr 4x0 + 3x1 + 2x2 + 1x3 + [1,2] = 0 mod 3:     " << n << endl;
  n.get_linexpr()[0] = interval(0,1);
  assert(!n.is_unsat() && !n.is_linear() && !n.is_quasilinear());
  cout << "get_linexpr [0,1]x0 + 3x1 + 2x2 + 1x3 + [1,2] = 0 mod 3: " << n << endl;
  printf("print: "); n.print(); printf("\n");
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << n << varname(vector<string>(0)) << endl;
}

void test_lincons0_array()
{
  cout << endl << "lincons0 array" 
       << endl << "==============" << endl << endl;
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  lincons0_array t = lincons0_array(2);
  try { cout << t; assert(0); } catch (invalid_argument& r) { cout << endl; }
  t[0] = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  t[1] = lincons0(AP_CONS_EQ,linexpr0(2,b,99));
  cout << "construct:    " << t << endl;
  lincons0_array u = t;
  cout << "copy:         " << t << endl; 
  lincons0_array v = lincons0_array(u,dimperm(3,dim102));
  cout << "permutation: " << v << endl;
  lincons0_array w = lincons0_array(u,dimchange(1,1,dim02));
  cout << "dim change:   " << w << endl;
  lincons0 ar[3] = { lincons0(AP_CONS_EQ,linexpr0(3,a,5)), 
		     lincons0(AP_CONS_EQ,linexpr0(2,b,4)),
		     lincons0(AP_CONS_EQMOD,linexpr0(2,b,3),5)  };
  lincons0_array x = lincons0_array(3,ar);
  cout << "from array:  " << x << endl;
  vector<lincons0> ve = vector<lincons0>(2);
  ve[1] = lincons0(AP_CONS_EQ,linexpr0(2,b,99));
  lincons0_array y = lincons0_array(ve);
  cout << "from vector:  " << y << endl;  
  y = ar;
  cout << "from array:   " << y << endl;
  x = ve;
  cout << "from vector:  " << x << endl;  
  w = v;
  cout << "copy:         " << w << endl;
  w.add_dimensions(dimchange(1,1,dim02));
  cout << "dim change:   " << w << endl;
  w = v;
  w.permute_dimensions(dimperm(3,dim102));
  cout << "permutation:  " << w << endl;
  cout << "size:     " << w.size() << endl;
  assert(w.is_linear() && w.is_quasilinear());
  w[0] = lincons0(AP_CONS_SUPEQ,linexpr0(2,b,interval(1,88)));
  b[0] = interval(0,1);
  cout << "[]:           " << w << endl;
  assert(!w.is_linear() && w.is_quasilinear());
  w.get(1) = lincons0(AP_CONS_SUPEQ,linexpr0(2,b,interval(1,88)));
  cout << "get:          " << w << endl;
  assert(!w.is_linear() && !w.is_quasilinear());
  try { w.get(2); assert(0); } catch (out_of_range& b) {}
  *w.contents() = w[1];
  cout << "contents: " << w << endl;
  vector<lincons0> vv = w;
  cout << "to vector:    " << vv.size() << ": " << vv[0] << "; " << vv[1] << endl;
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << vv << varname(vector<string>(0)) << endl;
}

void test_generator0()
{
  cout << endl << "generator0" 
       << endl << "==========" << endl << endl;
  coeff a[] = {1,2,3};
  coeff b[] = {4,3,2,1};
  cout << "construct:                              " << generator0() << endl;
  cout << "construct vert 1x0 + 2x1 + 3x2 + 4:     " << generator0(AP_GEN_VERTEX,linexpr0(3,a,4)) << endl;
  cout << "construct ray 1x0 + 2x1 + 3x2 + 4:      " << generator0(AP_GEN_RAY,linexpr0(3,a,4)) << endl;
  cout << "construct line 1x0 + 2x1 + 3x2 + 4:     " << generator0(AP_GEN_LINE,linexpr0(3,a,4)) << endl;
  cout << "construct modray 1x0 + 2x1 + 3x2 + 4:   " << generator0(AP_GEN_RAYMOD,linexpr0(3,a,4)) << endl;
  cout << "construct modline 1x0 + 2x1 + 3x2 + 4:  " << generator0(AP_GEN_LINEMOD,linexpr0(3,a,4)) << endl;
  generator0 l = generator0(AP_GEN_VERTEX,linexpr0(3,a,4));
  generator0 m = l;
  cout << "copy vert 1x0 + 2x1 + 3x2 + 4:          " << m << endl;
  generator0 n = generator0(m,dimchange(2,0,dim02));
  cout << "dimchange vert 1x1 + 2x2 + 3x4 + 4:     " << n << endl;
  generator0 o = generator0(m,dimperm(3,dim102));
  cout << "dimperm vert 2x0 + 1x1 + 3x2 + 4:       " << o << endl;
  l = generator0(AP_GEN_RAY,linexpr0(4,b,5));
  cout << "copy ray 4x0 + 3x1 + 2x2 + 1x3 + 5:     " << l << endl;
  o = n = generator0(AP_GEN_LINE,linexpr0(3,a,4));
  cout << "chained copy line 1x0 + 2x1 + 3x2 + 4:  " << o << endl;
  o.add_dimensions(dimchange(2,0,dim02));
  cout << "dimchange line 1x1 + 2x2 + 3x4 + 4:     " << o << endl;
  n.permute_dimensions(dimperm(3,dim102));
  cout << "dimperm line 2x0 + 1x1 + 3x2 + 4:       " << n << endl;
  n.resize(2);
  cout << "resize line 2x0 + 1x1 + 4:              " << n << endl;
  assert(n.size()==2);
  assert(n.get_gentyp()==AP_GEN_LINE);
  n.get_gentyp()=AP_GEN_LINEMOD;
  assert(n.get_gentyp()==AP_GEN_LINEMOD);
  cout << "get_gentyp modline 2x0 + 1x1 + 4:      " << n << endl;  
  assert(n.has_linexpr());
  n.set_linexpr(linexpr0(3,a,4));
  cout << "set_linexpr modline 1x0 + 2x1 + 3x2 + 4:  " << n << endl;  
  n.get_linexpr() = linexpr0(4,b,5);
  cout << "get_linexpr modline 4x0 + 3x1 + 2x2 + 1x3 + 5:   " << n << endl;
  n.get_linexpr().get_cst() = 2;
  cout << "get_linexpr modline 4x0 + 3x1 + 2x2 + 1x3 + 2:   " << n << endl;
  n.get_linexpr()[0] = 18;
  cout << "get_linexpr modline 18x0 + 3x1 + 2x2 + 1x3 + 2:  " << n << endl;
  printf("print: "); n.print(); printf("\n");
   string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << n << varname(vector<string>(0)) << endl;
}

void test_generator0_array()
{
  cout << endl << "generator0 array" 
       << endl << "================" << endl << endl;
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  generator0_array t = generator0_array(2);
  try { cout << t; assert(0); } catch (invalid_argument& r) { cout << endl; }
  t[0] = generator0(AP_GEN_VERTEX,linexpr0(3,a,4));
  t[1] = generator0(AP_GEN_VERTEX,linexpr0(2,b,99));
  cout << "construct:    " << t << endl;
  generator0_array u = t;
  cout << "copy:         " << t << endl; 
  generator0_array v = generator0_array(u,dimperm(3,dim102));
  cout << "permutation: " << v << endl;
  generator0_array w = generator0_array(u,dimchange(1,1,dim02));
  cout << "dim change:   " << w << endl;
  generator0 ar[3] = { generator0(AP_GEN_VERTEX,linexpr0(3,a,5)), 
		       generator0(AP_GEN_VERTEX,linexpr0(2,b,4)),
		       generator0(AP_GEN_RAY,linexpr0(2,b,3))  };
  generator0_array x = generator0_array(3,ar);
  cout << "from array:  " << x << endl;
  vector<generator0> ve = vector<generator0>(2);
  ve[1] = generator0(AP_GEN_VERTEX,linexpr0(2,b,99));
  generator0_array y = generator0_array(ve);
  cout << "from vector:  " << y << endl;  
  y = ar;
  cout << "from array:   " << y << endl;
  x = ve;
  cout << "from vector:  " << x << endl;  
  w = v;
  cout << "copy:         " << w << endl;
  w.add_dimensions(dimchange(1,1,dim02));
  cout << "dim change:   " << w << endl;
  w = v;
  w.permute_dimensions(dimperm(3,dim102));
  cout << "permutation:  " << w << endl;
  cout << "size:     " << w.size() << endl;
  w[0] = generator0(AP_GEN_LINE,linexpr0(2,b,88));
  b[0] = 99;
  cout << "[]:           " << w << endl;
  w.get(1) = generator0(AP_GEN_RAY,linexpr0(2,b,88));
  cout << "get:          " << w << endl;
  try { w.get(2); assert(0); } catch (out_of_range& b) {}
  *w.contents() = w[1];
  cout << "contents: " << w << endl;
  vector<generator0> vv = w;
  cout << "to vector:    " << vv.size() << ": " << vv[0] << "; " << vv[1] << endl;
  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << vv << varname(vector<string>(0)) << endl;
}

void test_texpr0()
{
  cout << endl << "texpr0"
       << endl << "======" << endl << endl;
  coeff a[] = {1,2,3};

  // constructors
  cout << "construct 2:              " << texpr0(2) << endl;
  cout << "construct 2L:             " << texpr0(2L) << endl;
  cout << "construct 2.2:            " << texpr0(2.2) << endl;
  cout << "construct mpq(1,2):       " << texpr0(mpq_class(1,2)) << endl;
  cout << "construct mpfr(1.25):     " << texpr0(mpfr1) << endl;
  cout << "construct coeff(2):       " << texpr0(coeff(2)) << endl;
  cout << "construct scalar(2):      " << texpr0(scalar(2)) << endl;
  cout << "construct 1/2:            " << texpr0(frac(1,2)) << endl;
  cout << "construct interval(1,2):  " << texpr0(interval(1,2)) << endl;
  cout << "construct [2.2,3.3]:      " << texpr0(interval(2.2,3.3)) << endl;
  cout << "construct [1/2,3/4]:      " << texpr0(interval(frac(1,2),frac(3,4))) << endl;
  cout << "construct top:            " << texpr0(top()) << endl;
  cout << "construct x2:             " << texpr0(dim(2)) << endl;
  cout << "construct +x2:            " << texpr0(+dim(2)) << endl;
  cout << "construct -x2:            " << texpr0(-dim(2)) << endl;
  cout << "construct x2+2:           " << texpr0(dim(2)+2) << endl;
  cout << "construct x2-2:           " << texpr0(dim(2)-2) << endl;
  cout << "construct x2*2:           " << texpr0(dim(2)*2) << endl;
  cout << "construct x2/2:           " << texpr0(dim(2)/2) << endl;
  cout << "construct x2%2:           " << texpr0(dim(2)%2) << endl;
  cout << "construct sqrt(x2):       " << texpr0(sqrt(dim(2))) << endl;
  cout << "construct float -x2:      " << texpr0(neg(dim(2),AP_RTYPE_SINGLE)) << endl;
  cout << "construct float x2+2:     " << texpr0(add(dim(2),2,AP_RTYPE_SINGLE)) << endl;
  cout << "construct float x2-2:     " << texpr0(sub(dim(2),2,AP_RTYPE_SINGLE)) << endl;
  cout << "construct float x2*2:     " << texpr0(mul(dim(2),2,AP_RTYPE_SINGLE)) << endl;
  cout << "construct float x2/2:     " << texpr0(div(dim(2),2,AP_RTYPE_SINGLE)) << endl;
  cout << "construct float x2%2:     " << texpr0(mod(dim(2),2,AP_RTYPE_SINGLE)) << endl;
  cout << "construct float sqrt(x2): " << texpr0(sqrt(dim(2),AP_RTYPE_SINGLE)) << endl;
  cout << "construct float cast(x2): " << texpr0(cast(dim(2),AP_RTYPE_SINGLE)) << endl;
  cout << "construct floor(x2):      " << texpr0(floor(dim(2))) << endl;
  cout << "construct ceil(x2):       " << texpr0(ceil(dim(2))) << endl;
  cout << "construct trunc(x2):      " << texpr0(trunc(dim(2))) << endl;
  cout << "construct linexpr:        " << texpr0(linexpr0(3,a,4)) << endl;

  texpr0 x = 2+3*dim(0);
  texpr0 y = x;
  cout << "copy x=2+3*x0:            " << y << endl;
  cout << "construct (x+x)/(x*x):    " << (x+x)/(x*x) << endl;
  cout << "construct x0 + 2:              " << dim(0)+2 << endl;
  cout << "construct x0 + 2L:             " << dim(0)+2L << endl;
  cout << "construct x0 + 2.2:            " << dim(0)+2.2 << endl;
  cout << "construct x0 + mpq(1,2):       " << dim(0)+mpq_class(1,2) << endl;
  cout << "construct x0 + mpfr(1.25):     " << dim(0)+mpfr1 << endl;
  cout << "construct x0 + coeff(2):       " << dim(0)+coeff(2) << endl;
  cout << "construct x0 + scalar(2):      " << dim(0)+scalar(2) << endl;
  cout << "construct x0 + 1/2:            " << dim(0)+frac(1,2) << endl;
  cout << "construct x0 + interval(1,2):  " << dim(0)+interval(1,2) << endl;
  cout << "construct x0 + [2,3]:          " << dim(0)+texpr0::builder(2,3) << endl;
  cout << "construct x0 + [2L,3L]:        " << dim(0)+texpr0::builder(2L,3L) << endl;
  cout << "construct x0 + [2.2,3.3]:      " << dim(0)+texpr0::builder(2.2,3.3) << endl;
  cout << "construct x0 + [1/2,3/4]:      " << dim(0)+texpr0::builder(frac(1,2),frac(3,4)) << endl;
  cout << "construct x0 + top:            " << dim(0)+top() << endl;
  cout << "construct x0 + x2:             " << dim(0)+dim(2) << endl;
  cout << "construct x0 + linexpr:        " << dim(0)+texpr0(linexpr0(3,a,4)) << endl;

  try { unary(AP_TEXPR_ADD,2); assert(0); } catch (invalid_argument& a) {}
  try { binary(AP_TEXPR_NEG,1,2); assert(0); } catch (invalid_argument& a) {}

  // assignments
  y = 2; cout << "assign 2:              " << y << endl;
  y = 2L; cout << "assign 2L:             " << y << endl;
  y = 2.2; cout << "assign 2.2:            " << y << endl;
  y = mpq_class(1,2); cout << "assign mpq(1,2):       " << y << endl;
  y = mpfr1; cout << "assign mpfr(1.25):     " << y << endl;
  y = coeff(2); cout << "assign coeff(2):       " << y << endl;
  y = scalar(2); cout << "assign scalar(2):      " << y << endl;
  y = interval(1,2); cout << "assign interval(1,2):  " << y << endl;
  y = top(); cout << "assign top:            " << y << endl;
  y = dim(2); cout << "assign x2:             " << y << endl;
  y = linexpr0(3,a,4); cout << "assign linexpr:        " << y << endl;
  y = x; 
  cout << "assign x:              " << y << endl;
  y = x+1; cout << "assign x+1:            " << y << endl;
  x = x*x; cout << "assign x*x:            " << x << endl;
  x = y = sqrt(dim(0)+1); cout << "chained assign sqrt(x0+1): " << x << "; " << y << endl;

  // dimensions
  dimchange dimc = dimchange(1,1,dim02);
  dimperm perm  = dimperm(3,dim102);
  x = dim(0)/dim(1);
  cout << "add dim x1/x2:         " << texpr0(x,dimc) << endl;
  cout << "del dim ]-oo;+oo[/x0:  " << texpr0(x,dimc,false) << endl;
  cout << "dim perm x1/x0:        " << texpr0(x,perm) << endl;
  x.add_dimensions(dimc);
  cout << "add dim x1/x2:         " << x << endl;
  x.remove_dimensions(-dimc);
  cout << "del dim x0/x1:         " << x << endl;
  x.permute_dimensions(perm);
  cout << "dim perm x1/x0:        " << x << endl; 
  y = texpr0(x,perm)+texpr0(x,dimc,false);
  cout << "dim mix x0/x1+x0/]-oo;+oo[:  " << y << endl;

  // access && tests
  x = 2;
  assert(x.max_dim()==0);
  assert(x.depth()==0);
  assert(x.size()==0);
  assert(x.equal(texpr0(2)));
  assert(!x.equal(texpr0(3)) && !x.equal(texpr0(dim(3))) && !x.equal(dim(4)+dim(3)));
  assert(x.root().get_discr()==AP_TEXPR_CST);
  assert(x.is_interval_cst() && x.is_interval_linear() && x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && x.is_scalar());
  try { x.root().get_dim(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_op(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_rtype(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_rdir(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().left(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().right(); assert(0); } catch (bad_discriminant& b) {}
  x.root().get_coeff() = interval(1,2); cout << "get_coeff [1,2]: " << x << endl;
  assert(!x.equal(texpr0(2)) && x.equal(texpr0(interval(1,2))));
  assert(x.is_interval_cst() && x.is_interval_linear() && x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && !x.is_scalar());
  x = dim(2);
  assert(x.max_dim()==3);
  assert(x.depth()==0);
  assert(x.size()==0);
  assert(x.equal(texpr0(dim(2))));
  assert(!x.equal(texpr0(3)) && !x.equal(texpr0(dim(3))) && !x.equal(texpr0(dim(4)+dim(3))));
  assert(x.root().get_discr()==AP_TEXPR_DIM);
  assert(!x.is_interval_cst() && x.is_interval_linear() && x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && x.is_scalar());
  try { x.root().get_coeff(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_op(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_rtype(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_rdir(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().left(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().right(); assert(0); } catch (bad_discriminant& b) {}
  x.root().get_dim() = 3;   cout << "get_dim 3: " << x << endl;
  assert(!x.equal(texpr0(dim(2))) && x.equal(texpr0(dim(3))));
  x = add(dim(0),1,AP_RTYPE_SINGLE,AP_RDIR_NEAREST);
  assert(!x.is_interval_cst() && !x.is_interval_linear() && !x.is_interval_polynomial() && 
	 !x.is_interval_polyfrac() && x.is_scalar());
  assert(x.depth()==1);
  assert(x.size()==1);
  assert(x.equal(texpr0(add(dim(0),1,AP_RTYPE_SINGLE,AP_RDIR_NEAREST))));
  assert(!x.equal(texpr0(3)) && !x.equal(texpr0(dim(3))) && !x.equal(texpr0(dim(4)+dim(3))));
  assert(x.root().get_discr()==AP_TEXPR_NODE);
  try { x.root().get_coeff(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_dim(); assert(0); } catch (bad_discriminant& b) {}
  assert(x.root().get_op()==AP_TEXPR_ADD);
  assert(is_binop(x.root().get_op()));
  assert(x.root().get_rtype()==AP_RTYPE_SINGLE);
  assert(x.root().get_rdir()==AP_RDIR_NEAREST);
  assert(x.root().left().equal(texpr0(dim(0))));
  assert(x.root().right().equal(texpr0(1)));
  x.root().get_op()=AP_TEXPR_SUB;
  x.root().get_rtype()=AP_RTYPE_DOUBLE;
  x.root().get_rdir()=AP_RDIR_ZERO;
  x.root().left()=2;
  x.root().right()=dim(1);
  cout << "get_op rtype rdir argA argB: " << x << endl;
  assert(!x.equal(add(dim(0),1,AP_RTYPE_SINGLE,AP_RDIR_NEAREST)));
  assert(x.equal(sub(2,dim(1),AP_RTYPE_DOUBLE,AP_RDIR_ZERO)));
  x.root().get_rtype()=AP_RTYPE_REAL;
  assert(!x.is_interval_cst() && x.is_interval_linear() && x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && x.is_scalar());
  x.root().get_op()=AP_TEXPR_MUL;
  assert(!x.is_interval_cst() && x.is_interval_linear() && x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && x.is_scalar());
  x.root().get_op()=AP_TEXPR_DIV;
  assert(!x.is_interval_cst() && !x.is_interval_linear() && !x.is_interval_polynomial() && 
	 x.is_interval_polyfrac() && x.is_scalar());
  x = sqrt(dim(1)+5,AP_RTYPE_SINGLE,AP_RDIR_NEAREST);
  assert(!x.is_interval_cst() && !x.is_interval_linear() && !x.is_interval_polynomial() && 
	 !x.is_interval_polyfrac() && x.is_scalar());
  assert(x.depth()==2);
  assert(x.max_dim()==2);
  assert(!x.equal(texpr0(3)) && !x.equal(texpr0(dim(3))) && !x.equal(texpr0(dim(4)+dim(3))));
  assert(x.equal(sqrt(dim(1)+5,AP_RTYPE_SINGLE,AP_RDIR_NEAREST)));
  assert(x.root().get_discr()==AP_TEXPR_NODE);
  try { x.root().get_coeff(); assert(0); } catch (bad_discriminant& b) {}
  try { x.root().get_dim(); assert(0); } catch (bad_discriminant& b) {}
  assert(x.root().get_op()==AP_TEXPR_SQRT);
  assert(is_unop(x.root().get_op()));
  assert(x.root().get_rtype()==AP_RTYPE_SINGLE);
  assert(x.root().get_rdir()==AP_RDIR_NEAREST);
  assert(x.root().left().left().equal(texpr0(dim(1))));
  assert(x.root().left().right().equal(texpr0(5)));
  try { x.root().right(); assert(0); } catch (out_of_range& b) {}  
  cout << "get: " << x << endl;
  x.root().get_op()=AP_TEXPR_CAST;
  x.root().get_rtype()=AP_RTYPE_DOUBLE;
  x.root().get_rdir()=AP_RDIR_ZERO;
  x.root().left()=2+dim(3);
  cout << "set: " << x << endl;
  assert(!x.equal(sqrt(dim(3),AP_RTYPE_SINGLE,AP_RDIR_NEAREST)));
  assert(x.equal(cast(2+dim(3),AP_RTYPE_DOUBLE,AP_RDIR_ZERO)));
  assert(x.size()==2);
  assert(x.depth()==2);
  assert(x.max_dim()==4);
  x = dim(0)+dim(5)+dim(2);
  assert(x.has_dim(0) && !x.has_dim(1) && x.has_dim(2) && !x.has_dim(3) && 
	 !x.has_dim(4) && x.has_dim(5) && !x.has_dim(6));
  vector<ap_dim_t> dm = x.dimlist();
  assert(dm.size()==3 && dm[0]==0 && dm[1]==2 && dm[2]==5);
  cout << x << endl;
  texpr0 z(x.root().left());
  cout << "iterator -> texpr0 x0+x5:           " << z << endl;
  z = x.root().right();
  cout << "iterator assign x2:                 " << z << endl;
  cout << "construct from iterators x0+x5-x2:  " << x.root().left() - x.root().right() << endl;

  // substitutions
  x = sqrt(dim(0)+2*dim(1)*dim(1));
  cout << "subst: " << texpr0(x,0,dim(0)+1) << endl;
  assert(x.equal(sqrt(dim(0)+2*dim(1)*dim(1))));
  x.substitute(1,sqrt(dim(0))+1);
  cout << "subst: " << x << endl;
  assert(x.equal(sqrt(dim(0)+2*(sqrt(dim(0))+1)*(sqrt(dim(0))+1))));

  // hash & print
  x = sqrt(dim(0)+2*dim(1)*dim(1));
  cout << "hash: " << x.hash() << endl;
  printf("print: "); x.print(); printf("\n");

  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << x << varname(vector<string>(0)) << endl;
}


void test_tcons0()
{
  cout << endl << "tcons0" 
       << endl << "======" << endl << endl;
  coeff a[] = {1,2,3};
  lincons0 ll = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  try { cout <<  tcons0(); assert(0); } catch (invalid_argument& r) {}
  cout << "construct x0 + 1 = 0:                " << tcons0(AP_CONS_EQ,dim(0)+1) << endl;
  cout << "construct floor(x0+2) >= 0:          " << tcons0(AP_CONS_SUPEQ,floor(dim(0)+2)) << endl;
  cout << "construct x0*x1 > 0:                 " << tcons0(AP_CONS_SUP,dim(0)*dim(1)) << endl;
  cout << "construct [-1,1]*x0 != 0:            " << tcons0(AP_CONS_DISEQ,interval(-1,1)*dim(0)) << endl;
  cout << "construct x0/x1 = 0 mod 5:           " << tcons0(AP_CONS_EQMOD,dim(0)/dim(1),5) << endl;
  cout << "from lincons 1x0+2x1+3x2+4 = 0:      " << tcons0(ll) << endl;
  cout << "construct x0 >= x2+1:                " << (dim(0)>=dim(2)+1) << endl;
  cout << "construct x0 <= x2+1:                " << (dim(0)<=dim(2)+1) << endl;
  cout << "construct x0 >  x2+1:                " << (dim(0)>dim(2)+1) << endl;
  cout << "construct x0 <  x2+1:                " << (dim(0)<dim(2)+1) << endl;
  cout << "construct x0 == x2+1:                " << (dim(0)==dim(2)+1) << endl;
  cout << "construct x0 != x2+1:                " << (dim(0)!=dim(2)+1) << endl;
  cout << "construct x0 == 0:                   " << (dim(0)==0) << endl;
  cout << "construct 0 != x2+1:                 " << (0!=dim(2)+1) << endl;
  tcons0 l = tcons0(AP_CONS_EQ,dim(0)*dim(2));
  tcons0 m = l;
  assert(!l.is_interval_cst() && !l.is_interval_linear() && l.is_interval_polynomial() && 
	 l.is_interval_polyfrac() && l.is_scalar());
  cout << "copy x0 * x2 = 0:                    " << m << endl;
  tcons0 n = tcons0(m,dimchange(2,0,dim02));
  cout << "dimchange x1 * x4 = 0:               " << n << endl;
  tcons0 o = tcons0(m,dimperm(3,dim102));
  cout << "dimperm x1 * x2 = 0:                 " << o << endl;
  cout << "unsat:                               " << tcons0(unsat()) << endl;
  l = tcons0(AP_CONS_SUPEQ,dim(0));
  l = unsat();
  cout << "unsat:                               " << l << endl;
  l = ll;
  cout << "from lincons 1x0+2x1+3x2+4 = 0:      " << l << endl;
  assert(!l.is_interval_cst() && l.is_interval_linear() && l.is_interval_polynomial() && 
	 l.is_interval_polyfrac() && l.is_scalar());
  o = n = tcons0(AP_CONS_EQ,dim(0)*dim(2)+2);
  cout << "chained copy x0 * x2 +2 = 0:         " << o << endl;
  o.add_dimensions(dimchange(2,0,dim02));
  cout << "dimchange x1 * x4 + 2 = 0:           " << o << endl;
  o = tcons0(AP_CONS_EQ,dim(0)*dim(3)+2);
  o.remove_dimensions(dimchange(2,0,dim02));
  cout << "dimchange ]-oo;+oo[ * x1 + 2 = 0:    " << o << endl;
  n.permute_dimensions(dimperm(3,dim102));
  cout << "dimperm x1 * x2 + 2 = 0:              " << n << endl;
  assert(n.get_constyp()==AP_CONS_EQ);
  n.get_constyp()=AP_CONS_SUP;
  assert(n.get_constyp()==AP_CONS_SUP);
  cout << "get_constyp x1 * x2 + 2 > 0:         " << n << endl;  
  assert(!n.has_modulo());
  assert(n.has_texpr());
  try { n.get_modulo(); assert(0); } catch (invalid_argument& b) {}
  n.set_modulo(2);
  n.set_modulo(3);
  assert(n.has_modulo());
  n.get_constyp()=AP_CONS_EQMOD; 
  cout << "set_modulo x1 * x2 + 2 = 0 mod 3:    " << n << endl;  
  assert(!n.is_interval_cst() && !n.is_interval_linear() && l.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && n.is_scalar());
  assert(n.has_modulo());
  assert(n.get_modulo()==3);
  n.set_texpr(sqrt(dim(0)+1));
  cout << "set_texpr sqrt(x0+1) = 0 mod 3:      " << n << endl;  
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 !n.is_interval_polyfrac() && n.is_scalar());
  n.get_texpr() = dim(0)/dim(1)+5;
  cout << "get_texpr x0/x1+5 = 0 mod 3:         " << n << endl;
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && n.is_scalar());
  n.get_texpr().left().left() = interval(0,1);
  cout << "get_texpr [0,1]/x1+5 = 0 mod 3:      " << n << endl;
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && !n.is_scalar());
  printf("print: "); n.print(); printf("\n");

  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << n << varname(vector<string>(0)) << endl;
}


void test_tcons0_array()
{
  cout << endl << "tcons0 array" 
       << endl << "============" << endl << endl;
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  tcons0_array t = tcons0_array(2);
  try { cout << t; assert(0); } catch (invalid_argument& r) { cout << endl; }
  t[0] = tcons0(AP_CONS_SUPEQ,sqrt(dim(0)+15*dim(2)));
  t[1] = tcons0(AP_CONS_EQ,interval(2,5));
  cout << "construct:    " << t << endl;
  tcons0_array u = t;
  cout << "copy:         " << t << endl; 
  tcons0_array v = tcons0_array(u,dimperm(3,dim102));
  cout << "permutation:  " << v << endl;
  tcons0_array w = tcons0_array(u,dimchange(1,1,dim02));
  cout << "add dim:      " << w << endl;
  tcons0_array ww = tcons0_array(u,dimchange(1,1,dim02),false);
  cout << "rem dim:      " << ww << endl;
  tcons0 ar[3] = { tcons0(AP_CONS_SUPEQ,dim(0)*dim(1)*dim(2)),
		   tcons0(AP_CONS_EQ,dim(0)%dim(1)),
		   tcons0(AP_CONS_EQMOD,interval(1,2)*dim(0),5)  };
  tcons0_array x = tcons0_array(3,ar);
  cout << "from array:   " << x << endl;
  vector<tcons0> ve = vector<tcons0>(2);
  ve[0] = tcons0(AP_CONS_EQ,0);
  ve[1] = tcons0(AP_CONS_EQ,1-dim(5));
  tcons0_array y = tcons0_array(ve);
  cout << "from vector:  " << y << endl;  
  y = ar;
  cout << "from array:   " << y << endl;
  x = ve;
  cout << "from vector:  " << x << endl;  
  w = u;
  cout << "copy:         " << w << endl;
  w.add_dimensions(dimchange(1,1,dim02));
  cout << "add dim:      " << w << endl;
  w = u;
  w.remove_dimensions(dimchange(1,1,dim02));
  cout << "del dim:      " << w << endl;
  w = u;
  w.permute_dimensions(dimperm(3,dim102));
  cout << "permutation:  " << w << endl;
  cout << "size:     " << w.size() << endl;
  assert(!w.is_interval_linear());
  w[0] = tcons0(AP_CONS_SUPEQ,dim(1)*5+dim(2));
  b[0] = interval(0,1);
  cout << "[]:           " << w << endl;
  assert(w.is_interval_linear());
  w.get(1) = tcons0(AP_CONS_SUPEQ,dim(0)*dim(2));
  cout << "get:          " << w << endl;
  try { w.get(2); assert(0); } catch (out_of_range& b) {}
  *w.contents() = w[1];
  cout << "contents: " << w << endl;
  vector<tcons0> vv = w;
  cout << "to vector:    " << vv.size() << ": " << vv[0] << "; " << vv[1] << endl;

  // from lincons0_array
  lincons0_array lt = lincons0_array(2);
  lt[0] = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  lt[1] = lincons0(AP_CONS_EQ,linexpr0(2,b,99));
  tcons0_array A = lt;
  cout << "from lincons0_array: " << A << endl;
  v = lt;
  cout << "from lincons0_array: " << v << endl;    

  // print
  printf("print: "); w.print(); printf("\n");

  string nms[4] ={ "x", "y", "z", "t" };
  cout << "print w/ names: " << varname(vector<string>(nms,nms+4)) << w << varname(vector<string>(0)) << endl;
}

void test_var()
{
  cout << endl << "var" 
       << endl << "===" << endl << endl;
  var v1 = "v1";
  printf("print: "); v1.print(); printf("\n");
  cout << "from char*: " << v1 << endl;  
  var v2 = string("v2");
  cout << "from string: " << v2 << endl;  
  var v3 = v2;
  cout << "copy constructor: " << v3 << endl;  
  v1 = v2;    cout << "copy assignment: " << v1 << endl;  
  v1 = "aa";  cout << "char* assignment: " << v1 << endl;  
  v1 = string("aa");  cout << "string assignment: " << v1 << endl;
  char* s = v1; cout << "to char*: " << s << endl; free(s);
  string ss = v1; cout << "to string: " << ss << endl;
  v1 = "a"; v2 = "a"; v3 = "b";
  assert(v1==v2 && !(v1!=v2) && v1<=v2 && v1>=v2 && !(v1<v2) && !(v1>v2));
  assert(v1!=v3 && !(v1==v3) && v1<=v3 && !(v1>=v3) && v1<v3 && ! (v1>v3));
}

void test_environment()
{
  cout << endl << "environment" 
       << endl << "===========" << endl << endl;
  var ai[3] = { "i1","i3","i2"  };
  var af[3] = { "f1","f3","f2"  };
  var ag[2] = { "f1","x"  };
  var ah[2] = { "j","h"  };
  vector<var> vi = vector<var>(2,"j"); vi[1] = "h";
  vector<var> vf = vector<var>(2,"g"); vf[1] = "e";
  vector<var> vn = vector<var>();
  cout << "empty:       " << endl << environment() << endl;
  cout << "from vector: " << endl << environment(vi,vf) << endl;
  cout << "from array:  " << endl << environment(ai,3,af,3) << endl;

  environment e(vi,vf);
  { environment f(ai,3,af,3); e = f; } cout << "copy assignment: " << endl << e << endl;

  environment* pf = new environment(ai,3,af,3);
  environment f = *pf;
  delete pf;
  cout << "copy constructor: " << endl << f << endl;

  e = environment(vi,vn);
  try { e.add(vi,vn); assert(0); } catch (invalid_argument& c) {}
  f = e.add(ai,3,af,3);
  cout << "add array:" << endl << f << endl;
  try { f.remove(vf); assert(0); } catch (invalid_argument& c) {}
  cout << "del vector:" << endl << f.remove(vi) << endl;

  e = environment(ai,3,af,3);
  try { e.add(ai,3,NULL,0); assert(0); } catch (invalid_argument& c) {}
  f = e.add(vi,vn);
  cout << "add vector:" << endl << f << endl;
  try { f.remove(ag,2); assert(0); } catch (invalid_argument& c) {}
  cout << "del array:" << endl << f.remove(ai,3) << endl;

  dimperm p;
  e = environment(vi,vn);
  try { e.add(vi,vn,p); assert(0); } catch (invalid_argument& c) {}
  cout << "add vector w/ dimperm:" << endl << e.add(ai,3,af,3,p) << p << endl;

  e = environment(ai,3,af,3);
  try { e.add(ai,3,NULL,0,p); assert(0); } catch (invalid_argument& c) {}
  cout << "add vector w/ dimperm:" << endl << e.add(vi,vn,p) << p << endl;

  e = environment(ai,3,af,3);
  vector< pair<var,var> > vr(2,pair<var,var>("i2","a"));
  vr[1].first = "f2"; vr[1].second = "z";
  try { environment(ai,3,NULL,0).rename(vr); } catch (invalid_argument& c) {} 
  f = e.rename(vr);
  cout << "rename vector:" << endl << f << endl;

  var ar1[2] = { "i2", "f2" }, ar2[2] = { "a", "z" };
  try { environment(ai,3,NULL,0).rename(ar1,ar2,2); } catch (invalid_argument& c) {} 
  f = e.rename(ar1,ar2,2);
  cout << "rename array:" << endl << f << endl;

  try { environment(ai,3,NULL,0).rename(vr,p); } catch (invalid_argument& c) {} 
  f = e.rename(vr,p);
  cout  << "rename vector w/ dimperm:" << endl << f << p << endl;

  try { environment(ai,3,NULL,0).rename(ar1,ar2,2,p); } catch (invalid_argument& c) {} 
  f = e.rename(ar1,ar2,2,p);
  cout << "rename array w/ dimperm:" << endl << f << p << endl;
 
  dimchange x,y;
  e = environment(ai,3,af,3);
  f = environment(ah,2,ag,2);
  try { lce(e,environment(ag,2,ah,2)); assert(0); } catch (invalid_argument& c) {} 
  cout << "lub:" << endl << lce(e,f) << endl;
  try { lce(e,environment(ag,2,ah,2),x,y); assert(0); } catch (invalid_argument& c) {} 
  cout << "lub w/ dimchange:" << endl << lce(e,f,x,y); cout << x << y << endl;

  vector<environment> ve(2);
  vector<dimchange> vd(2);
  ve[0] = environment(ai,3,af,3);
  ve[1] = environment(ah,2,ag,2);
  cout << "n-ary vector lce:" << endl << lce(ve) << endl;
  cout << "n-ary vector lce w/ dimchange:" << endl << lce(ve,vd); 
  for (size_t i=0;i<2;i++) cout << vd[i];
  cout << endl;

  environment ae[2] = { environment(ai,3,af,3), environment(ah,2,ag,2) };
  dimchange ad[2];
  cout << "n-ary array lce:" << endl << lce(ae,2) << endl;
  cout << "n-ary array lce w/ dimchange:" << endl << lce(ae,ad,2); 
  for (size_t i=0;i<2;i++) cout << ad[i];
  cout << endl;

  e = environment(ai,3,af,2);
  assert(e.intdim()==3 && e.realdim()==2);
  assert(e.contains("i1") && e.contains("f3")  && !e.contains("f2"));
  assert(e[1]==var("i2") && !(e[1]==var("i3")) && e[1]!=var("i3") && !(e[1]!=var("i2")));
  assert(e["i1"]==0 && e["i2"]==1);
  try { e["i4"]; assert(0); } catch (invalid_argument &c) {};
  assert(e.get_var(1)==var("i2") && !(e.get_var(1)==var("i3")) && e.get_var(1)!=var("i3") && !(e.get_var(1)!=var("i2")));
  assert(e.get_dim("i1")==0 && e.get_dim("i2")==1);
  try { e.get_dim("i4"); assert(0); } catch (invalid_argument &c) {};
  try { e.get_var(5); assert(0); } catch (out_of_range &c) {};
  vector<var> vv = e.get_vars();
  cout << "iterator: ";
  for (size_t i=0;i<vv.size();i++) cout << vv[i] << " ";
  cout << endl << endl;

  e = environment(ai,3,af,2);
  f = environment(ai,3,af,3);
  assert(e==e && e!=f && !(e==f) && !(e!=e));
  assert(e<=e && e<=f && !(f<=e));
  assert(e>=e && !(e>=f) && f>=e);
  assert(cmp(e,e)==0 && cmp(e,f)==-1 && cmp(f,e)==1);
  cout << "dimchange to super:" << endl << get_dimchange(e,f) << endl;
  f = environment(ai,2,af,3);
  assert(e!=f && !(e==f) && !(e<=f) && !(e>=f));
  assert(cmp(e,f)==2);
  try { get_dimchange(e,f); assert(0); } catch (invalid_argument& c) {};

  printf("print:\n"); environment(vi,vf).print(); printf("\n");
}

void test_linexpr1()
{
  cout << endl << "linexpr1" 
       << endl << "========" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff c[] = {1,2,3,4,5,6};
  linexpr0 e(3,c,4);
  cout << "from exp: " << linexpr1(env,e) << endl;
  cout << "from exp: " << linexpr1(env2,e) << endl;
  cout << "empty:    " << linexpr1(env) << endl;
  linexpr1 x(env,e);
  linexpr1 y = x;
  linexpr1 z(x,env2);
  //assert(!x.get_environment().contains("f2"));
  //assert(z.get_environment().contains("f2"));
  cout << endl << "copy constructor:" << endl << y.get_environment() << y.get_linexpr0() << endl << y << endl;
  cout << endl << "copy to super:" << endl << z.get_environment() << z.get_linexpr0() << endl << z << endl;
  x=z;
  cout << "assignment: " << x << endl;
  
  assert(!y.get_environment().contains("f2"));
  y.extend_environment(env2);
  assert(y.get_environment().contains("f2"));
  cout << "to supper: " << endl << y << endl;
  y.extend_environment(env2);
  cout << "to supper: " << y << endl << endl;
  x = linexpr1(env2,e);
  try { x.extend_environment(env); assert(0); } catch (invalid_argument& c) {}
  
  x = linexpr1(env,e);
  y.get_linexpr0() = linexpr0(4,c,4);
  cout << "linepxr0:  " << y.get_linexpr0() << endl;
  cout << "linepxr1:  " << y << endl;
  cout << "size:      " << y.size() << endl;
  cout << "discr:     " << y.get_discr() << endl;
  cout << "cst:       " << y.get_cst() << endl;
  cout << "[i2]:      " << y["i2"] << endl;
  y["i3"] = -3;
  y.get_cst() = 99;
  cout << "set i3 & cst: " << y << endl;
  try { y["z"]; assert(0); } catch (invalid_argument& c) {}

  assert(!y.is_integer() && !y.is_real());
  assert(y.is_linear() && y.is_quasilinear() && y.get_type()==AP_LINEXPR_LINEAR);
  y.get_cst() = coeff(0,1);
  assert(!y.is_linear() && y.is_quasilinear() && y.get_type()==AP_LINEXPR_QUASILINEAR);
  y["i3"] = coeff(0,1);
  assert(!y.is_linear() && !y.is_quasilinear() && y.get_type()==AP_LINEXPR_INTLINEAR);

  cout << "const iterator:";
  for (linexpr1::const_iterator i = y.begin();i.valid();++i)
    cout << " " << i.get_coeff();
  cout << endl;
  for (linexpr1::iterator i = y.begin();i.valid();++i) {
    cout << i.get_dim() << ": " << i.get_var() << " = " << i.get_coeff() << endl;
    i.get_coeff().neg();
  }
  cout << "after negation:  " << y << endl;

  y.get_cst() = coeff(2,2);
  y["i3"] = 0;
  cout << "before minimize: " << y << endl;
  y.minimize();
  cout << "after minimize:  " << y << endl;
  

  x.print(); printf("\n");
}

void test_lincons1()
{
  cout << endl << "lincons1" 
       << endl << "========" << endl << endl;
  coeff a[] = {1,2,3,4};
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  linexpr0 e0(3,a,4);
  linexpr0 f0(3,a+1,45);
  linexpr1 e1(env,e0);
  lincons0 c0(AP_CONS_EQ,e0);
  cout << "construct empty:         " << lincons1(env) << endl;
  cout << "construct from lincons0: " << lincons1(env,c0) << endl;
  cout << "construct from linexpr1: " << lincons1(AP_CONS_SUPEQ,e1) << endl;
  cout << "construct from linexpr1: " << lincons1(AP_CONS_EQMOD,e1,5) << endl;
  cout << "construct unsat:         " << lincons1(env,unsat()) << endl;
  lincons1 c1(AP_CONS_DISEQ,e1);
  assert(c1.has_linexpr() && !c1.has_modulo());
  lincons1 cc1(c1);
  cout << "construct copy:          " << cc1 << endl;
  cout << "construct extend:        " << lincons1(cc1,env2) << endl << lincons1(cc1,env2).get_environment();
  c1 = lincons1(AP_CONS_EQMOD,e1,99);
  assert(c1.has_linexpr() && c1.has_modulo());
  cout << "assign copy:             " << c1 << endl;
  assert(!c1.is_unsat());
  c1 = unsat();
  assert(c1.is_unsat());
  assert(c1.is_linear() && c1.is_quasilinear());
  cout << "assign unsat:            " << c1 << endl;
  c1 = lincons1(AP_CONS_EQMOD,e1);
  try { c1.get_modulo(); assert(0); } catch (invalid_argument& c) {}
  try { cout << c1; assert(0); } catch (invalid_argument& c) {}
  assert(!c1.has_modulo());
  c1.set_modulo(69);
  assert(c1.has_modulo());
  cout << "set modulo:              " << c1 << endl;
  cout << "get modulo:              " << c1.get_modulo() << endl;
  c1.get_modulo() = 70;
  cout << "set modulo:              " << c1 << endl;
  cout << "get linexpr :            " << c1.get_linexpr() << endl;
  c1.set_linexpr(linexpr1(env,f0));
  cout << "set linexpr :            " << c1 << endl;
  c1 = lincons1(AP_CONS_EQMOD,e1,99);
  c1.extend_environment(env2);
  cout << "extend environment:      " << c1 << endl;
  try { c1.extend_environment(env); assert(0); } catch (invalid_argument& c) {}
  cout << "get lincons0:            " << c1.get_lincons0() << endl;
  c1.get_lincons0() = c0;
  cout << "set lincons0:            " << c1 << endl;
  cout << "size:                    " << c1.size() << endl;
  cout << "get constyp:             " << c1.get_constyp() << endl;
  c1.get_constyp() = AP_CONS_SUPEQ;
  cout << "set constyp:             " << c1 << endl;
  cout << "get cst:                 " << c1.get_cst() << endl;
  c1.get_cst() = 101;
  cout << "set cst:                 " << c1 << endl;
  cout << "get [f3]:                " << c1["f3"] << endl;
  c1["f2"] = interval(-2,3);
  cout << "set [f2]:                " << c1["f2"] << endl;
  assert(!c1.is_linear() && !c1.is_quasilinear());
  printf("print: "); c1.print(); printf("\n");
}

void test_lincons1_array()
{
  cout << endl << "lincons1 array" 
       << endl << "==============" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  try { cout << lincons1_array(env,2); assert(0); } catch (invalid_argument& r) { cout << endl; }
  lincons0_array t0(2);
  t0[0] = lincons0(AP_CONS_EQ,linexpr0(3,a,4));
  t0[1] = lincons0(AP_CONS_EQ,linexpr0(2,b,99));
  lincons1_array t1(env,t0);
  cout << "construct from lincons0_array:" << endl << t1 << endl;
  lincons1_array tt1 = t1;
  cout << "construct copy:" << endl << tt1 << endl;
  cout << "construct extend:" << endl << lincons1_array(tt1,env2) << endl << lincons1_array(tt1,env2).get_environment();
   
  lincons1 ac[] = { lincons1(env,t0[0]), lincons1(env,t0[1]), lincons1(env,t0[0]) };
  cout << "construct from array:" << endl << lincons1_array(3,ac) << endl;
  vector<lincons1> vc(3,lincons1(env,t0[1]));
  vc[0] = lincons1(env,t0[1]); 
  vc[1] = lincons1(env,t0[0]);
  vc[2] = lincons1(env,t0[1]);
  cout << "construct from vector:" << endl << lincons1_array(vc) << endl;
  tt1 = lincons1_array(vc);
  cout << "assign copy:" << endl << tt1 << endl;
  tt1 = ac;
  cout << "assign from array:" << endl << tt1 << endl;
  tt1 = vc;
  cout << "assign from vector:" << endl << tt1 << endl;
  cout << "size: " << tt1.size() << endl;
  tt1.resize(2);
  cout << "resize:" << endl << tt1 << endl;
  cout << "size: " << tt1.size() << endl;
  tt1.extend_environment(env2);
  cout << "extend:" << endl << tt1 << endl << tt1.get_environment();
  cout << "get lincons0 array:" << endl << tt1.get_lincons0_array() << endl;
  tt1.get_lincons0_array() = t0;
  cout << "set lincons0 array:" << endl << tt1 << endl;
  cout << "get: " << tt1.get(0) << endl;
  tt1.set(0,lincons1(env,lincons0(AP_CONS_SUPEQ,linexpr0(2,b,181))));
  cout << "set: " << endl << tt1 << endl;
  try { tt1.get(2); assert(0); } catch (out_of_range& c) {}
  try { tt1.set(2,vc[0]); assert(0); } catch (out_of_range& c) {}
  tt1.resize(3);
  tt1.get(2);
  assert(!tt1.get(2).has_linexpr());
  t1 = tt1;
  assert(!t1.get(2).has_linexpr());
  tt1.set(2,vc[0]);
  assert(tt1.get(2).has_linexpr());
  assert(!t1.get(2).has_linexpr());
  vector<lincons1> vv = tt1;
  cout << "to vector: " << vv.size() << endl;
  for (size_t i=0;i<vv.size();i++) cout << vv[i] << endl;
  printf("print: "); tt1.print(); printf("\n");
}

void test_generator1()
{
  cout << endl << "generator1" 
       << endl << "==========" << endl << endl;
  coeff a[] = {1,2,3,4};
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  linexpr0 e0(3,a,4);
  linexpr0 f0(3,a+1,45);
  linexpr1 e1(env,e0);
  generator0 c0(AP_GEN_LINE,e0);
  cout << "construct empty:           " << generator1(env) << endl;
  cout << "construct from generator0: " << generator1(env,c0) << endl;
  cout << "construct from linexpr1:   " << generator1(AP_GEN_RAY,e1) << endl;
  generator1 c1(AP_GEN_VERTEX,e1);
  assert(c1.has_linexpr());
  generator1 cc1(c1);
  cout << "construct copy:            " << cc1 << endl;
  cout << "construct extend:          " << generator1(cc1,env2) << endl << generator1(cc1,env2).get_environment();
  c1 = generator1(AP_GEN_LINEMOD,e1);
  cout << "assign copy:             " << c1 << endl;
  cout << "get linexpr :            " << c1.get_linexpr() << endl;
  c1.set_linexpr(linexpr1(env,f0));
  cout << "set linexpr :            " << c1 << endl;
  c1 = generator1(AP_GEN_VERTEX,e1);
  c1.extend_environment(env2);
  cout << "extend environment:      " << c1 << endl;
  try { c1.extend_environment(env); assert(0); } catch (invalid_argument& c) {}
  cout << "get generator0:          " << c1.get_generator0() << endl;
  c1 = generator1(env,c0);
  c1.get_generator0() = generator0(AP_GEN_RAYMOD,f0);
  cout << "set generator0:          " << c1 << endl;
  cout << "size:                    " << c1.size() << endl;
  cout << "get gentyp:              " << c1.get_gentyp() << endl;
  c1.get_gentyp() = AP_GEN_RAY;
  cout << "set gentyp:              " << c1 << endl;
  cout << "get cst:                 " << c1.get_cst() << endl;
  c1.get_cst() = 101;
  cout << "set cst:                 " << c1 << endl;
  cout << "get [i3]:                " << c1["i3"] << endl;
  c1["f3"] = 18;
  cout << "set [f3]:                " << c1 << endl;
  printf("print: "); c1.print(); printf("\n");
}

void test_generator1_array()
{
  cout << endl << "generator1 array" 
       << endl << "================" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  try { cout << generator1_array(env,2); assert(0); } catch (invalid_argument& r) { cout << endl; }
  generator0_array t0(2);
  t0[0] = generator0(AP_GEN_VERTEX,linexpr0(3,a,98));
  t0[1] = generator0(AP_GEN_RAY,linexpr0(2,b,67));
  generator1_array t1(env,t0);
  cout << "construct from generator0_array:" << endl << t1 << endl;
  generator1_array tt1 = t1;
  cout << "construct copy:" << endl << tt1 << endl;
  cout << "construct extend:" << endl << generator1_array(tt1,env2) << endl << generator1_array(tt1,env2).get_environment();
   
  generator1 ac[] = { generator1(env,t0[0]), generator1(env,t0[1]), generator1(env,t0[0]) };
  cout << "construct from array:" << endl << generator1_array(3,ac) << endl;
  vector<generator1> vc(3,generator1(env,t0[1]));
  vc[0] = generator1(env,t0[1]); 
  vc[1] = generator1(env,t0[0]);
  vc[2] = generator1(env,t0[1]);
  cout << "construct from vector:" << endl << generator1_array(vc) << endl;
  tt1 = generator1_array(vc);
  cout << "assign copy:" << endl << tt1 << endl;
  tt1 = ac;
  cout << "assign from array:" << endl << tt1 << endl;
  cout << "size: " << tt1.size() << endl;
  tt1.resize(2);
  cout << "resize:" << endl << tt1 << endl;
  cout << "size: " << tt1.size() << endl;
  tt1 = vc;
  cout << "assign from vector:" << endl << tt1 << endl;
  tt1.extend_environment(env2);
  cout << "extend:" << endl << tt1 << endl << tt1.get_environment();
  cout << "get generator0 array:" << endl << tt1.get_generator0_array() << endl;
  tt1.get_generator0_array() = t0;
  cout << "set generator0 array:" << endl << tt1 << endl;
  cout << "get: " << tt1.get(0) << endl;
  tt1.set(0,generator1(env,generator0(AP_GEN_LINE,linexpr0(2,b,181))));
  cout << "set: " << endl << tt1 << endl;
  try { tt1.get(2); assert(0); } catch (out_of_range& c) {}
  try { tt1.set(2,vc[0]); assert(0); } catch (out_of_range& c) {}
  tt1.resize(3);
  tt1.get(2);
  assert(!tt1.get(2).has_linexpr());
  t1 = tt1;
  assert(!t1.get(2).has_linexpr());
  tt1.set(2,vc[0]);
  assert(tt1.get(2).has_linexpr());
  assert(!t1.get(2).has_linexpr());
  vector<generator1> vv = tt1;
  cout << "to vector: " << vv.size() << endl;
  for (size_t i=0;i<vv.size();i++) cout << vv[i] << endl;
  printf("print:\n"); tt1.print(); printf("\n");
}

void test_texpr1()
{
  cout << endl << "texpr1" 
       << endl << "======" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff a[] = {1,2,3};
  
  // construction
  cout << "construct 2:              " << texpr1(env,2) << endl;
  cout << "construct [-1,1]          " << texpr1(env,interval(-1,1)) << endl;
  cout << "construct 5/4             " << texpr1(env,frac(5,4)) << endl;
  cout << "construct 2.5             " << texpr1(env,2.5) << endl;
  cout << "construct dim(1)          " << texpr1(env,dim(1)) << endl;
  cout << "construct i3              " << texpr1(env,"i3") << endl;
  cout << "construct dim(1)*5        " << texpr1(env,dim(1)*5) << endl;
  cout << "construct f3*5            " << texpr1(env,"f3")*texpr1(env,5) << endl;
  cout << "construct sqrt(i1/f3)     " << sqrt(texpr1(env,"i1")/texpr1(env,"f3")) << endl;
  cout << "construct 2               " << texpr1::builder(env,2) << endl;
  cout << "construct [-1,1]          " << texpr1::builder(env,interval(-1,1)) << endl;
  cout << "construct 5/4             " << texpr1::builder(env,frac(5,4)) << endl;
  cout << "construct [-8;9]          " << texpr1::builder(env,-8,9) << endl;
  cout << "construct 2.5             " << texpr1::builder(env,2.5) << endl;
  cout << "construct [-81.5;2.3]     " << texpr1::builder(env,-81.5,2.4) << endl;
  cout << "construct i3              " << texpr1::builder(env,"i3") << endl;
  cout << "construct f3*5            " << texpr1::builder(env,"f3")*texpr1::builder(env,5) << endl;
  cout << "construct sqrt(i1/f3)     " << sqrt(texpr1::builder(env,"i1")/texpr1::builder(env,"f3")) << endl;
  try { texpr1 x(env,"i2"); assert(0); } catch (invalid_argument& x) {}
  try { texpr1 x = texpr1::builder(env,"i2"); assert(0); } catch (invalid_argument& x) {}
  texpr0 t0 = dim(1) / dim(2) + 5;
  texpr1 t1(env,t0);
  cout << "construct dim(1)/dim(2)+5 " << texpr1(env,t0) << endl;
  cout << "construct dim(1)/dim(2)   " << texpr1(env,t0.root().left()) << endl;
  cout << "left i3/f1                " << t1.root().left() << endl;
  cout << "right 5                   " << t1.root().right() << endl;
  cout << "construct 8 * 5           " << texpr1(env,8) * t1.root().right() << endl;
  coeff c[] = {1,2,3,4,5,6};
  linexpr0 l0(3,c,4);
  linexpr1 l1(env,l0);
  cout << "construct linexpr0        " << texpr1(env,l0) << endl;
  cout << "construct linexpr1        " << texpr1(l1) << endl;
  cout << "construct linexpr1 * i1   " << texpr1(l1)*texpr1(env,"i1") << endl;
  cout << "construct extend          " << texpr1(t1,env2) << endl;
  try { texpr1 x(texpr1(env2,t0),env); assert(0); } catch (invalid_argument& x) {}
  cout << "construct subst           " << texpr1(t1,"f1",texpr1(env,"f3")*texpr1(env,5)) << endl;
  try { texpr1(t1,"f2",texpr1(env,"f3")); assert(0); } catch (invalid_argument& x) {}
  texpr1 t2(t1);
  cout << "construct copy            " << t2 << endl;
  
  // assign
  t2 = floor(texpr1(env,"i1")*texpr1(env,0.001));
  cout << "assign floor(i1*0.001)    " << t2 << endl;
  t2 = t1;
  cout << "assign copy t1            " << t2 << endl;
  t2 = t1*t1;
  cout << "assign t1*t1              " << t2 << endl;
  t2 = t1.root().left();
  cout << "assign t1.left            " << t2 << endl;
  t2 = l1;
  cout << "assign l1                 " << t2 << endl;
  linexpr1 l2 = linexpr1(env2,l0);
  t2 = l2;
  cout << "assign l1 w/env change    " << t2 << endl << t2.get_environment();
  t2 = texpr1(env,"i3");
  cout << "assign i3 w/env change    " << t2 << endl << t2.get_environment();

  // access
  cout << "get texpr0                " << t2.get_texpr0() << endl;
  t2.get_texpr0() = dim(1)/dim(2);
  cout << "set texpr0 dim(1)/dim(2)  " << t2 << endl;
  cout << "get left.texpr0           " << t2.root().left().get_texpr0() << endl;
  t2.root().left().get_texpr0() = dim(0)*5;
  cout << "set left.texpr0 dim(0)*5  " << t2 << endl;
  cout << "get right                 " << t2.root().right() << endl;
  t2.root().right() = sqrt(texpr1(env,5)+texpr1(env,"f3"));
  cout << "set right sqrt(5+f3)      " << t2 << endl;
  cout << "discr op rtype rdir       " << t2.root().get_discr() << " " << t2.root().get_op() << " " << t2.root().get_rtype() << " " << t2.root().get_rdir() << endl;
  try { t2.root().get_coeff(); assert(0); } catch (bad_discriminant& c) {}
  try { t2.root().get_var(); assert(0); } catch (bad_discriminant& c) {}
  try { t2.root().get_dim(); assert(0); } catch (bad_discriminant& c) {}
  t2 = texpr1(env,"f3")+texpr1(env,5);
  cout << "get var f3                " << t2.root().left().get_var() << endl;
  cout << "get coeff 5               " << t2.root().right().get_coeff() << endl;
  t2.root().get_op() = AP_TEXPR_SUB;
  t2.root().get_rtype() = AP_RTYPE_SINGLE;
  t2.root().get_rdir() = AP_RDIR_DOWN;
  t2.root().left().set_var("i1");
  t2.root().right().get_coeff() = 6;
  cout << "set i1 -_f,-oo 6          " << t2 << endl;
  assert(t2.depth()==1 && t2.root().depth()==1);
  assert(t2.size()==1 && t2.root().size()==1);
  assert(t2.has_var("i1") && t2.root().has_var("i1"));
  assert(!t2.has_var("f3") && !t2.root().has_var("f3"));
  assert(!t2.has_var("i2"));
  assert(!t2.is_interval_cst() && !t2.is_interval_linear() && !t2.is_interval_polynomial() &&
	 !t2.is_interval_polyfrac() && t2.is_scalar());

  // subst
  t2 = texpr1(env,"i3");
  t2 = t2*t2+t2;
  t2.substitute("i3",sqrt(texpr1(env,5)-texpr1(env,"f3")));
  cout << "substitute                " << t2 << endl;
  try { t2.substitute("i2",texpr1(env,"i3")); assert(0); } catch (invalid_argument&) {}

  // equality
  texpr1 t3 = t2;
  assert(t3.equal(t2));
  t2 = texpr1(env,"i3")+texpr1(env,5);
  assert(t2.equal(texpr1(env,"i3")+texpr1(env,5)));
  assert(!t2.equal(texpr1(env,"i3")-texpr1(env,5)));
  assert(t2.root().equal(texpr1(env,"i3")+texpr1(env,5)));
  assert(!t2.root().equal(texpr1(env,"i3")-texpr1(env,5)));
  printf("print "); t2.print(); printf("\n");
  printf("print "); t2.root().print(); printf("\n");
}


void test_tcons1()
{
  cout << endl << "tcons1" 
       << endl << "======" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff a[] = {1,2,3};
  lincons1 ll = lincons1(AP_CONS_EQ,linexpr1(env,linexpr0(3,a,4)));
  try { cout <<  tcons1(env); assert(0); } catch (invalid_argument& r) {}
  cout << "construct i1 + 1 = 0:                " << tcons1(AP_CONS_EQ,texpr1(env,"i1")+texpr1(env,1)) << endl;
  cout << "construct floor(i1+2) >= 0:          " << tcons1(AP_CONS_SUPEQ,floor(texpr1(env,"i1")+texpr1(env,2))) << endl;
  cout << "construct i1*f1 > 0:                 " << tcons1(AP_CONS_SUP,texpr1(env,"i1")*texpr1(env,"f1")) << endl;
  cout << "construct [-1,1]*i1 != 0:            " << tcons1(AP_CONS_DISEQ,texpr1(env,interval(-1,1))*texpr1(env,"i1")) << endl;
  cout << "construct i1/f1 = 0 mod 5:           " << tcons1(AP_CONS_EQMOD,texpr1(env,"i1")/texpr1(env,"f1"),5) << endl;
  cout << "from lincons 1i1+2i3+3f1+4 = 0:      " << tcons1(ll) << endl;
  cout << "construct i1 >= f1+1:                " << (texpr1(env,"i1")>=texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 <= f1+1:                " << (texpr1(env,"i1")<=texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 >  f1+1:                " << (texpr1(env,"i1")>texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 <  f1+1:                " << (texpr1(env,"i1")<texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 == f1+1:                " << (texpr1(env,"i1")==texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 != f1+1:                " << (texpr1(env,"i1")!=texpr1(env,"f1")+texpr1(env,1)) << endl;
  cout << "construct i1 == 0:                   " << (texpr1(env,"i1")==texpr1::builder(env,0)) << endl;
  cout << "construct 0 != f1+1:                 " << (texpr1::builder(env,0)!=texpr1(env,"f1")+texpr1(env,1)) << endl;
							    
  tcons1 l = tcons1(AP_CONS_EQ,texpr1(env,"i1")*texpr1(env,"f1"));
  tcons1 m = l;
  assert(!l.is_interval_cst() && !l.is_interval_linear() && l.is_interval_polynomial() && 
	 l.is_interval_polyfrac() && l.is_scalar());
  cout << "copy i1 * f1 = 0:                    " << m << endl;
  tcons1 n(m,env2);
  cout << "extend env                           " << n << endl << n.get_environment();
  cout << "unsat:                               " << tcons1(env,unsat()) << endl;
  l = tcons1(AP_CONS_SUPEQ,texpr1(env,"i1"));
  l = unsat();
  cout << "unsat:                               " << l << endl;
  l = ll;
  cout << "from lincons 1i1+2i3+3f1+4 = 0:      " << l << endl;
  assert(!l.is_interval_cst() && l.is_interval_linear() && l.is_interval_polynomial() && 
	 l.is_interval_polyfrac() && l.is_scalar());
  tcons1 o(env);
  o = n = tcons1(AP_CONS_EQ,texpr1(env,"i1")*texpr1(env,"f1")+texpr1(env,2));
  cout << "chained copy i1 * f1 +2 = 0:         " << o << endl;
  o.extend_environment(env2);
  cout << "extend env                           " << o << endl << o.get_environment();
  assert(n.get_constyp()==AP_CONS_EQ);
  n.get_constyp()=AP_CONS_SUP;
  assert(n.get_constyp()==AP_CONS_SUP);
  cout << "get_constyp i1 * f1 + 2 > 0:         " << n << endl;  
  assert(!n.has_modulo());
  assert(n.has_texpr());
  try { n.get_modulo(); assert(0); } catch (invalid_argument& b) {}
  n.set_modulo(2);
  n.set_modulo(3);
  assert(n.has_modulo());
  n.get_constyp()=AP_CONS_EQMOD; 
  cout << "set_modulo i1 * f1 + 2 = 0 mod 3:    " << n << endl;  
  assert(!n.is_interval_cst() && !n.is_interval_linear() && l.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && n.is_scalar());
  assert(n.has_modulo());
  assert(n.get_modulo()==3);
  n.set_texpr(sqrt(texpr1(env,"i1")+texpr1(env,1)));
  cout << "set_texpr sqrt(i1+1) = 0 mod 3:      " << n << endl;  
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 !n.is_interval_polyfrac() && n.is_scalar());
  n.get_texpr() = texpr1(env,"i1")/texpr1(env,"f1")+texpr1(env,5);
  cout << "get_texpr i1/f1+5 = 0 mod 3:         " << n << endl;
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && n.is_scalar());
  n.get_texpr().left().left() = texpr1(env,interval(0,1));
  cout << "get_texpr [0,1]/i1+5 = 0 mod 3:      " << n << endl;
  assert(!n.is_interval_cst() && !n.is_interval_linear() && !n.is_interval_polynomial() && 
	 n.is_interval_polyfrac() && !n.is_scalar());
  cout << "get tcons0                           " << n.get_tcons0() << endl;
  n.get_tcons0() = (dim(1)>=5);
  cout << "set tcons0 i3>=5                     " << n << endl;
  printf("print: "); n.print(); printf("\n");
}

void test_tcons1_array()
{
  cout << endl << "tcons1 array" 
       << endl << "============" << endl << endl;
  var ai[] = { "i1","i3","i2"  };
  var af[] = { "f1","f3","f2"  };
  environment env(ai,2,af,2);
  environment env2(ai,3,af,3);
  coeff a[] = {1,2,3};
  coeff b[] = {5,3};
  tcons1_array t = tcons1_array(env,2);
  try { cout << t; assert(0); } catch (invalid_argument& r) { cout << endl; }
  t.set(0, tcons1(AP_CONS_SUPEQ,texpr1(env,sqrt(dim(0)+15*dim(2)))));
  t.set(1, tcons1(AP_CONS_EQ,texpr1(env,interval(2,5))));
  cout << "construct:    " << t << endl;
  cout << "get(1):       " << t.get(1) << endl;
  try { t.get(2); assert(0); } catch (out_of_range& c) {}
  try { t.set(3,t.get(1)); assert(0); } catch (out_of_range& c) {}
  tcons1_array u = t;
  cout << "copy:         " << t << endl; 
  tcons1_array v = tcons1_array(u,env2);
  cout << "env extend:   " << v << endl << v.get_environment();
  tcons1 ar[3] = { tcons1(AP_CONS_SUPEQ,texpr1(env,dim(0)*dim(1)*dim(2))),
		   texpr1(env,"i1")>=texpr1(env,"f1")+texpr1(env,1),
		   tcons1(AP_CONS_EQMOD,texpr1(env,interval(1,2))*texpr1(env,"i1"),5) };
  tcons1_array x = tcons1_array(3,ar);
  cout << "from array:   " << x << endl;
  vector<tcons1> ve = vector<tcons1>(2,tcons1(env));
  ve[0] = tcons1(AP_CONS_EQ,texpr1::builder(env,0));
  ve[1] = tcons1(AP_CONS_EQ,texpr1(env,1)-texpr1(env,"f3"));
  tcons1_array y = tcons1_array(ve);
  cout << "from vector:  " << y << endl;  
  y = ar;
  cout << "from array:   " << y << endl;
  x = ve;
  cout << "from vector:  " << x << endl;  
  tcons1_array w = x;
  w = u;
  cout << "copy:         " << w << endl;
  w.extend_environment(env2);
  cout << "extend environment " << w << endl << w.get_environment();
  cout << "size:     " << w.size() << endl;
  w.set(0, tcons1(AP_CONS_SUPEQ,texpr1(env,dim(1)*5+dim(2))));
  b[0] = interval(0,1);
  cout << "set:           " << w << endl;
  vector<tcons1> vv = w;
  cout << "to vector:    " << vv.size() << ": " << vv[0] << "; " << vv[1] << endl;

  // tcons0_array
  cout << "get environment  " << w.get_environment();
  cout << "get tcons0_array " << w.get_tcons0_array() << endl;
  w.get_tcons0_array()[0] = (dim(0)>=dim(1));
  cout << "set tcons_array  " << w << endl;

  // print
  printf("print: "); w.print(); printf("\n");
}


void test_abstract0(manager& m, manager& mm)
{
  cout << "level 0" << endl << endl;

  interval i[5] = { 
    interval(-1,1), 
    top(), 
    interval(scalar(infty(-1)),scalar(18)),
    interval(-0.5,1.5),
    interval(frac(1,2),frac(8,5))
  };
  interval_array ia(5,i);
  coeff c1[5] = { 1,2,3,4,0 };
  coeff c2[5] = { -1,-1 };
  coeff c3[5] = { 5,4,3,0,1 };
  coeff c4[5] = { 0,2,1,3,4 };
  linexpr0 e(5,c2,-20,AP_LINEXPR_SPARSE);
  linexpr0 ee(5,c3,-10,AP_LINEXPR_DENSE);
  lincons0 c[4] = {
    lincons0(AP_CONS_SUP,linexpr0(5,c1,1,AP_LINEXPR_DENSE)),
    lincons0(AP_CONS_SUPEQ,linexpr0(2,c2,-20,AP_LINEXPR_SPARSE)),
    lincons0(AP_CONS_EQ,linexpr0(5,c3,3,AP_LINEXPR_DENSE)),
    lincons0(AP_CONS_EQMOD,linexpr0(5,c4,4,AP_LINEXPR_SPARSE),5)
  };
  lincons0_array ca(4,c);
  texpr0 x = dim(1)*dim(1)+dim(3)/2;
  texpr0 xx = sqrt(dim(1))*dim(3);
  tcons0 t[2] = {
    dim(0)>=dim(2)+1,
    dim(1)*dim(1)<=dim(3)/2
  };
  tcons0_array ta(2,t);
  generator0 g[2] = {
    generator0(AP_GEN_LINE,linexpr0(5,c1,AP_GEN_LINE)),
    generator0(AP_GEN_RAY,linexpr0(2,c2,AP_GEN_RAY))
  };
  generator0_array ga(2,g);

  // constructors

  cout << "construct top:      " << abstract0(m,2,3,top()) << endl;
  cout << "construct bottom:   " << abstract0(m,2,3,top()) << endl;
  cout << "construct box:      " << abstract0(m,2,3,ia) << endl;
  cout << "construct lincons:  " << abstract0(m,2,3,ca) << endl;
  cout << "construct tcons:    " << abstract0(m,2,3,ta) << endl;

  abstract0 a0(m,2,3,ca);
  abstract0 a1 = a0;
  abstract0 a2(mm,a0);
  cout << "construct copy:     " << a1 << endl;
  cout << "construct copy:     " << a2 << endl;

  // copy

  a1 = top(); cout << "assign top:         " << a1 << endl;
  assert(a1==abstract0(mm,2,3,top()) && !(a1!=abstract0(mm,2,3,top())) &&
	 a1<=abstract0(mm,2,3,top()) && a1>=abstract0(mm,2,3,top()) && 
	 abstract0(mm,2,3,top())<=a1 && abstract0(mm,2,3,top())>=a1 &&
	 !(a1<abstract0(mm,2,3,top())) && !(a1>abstract0(mm,2,3,top())) && 
	 !(abstract0(mm,2,3,top())>a1) && !(abstract0(mm,2,3,top())<a1));
#if 0 // failed by Grid
  assert(a1.is_top(mm));
#endif
  a1 = bottom(); cout << "assign bottom:      " << a1 << endl;
  assert(a1==abstract0(mm,2,3,bottom()));
  assert(a1<=abstract0(mm,2,3,top()) && abstract0(mm,2,3,top())>=a1);
  assert(a1.is_bottom(mm));
  a1 = ia; cout << "assign box:         " << a1 << endl;
  a1 = ca; cout << "assign lincons:     " << a1 << endl;
  a1 = ta; cout << "assign tcons:       " << a1 << endl;
  a1 = a2; cout << "assign copy:        " << a1 << endl;
  assert(a1==a2 && a1<=a2 && a1>=a2 && !(a1!=a2));
  assert(a1.is_eq(mm,a2) && a1.is_leq(mm,a2) && a2.is_leq(mm,a1));
  a1.set(mm,top()); cout << "assign top:         " << a1 << endl;
  a1.set(mm,bottom()); cout << "assign bottom:      " << a1 << endl;
  a1.set(mm,ia); cout << "assign box:         " << a1 << endl;
  a1.set(mm,ca); cout << "assign lincons:     " << a1 << endl;
  a1.set(mm,ta); cout << "assign tcons        " << a1 << endl;
  a1.set(mm,a2); cout << "assign copy:        " << a1 << endl;
  a1.set(mm,ia,1,2); cout << "assign box redim:   " << a1 << endl;
  cout << "unchanged:          " << a0 << endl;

  // representation

  cout << "minimize:           "; 
  try { a0.minimize(m); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "canonicalize:       "; 
  try { a0.canonicalize(m); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "approximate:        "; 
  try { a0.approximate(m,0); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "closure:            "; 
  try { a0.closure(m); cout << a0; } catch (not_implemented& c) {} cout << endl;

  a0.set(mm,ca,2,3); a1.set(mm,ia,2,3);
  printf( "print:\n"); a0.print(m); printf("\n");
  try { printf( "printdiff:\n"); printdiff(m,a0,a1); } 
  catch (not_implemented& c) { printf("not implemented %s\n",c.what());}
  printf( "dump:\n"); a0.dump(m); printf("\n");

  // serialization

  try {
    size_t x;
    a0 = abstract0(m,2,3,ca);
    string* s = a0.serialize(m);
    deserialize(mm,a1,*s,&x);
    cout << "serialize: " << s->length() << " bytes" << endl;
    cout << "deserialize: " << x << " bytes" << endl;
    assert(a0==a1 && a0<=a1 && a0>=a1 && !(a0!=a1));
    delete s;
  }
  catch (not_implemented& c) { cout << "serialization not implemented: " << c.what() << endl; }

  // predicate & properties

  ap_dimension_t d = a1.get_dimension(m);
  cout << "dimensions:         " << d.intdim <<", " << d.realdim << endl;
  cout << "size:               " << a1.size(m) << endl;

  cout << "sat lincons:        " << abstract0(m,2,3,ca).sat(mm,ca[1]) << endl;
  cout << "sat tcons:          " << abstract0(m,2,3,ta).sat(mm,ta[1]) << endl;
  cout << "sat interval:       " << abstract0(m,2,3,ia).sat(mm,2,ia[2]) << endl;
  cout << "unconstrainned:     " << abstract0(m,2,3,ia).is_dimension_unconstrained(mm,2) << endl;
  a0 = abstract0(m,2,3,ca);
  cout << "bound linexpr:      " << a0.bound(mm,e) << endl;
  cout << "bound texpr:        " << a0.bound(mm,x) << endl;
  cout << "bound dimension:    " << a0.bound(mm,2) << endl;
  // reduced-product does not implement to_generator
  try { cout << "to generator:       " << a0.to_generator_array(mm) << endl; }
  catch (not_implemented& i) { cout << "to generator: not implemented" << endl; }
  cout << "to lincons:         " << a0.to_lincons_array(mm) << endl;
  cout << "to tcons:           " << a0.to_tcons_array(mm) << endl;
  cout << "to box:             " << a0.to_box(mm) << endl;

  // meet

  a1 = abstract0(m,2,3,ia);
  a2 = abstract0(m,2,3,ca);
  meet(m,a0,a1,a2);
  assert(a0<=a1 && a0<=a2);
  cout << "meet:               " << a0 << endl;
  abstract0 a3(m,2,3,ta);
  abstract0 a4(a1);
  a4.meet(m,a1); assert(a1==a4);
  a4.meet(m,a2); assert(a0==a4);
  abstract0 a5 = a1;
  a5 *= a2; assert(a5==a0);
  abstract0* ar[] = { &a1,&a2,&a3 };
  vector<const abstract0*> br(3); br[0] = &a1; br[1] = &a2; br[2] = &a3;
  meet(m,a4,3,ar);
  assert(a4<=a1 && a4<=a2 && a4<=a3);
  meet(m,a1,br);
  assert(a1==a4);
  cout << "meet array:         " << a1 << endl;

  a1 = abstract0(m,2,3,ta);
  a2 = abstract0(m,2,3,ca);
  meet(m,a3,a1,ca);
  meet(m,a4,a2,ta);
  assert(a3==a4);
  a1.meet(m,ca);
  a2.meet(m,ta);
  assert(a1==a3 && a2==a4);  
  a1 = abstract0(m,2,3,ta);
  a2 = abstract0(m,2,3,ca);
  a1 *= ca;
  a2 *= ta;
  assert(a1==a3 && a2==a4);
  cout << "meet lincons:       " << a1 << endl;
  cout << "meet tcons:         " << a2 << endl;

  // join

  a1 = abstract0(m,2,3,ia);
  a2 = abstract0(m,2,3,ca);
  join(m,a0,a1,a2);
  assert(a0>=a1 && a0>=a2);
  cout << "join:               " << a0 << endl;
  a3 = abstract0(m,2,3,ta);
  a4 = abstract0(a1);
  a4.join(m,a1); assert(a1==a4);
  a4.join(m,a2); assert(a0==a4);
  a5 = a1;
  a5 += a2; assert(a5==a0);
  join(m,a4,3, ar);
  assert(a4>=a1 && a4>=a2 && a4>=a3);
  join(m,a1,br);
  assert(a1==a4);
  cout << "join array:         " << a1 << endl;

  a1 = abstract0(m,2,3,ia);
  add_rays(m,a2,a1,ga);
  assert(a2>=a1);
  a1.add_rays(m,ga);
  assert(a1==a2);
  a1 = abstract0(m,2,3,ia);
  a1 += ga;
  assert(a1==a2);
  cout << "add rays:           " << a1 << endl;

  // assign

  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  assign(m,a2,a1,0,e);
  a1.assign(m,0,e);
  assert(a1==a2);
  cout << "assign linexpr:         " << a1 << endl;
  a5 = abstract0(m,2,3,ta);
  assign(m,a4,a3,0,e,a5);
  a3.assign(m,0,e,a5);
  assert(a3==a4);
  cout << "assign linexpr inter:   " << a3 << endl;
  linexpr0* ea[] = { &e, &ee };
  vector<const linexpr0*> ev(2); ev[0] = &e; ev[1] = &ee;
  vector<ap_dim_t> dv(2); dv[0] = 1; dv[1] = 2;
  a1 = abstract0(m,2,3,ia);
  assign(m,a2,a1,dv,ev);
  assign(m,a3,a1,2,dim12,ea);
  a4 = a1;
  a1.assign(m,dv,ev);
  a4.assign(m,2,dim12,ea);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign linexprs:        " << a1 << endl;
  a1 = abstract0(m,2,3,ia);
  assign(m,a2,a1,dv,ev,a5);
  assign(m,a3,a1,2,dim12,ea,a5);
  a4 = a1;
  a1.assign(m,dv,ev,a5);
  a4.assign(m,2,dim12,ea,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign linexprs inter:  " << a1 << endl;

  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  assign(m,a2,a1,0,x);
  a1.assign(m,0,x);
  assert(a1==a2);
  cout << "assign texpr:           " << a1 << endl;
  a5 = abstract0(m,2,3,ta);
  assign(m,a4,a3,0,x,a5);
  a3.assign(m,0,x,a5);
  assert(a3==a4);
  cout << "assign texpr inter:     " << a3 << endl;
  texpr0* xa[] = { &x, &xx };
  vector<const texpr0*> xv(2); xv[0] = &x; xv[1] = &xx;
  a1 = abstract0(m,2,3,ia);
  assign(m,a2,a1,dv,xv);
  assign(m,a3,a1,2,dim12,xa);
  a4 = a1;
  a1.assign(m,dv,xv);
  a4.assign(m,2,dim12,xa);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign texprs:          " << a1 << endl;
  a1 = abstract0(m,2,3,ia);
  assign(m,a2,a1,dv,xv,a5);
  assign(m,a3,a1,2,dim12,xa,a5);
  a4 = a1;
  a1.assign(m,dv,xv,a5);
  a4.assign(m,2,dim12,xa,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign texprs inter:    " << a1 << endl;

  // substitute

  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  substitute(m,a2,a1,0,e);
  a1.substitute(m,0,e);
  assert(a1==a2);
  cout << "subst linexpr:          " << a1 << endl;
  a5 = abstract0(m,2,3,ta);
  substitute(m,a4,a3,0,e,a5);
  a3.substitute(m,0,e,a5);
  assert(a3==a4);
  cout << "subst linexpr inter:    " << a3 << endl;
  a1 = abstract0(m,2,3,ia);
  substitute(m,a2,a1,dv,ev);
  substitute(m,a3,a1,2,dim12,ea);
  a4 = a1;
  a1.substitute(m,dv,ev);
  a4.substitute(m,2,dim12,ea);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst linexprs:         " << a1 << endl;
  a1 = abstract0(m,2,3,ia);
  substitute(m,a2,a1,dv,ev,a5);
  substitute(m,a3,a1,2,dim12,ea,a5);
  a4 = a1;
  a1.substitute(m,dv,ev,a5);
  a4.substitute(m,2,dim12,ea,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst linexprs inter:   " << a1 << endl;

  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  substitute(m,a2,a1,0,x);
  a1.substitute(m,0,x);
  assert(a1==a2);
  cout << "subst texpr:            " << a1 << endl;
  a5 = abstract0(m,2,3,ta);
  substitute(m,a4,a3,0,x,a5);
  a3.substitute(m,0,x,a5);
  assert(a3==a4);
  cout << "subst texpr inter:      " << a3 << endl;
  a1 = abstract0(m,2,3,ia);
  substitute(m,a2,a1,dv,xv);
  substitute(m,a3,a1,2,dim12,xa);
  a4 = a1;
  a1.substitute(m,dv,xv);
  a4.substitute(m,2,dim12,xa);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst texprs:           " << a1 << endl;
  a1 = abstract0(m,2,3,ia);
  substitute(m,a2,a1,dv,xv,a5);
  substitute(m,a3,a1,2,dim12,xa,a5);
  a4 = a1;
  a1.substitute(m,dv,xv,a5);
  a4.substitute(m,2,dim12,xa,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst texprs inter:     " << a1 << endl;

  // forget

  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  forget(m,a2,a1,0);
  a1.forget(m,0);
  assert(a1==a2);
  forget(m,a4,a3,0,true);
  a3.forget(m,0,true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forget:             " << a1 << endl;
  cout << "forget 0:           " << a3 << endl;
  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  forget(m,a2,a1,2,dim12);
  a1.forget(m,2,dim12);
  assert(a1==a2);
  forget(m,a4,a3,2,dim12,true);
  a3.forget(m,2,dim12,true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forgets:            " << a1 << endl;
  cout << "forgets 0:          " << a3 << endl;
  a1 = abstract0(m,2,3,ia);
  a3 = a1;
  forget(m,a2,a1,dv);
  a1.forget(m,dv);
  assert(a1==a2);
  forget(m,a4,a3,dv,true);
  a3.forget(m,dv,true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forgets:            " << a1 << endl;
  cout << "forgets 0:          " << a3 << endl;

  // dim change & permute

  ap_dim_t dca[] = {1,2,3};
  dimchange dc = dimchange(1,2,dca);
  a1 = abstract0(m,2,3,ia);
  add_dimensions(m,a2,a1,dc);
  add_dimensions(m,a3,a1,dc,true);
  a1.add_dimensions(m,dc);
  assert(a1==a2);
  assert(a1>=a3);
  cout << "add dim:            " << a1 << endl;
  cout << "add dim 0:          " << a3 << endl;
  remove_dimensions(m,a2,a1,-dc);
  a1.remove_dimensions(m,-dc);
  assert(a1==a2);
  assert(a1==abstract0(m,2,3,ia));
  cout << "remove dim:         " << a1 << endl;

  ap_dim_t dpa[] = {0,1,3,4,2};
  dimperm dp = dimperm(5,dpa);
  a1 = abstract0(m,2,3,ia);
  permute_dimensions(m,a2,a1,dp);
  a1.permute_dimensions(m,dp);
  assert(a1==a2);
  cout << "permute dim:        " << a1 << endl;
  a1.permute_dimensions(m,-dp);
  assert(a1==abstract0(m,2,3,ia));
  ap_dim_t dpa2[] = {0,1,2};
  try { a1.permute_dimensions(m,dimperm(3,dpa2)); assert(0); }
  catch (invalid_argument& c) {}
    
  // expand fold

  ap_dim_t fa[3] = { 0,2,3 };
  vector<ap_dim_t> fv(3); fv[0] = 0; fv[1] = 2; fv[2] = 3;
  a1 = abstract0(m,2,3,ia);
  expand(m,a2,a1,0,2);
  a1.expand(m,0,2);
  assert(a1==a2);
  cout << "expand:             " << a1 << endl;
  fold(m,a2,a1,3,fa);
  fold(m,a3,a1,fv);
  a4=a1;
  a1.fold(m,3,fa);
  a4.fold(m,fv);
  cout << "fold:               " << a1 << endl;
  assert(a1==a2 && a1==a3 && a1==a4);
  assert(a1==abstract0(m,2,3,ia));

  // widening

  a1 = abstract0(m,2,3,ia);
  a2 = abstract0(m,2,3,ca);
  a2 += a1;
  widening(m,a3,a1,a2);
  cout << "widening:           " << a3 << endl;
  assert(a1<=a3 && a2<=a3);
  widening(m,a3,a1,a2,ca);
  cout << "widening thr:       " << a3 << endl;
  assert(a1<=a3 && a2<=a3);

  a1.free(m);
  a2.free(mm);
}


void test_abstract1(manager& m, manager& mm)
{
  cout << "level 1" << endl << endl;
  
  var vi[3] = { "i1","i3","i2"  };
  var vf[3] = { "f1","f3","f2"  };
  environment env(vi,2,vf,3);
  environment env2(vi,3,vf,3);

  interval i[6] = { 
    interval(-1,1), 
    top(), 
    interval(scalar(infty(-1)),scalar(18)),
    interval(-0.5,1.5),
    interval(frac(1,2),frac(8,5)),
    interval(-2.,2.2)
  };
  interval_array ia(5,i);
  var va[5] = { "i1", "i3", "f1", "f3", "f2" };
  var va2[6] = { "i1", "i2", "i3", "f1", "f3", "f2" };
  vector<var> vv(5,"i1"); vv[1] = "i3"; vv[2] = "f1"; vv[3] = "f3"; vv[4] = "f2";
  coeff c1[5] = { 1,2,3,4,0 };
  coeff c2[5] = { -1,-1 };
  coeff c3[5] = { 5,4,3,0,1 };
  coeff c4[5] = { 0,2,1,3,4 };
  linexpr1 e(env,linexpr0(5,c2,-20,AP_LINEXPR_SPARSE));
  linexpr1 ee(env,linexpr0(5,c3,-10,AP_LINEXPR_DENSE));
  lincons1 c[4] = {
    lincons1(env,lincons0(AP_CONS_SUP,linexpr0(5,c1,1,AP_LINEXPR_DENSE))),
    lincons1(env,lincons0(AP_CONS_SUPEQ,linexpr0(2,c2,-20,AP_LINEXPR_SPARSE))),
    lincons1(env,lincons0(AP_CONS_EQ,linexpr0(5,c3,3,AP_LINEXPR_DENSE))),
    lincons1(env,lincons0(AP_CONS_EQMOD,linexpr0(5,c4,4,AP_LINEXPR_SPARSE),5))
  };
  lincons1_array ca(4,c);
  texpr1 x = texpr1(env,"i3")*texpr1(env,"f1")+texpr1(env,"f2")/texpr1(env,2);
  texpr1 xx = sqrt(texpr1(env,"i1"))*texpr1(env,"f2");
  tcons1 t[2] = {
    texpr1(env,"i1")>=texpr1(env,"i3")+texpr1(env,1),
    texpr1(env,"f1")*texpr1(env,"f2")<=texpr1(env,"f3")/texpr1(env,2),
  };
  tcons1_array ta(2,t);
  generator1 g[2] = {
    generator1(env,generator0(AP_GEN_LINE,linexpr0(5,c1,AP_GEN_LINE))),
    generator1(env,generator0(AP_GEN_RAY,linexpr0(2,c2,AP_GEN_RAY)))
  };
  generator1_array ga(2,g);

  // constructors

  cout << "construct top:      " << abstract1(m,env,top()) << endl;
  cout << "construct bottom:   " << abstract1(m,env,bottom()) << endl;
  cout << "construct box:      " << abstract1(m,env,va,ia) << endl;
  cout << "construct box:      " << abstract1(m,env,vv,ia) << endl;
  cout << "construct lincons:  " << abstract1(m,ca) << endl;
  cout << "construct tcons:    " << abstract1(m,ta) << endl;

  abstract1 a0(m,ca);
  abstract1 a1 = a0;
  abstract1 a2(mm,a0);
  cout << "construct copy:     " << a1 << endl;
  cout << "construct copy:     " << a2 << endl;

  // copy

  a1 = top(); cout << "assign top:         " << a1 << endl;

  assert(a1==abstract1(mm,env,top()) && !(a1!=abstract1(mm,env,top())) &&
	 a1<=abstract1(mm,env,top()) && a1>=abstract1(mm,env,top()) && 
	 abstract1(mm,env,top())<=a1 && abstract1(mm,env,top())>=a1 &&
	 !(a1<abstract1(mm,env,top())) && !(a1>abstract1(mm,env,top())) && 
	 !(abstract1(mm,env,top())>a1) && !(abstract1(mm,env,top())<a1));
#if 0 // failed by Grid
  assert(a1.is_top(mm));
#endif
  a1 = bottom(); cout << "assign bottom:      " << a1 << endl;
  assert(a1==abstract1(mm,env,bottom()));
  assert(a1<=abstract1(mm,env,top()) && abstract1(mm,env,top())>=a1);
  assert(a1.is_bottom(mm));
  a1 = ia; cout << "assign box:         " << a1 << endl;
  a1 = ca; cout << "assign lincons:     " << a1 << endl;
  a1 = ta; cout << "assign tcons:       " << a1 << endl;
  a1 = a2; cout << "assign copy:        " << a1 << endl;
  assert(a1==a2 && a1<=a2 && a1>=a2 && !(a1!=a2));
  assert(a1.is_eq(mm,a2) && a1.is_leq(mm,a2) && a2.is_leq(mm,a1));
  a1.set(mm,top()); cout << "assign top:         " << a1 << endl;
  a1.set(mm,env,top()); cout << "assign top:         " << a1 << endl;
  a1.set(mm,bottom()); cout << "assign bottom:      " << a1 << endl;
  a1.set(mm,env,bottom()); cout << "assign bottom:      " << a1 << endl;
  a1.set(mm,ia); cout << "assign box:         " << a1 << endl;
  a1.set(mm,env,va,ia); cout << "assign box:         " << a1 << endl;
  a1.set(mm,env,vv,ia); cout << "assign box:         " << a1 << endl;
  a1.set(mm,ca); cout << "assign lincons:     " << a1 << endl;
  a1.set(mm,ta); cout << "assign tcons        " << a1 << endl;
  a1.set(mm,a2); cout << "assign copy:        " << a1 << endl;
  cout << "unchanged:          " << a0 << endl;

  // representation

  cout << "minimize:           "; 
  try { a0.minimize(m); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "canonicalize:       "; 
  try { a0.canonicalize(m); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "approximate:        "; 
  try { a0.approximate(m,0); cout << a0; } catch (not_implemented& c) {} cout << endl;
  
  cout << "closure:            "; 
  try { a0.closure(m); cout << a0; } catch (not_implemented& c) {} cout << endl;

  a0.set(mm,ca); a1.set(mm,env,va,ia);
  printf( "print:\n"); a0.print(m); printf("\n");
  try { printf( "printdiff:\n"); printdiff(m,a0,a1); } 
  catch (not_implemented& c) { printf("not implemented %s\n",c.what());}
  printf( "dump:\n"); a0.dump(m); printf("\n");


  // serialization

  try {
    size_t x;
    a0 = abstract1(m,ca);
    string* s = a0.serialize(m);
    deserialize(mm,a1,*s,&x);
    cout << "serialize: " << s->length() << " bytes" << endl;
    cout << "deserialize: " << x << " bytes" << endl;
    assert(a0==a1 && a0<=a1 && a0>=a1 && !(a0!=a1));
    delete s;
  }
  catch (not_implemented& c) { cout << "serialization not implemented: " << c.what() << endl; }

  // predicate & properties

  a1.set(mm,ca); 
  cout << "environment:" << endl << a1.get_environment();
  cout << "size:               " << a1.size(m) << endl;
  cout << "get abstract0:      " << a1.get_abstract0() << endl;
  a1.get_abstract0() = ia;
  cout << "set abstract0:      " << a1 << endl;

  cout << "sat lincons:        " << abstract1(m,ca).sat(mm,ca.get(1)) << endl;
  cout << "sat tcons:          " << abstract1(m,ta).sat(mm,ta.get(1)) << endl;
  cout << "sat interval:       " << abstract1(m,env,va,ia).sat(mm,"i1",ia[2]) << endl;
  cout << "unconstrainned:     " << abstract1(m,env,va,ia).is_variable_unconstrained(mm,"f1") << endl;
  cout << "bound linexpr:      " << a0.bound(mm,e) << endl;
  cout << "bound texpr:        " << a0.bound(mm,x) << endl;

  cout << "bound variable:     " << a0.bound(mm,"f1") << endl;

  // reduced-product does not implement to_generator
  try { cout << "to generator:       " << a0.to_generator_array(mm) << endl; }
  catch (not_implemented& i) { cout << "to generator: not implemented" << endl; }

  cout << "to lincons:         " << a0.to_lincons_array(mm) << endl;
  cout << "to tcons:           " << a0.to_tcons_array(mm) << endl;
  cout << "to box:             " << a0.to_box(mm) << endl;

  // meet

  a1 = abstract1(m,env,va,ia);
  a2 = abstract1(m,ca);
  meet(m,a0,a1,a2);
  assert(a0<=a1 && a0<=a2);
  cout << "meet:               " << a0 << endl;
  abstract1 a3(m,ta);
  abstract1 a4(a1);
  a4.meet(m,a1); assert(a1==a4);
  a4.meet(m,a2); assert(a0==a4);
  abstract1 a5 = a1;
  a5 *= a2; assert(a5==a0);
  abstract1* ar[] = { &a1,&a2,&a3 };
  vector<const abstract1*> br(3); br[0] = &a1; br[1] = &a2; br[2] = &a3;
  meet(m,a4,3,ar);
  assert(a4<=a1 && a4<=a2 && a4<=a3);
  meet(m,a1,br);
  assert(a1==a4);
  cout << "meet array:         " << a1 << endl;

  a1 = abstract1(m,ta);
  a2 = abstract1(m,ca);
  meet(m,a3,a1,ca);
  meet(m,a4,a2,ta);
  assert(a3==a4);
  a1.meet(m,ca);
  a2.meet(m,ta);
  assert(a1==a3 && a2==a4);  
  a1 = abstract1(m,ta);
  a2 = abstract1(m,ca);
  a1 *= ca;
  a2 *= ta;
  assert(a1==a3 && a2==a4);
  cout << "meet lincons:       " << a1 << endl;
  cout << "meet tcons:         " << a2 << endl;

  // join

  a1 = abstract1(m,env,va,ia);
  a2 = abstract1(m,ca);
  join(m,a0,a1,a2);
  assert(a0>=a1 && a0>=a2);
  cout << "join:               " << a0 << endl;
  a3 = abstract1(m,ta);
  a4 = abstract1(a1);
  a4.join(m,a1); assert(a1==a4);
  a4.join(m,a2); assert(a0==a4);
  a5 = a1;
  a5 += a2; assert(a5==a0);
  join(m,a4,3, ar);
  assert(a4>=a1 && a4>=a2 && a4>=a3);
  join(m,a1,br);
  assert(a1==a4);
  cout << "join array:         " << a1 << endl;

  a1 = abstract1(m,env,va,ia);
  add_rays(m,a2,a1,ga);
  assert(a2>=a1);
  a1.add_rays(m,ga);
  assert(a1==a2);
  a1 = abstract1(m,env,va,ia);
  a1 += ga;
  assert(a1==a2);
  cout << "add rays:           " << a1 << endl;

  // unify

  a1 = abstract1(m,env,va,ia);
  a2 = abstract1(m,env2,va2,ia);
  unify(m,a0,a1,a2);
  a1.unify(m,a2);
  assert(a0==a1);
  cout << "unify:              " << a0 << endl;

  // assign

  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  assign(m,a2,a1,"i1",e);
  a1.assign(m,"i1",e);
  assert(a1==a2);
  cout << "assign linexpr:         " << a1 << endl;
  a5 = abstract1(m,ta);
  assign(m,a4,a3,"i1",e,a5);
  a3.assign(m,"i1",e,a5);
  assert(a3==a4);
  cout << "assign linexpr inter:   " << a3 << endl;
  linexpr1* ea[] = { &e, &ee };
  vector<const linexpr1*> ev(2); ev[0] = &e; ev[1] = &ee;
  var da[] = { "f1", "f2" };
  vector<var> dv(2,""); dv[0] = "f1"; dv[1] = "f2";
  a1 = abstract1(m,env,va,ia);
  assign(m,a2,a1,dv,ev);
  assign(m,a3,a1,2,da,ea);
  a4 = a1;
  a1.assign(m,dv,ev);
  a4.assign(m,2,da,ea);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign linexprs:        " << a1 << endl;
  a1 = abstract1(m,env,va,ia);
  assign(m,a2,a1,dv,ev,a5);
  assign(m,a3,a1,2,da,ea,a5);
  a4 = a1;
  a1.assign(m,dv,ev,a5);
  a4.assign(m,2,da,ea,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign linexprs inter:  " << a1 << endl;
  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  assign(m,a2,a1,"i1",x);
  a1.assign(m,"i1",x);
  assert(a1==a2);
  cout << "assign texpr:           " << a1 << endl;
  a5 = abstract1(m,ta);
  assign(m,a4,a3,"i1",x,a5);
  a3.assign(m,"i1",x,a5);
  assert(a3==a4);
  cout << "assign texpr inter:     " << a3 << endl;
  texpr1* xa[] = { &x, &xx };
  vector<const texpr1*> xv(2); xv[0] = &x; xv[1] = &xx;
  a1 = abstract1(m,env,va,ia);
  assign(m,a2,a1,dv,xv);
  assign(m,a3,a1,2,da,xa);
  a4 = a1;
  a1.assign(m,dv,xv);
  a4.assign(m,2,da,xa);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign texprs:          " << a1 << endl;
  a1 = abstract1(m,env,va,ia);
  assign(m,a2,a1,dv,xv,a5);
  assign(m,a3,a1,2,da,xa,a5);
  a4 = a1;
  a1.assign(m,dv,xv,a5);
  a4.assign(m,2,da,xa,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "assign texprs inter:    " << a1 << endl;

  // substitute

  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  substitute(m,a2,a1,"i1",e);
  a1.substitute(m,"i1",e);
  assert(a1==a2);
  cout << "subst linexpr:          " << a1 << endl;
  a5 = abstract1(m,ta);
  substitute(m,a4,a3,"i1",e,a5);
  a3.substitute(m,"i1",e,a5);
  assert(a3==a4);
  cout << "subst linexpr inter:    " << a3 << endl;
  a1 = abstract1(m,env,va,ia);
  substitute(m,a2,a1,dv,ev);
  substitute(m,a3,a1,2,da,ea);
  a4 = a1;
  a1.substitute(m,dv,ev);
  a4.substitute(m,2,da,ea);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst linexprs:         " << a1 << endl;
  a1 = abstract1(m,env,va,ia);
  substitute(m,a2,a1,dv,ev,a5);
  substitute(m,a3,a1,2,da,ea,a5);
  a4 = a1;
  a1.substitute(m,dv,ev,a5);
  a4.substitute(m,2,da,ea,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst linexprs inter:   " << a1 << endl;

  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  substitute(m,a2,a1,"i1",x);
  a1.substitute(m,"i1",x);
  assert(a1==a2);
  cout << "subst texpr:            " << a1 << endl;
  a5 = abstract1(m,ta);
  substitute(m,a4,a3,"i1",x,a5);
  a3.substitute(m,"i1",x,a5);
  assert(a3==a4);
  cout << "subst texpr inter:      " << a3 << endl;
  a1 = abstract1(m,env,va,ia);
  substitute(m,a2,a1,dv,xv);
  substitute(m,a3,a1,2,da,xa);
  a4 = a1;
  a1.substitute(m,dv,xv);
  a4.substitute(m,2,da,xa);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst texprs:           " << a1 << endl;
  a1 = abstract1(m,env,va,ia);
  substitute(m,a2,a1,dv,xv,a5);
  substitute(m,a3,a1,2,da,xa,a5);
  a4 = a1;
  a1.substitute(m,dv,xv,a5);
  a4.substitute(m,2,da,xa,a5);
  assert(a1==a2 && a1==a3 && a1==a4);
  cout << "subst texprs inter:     " << a1 << endl;

  // forget

  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  forget(m,a2,a1,"i1");
  a1.forget(m,"i1");
  assert(a1==a2);
  forget(m,a4,a3,"i1",true);
  a3.forget(m,"i1",true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forget:             " << a1 << endl;
  cout << "forget 0:           " << a3 << endl;
  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  forget(m,a2,a1,2,da);
  a1.forget(m,2,da);
  assert(a1==a2);
  forget(m,a4,a3,2,da,true);
  a3.forget(m,2,da,true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forgets:            " << a1 << endl;
  cout << "forgets 0:          " << a3 << endl;
  a1 = abstract1(m,env,va,ia);
  a3 = a1;
  forget(m,a2,a1,dv);
  a1.forget(m,dv);
  assert(a1==a2);
  forget(m,a4,a3,dv,true);
  a3.forget(m,dv,true);
  assert(a3==a4);
  assert(a3<=a1);
  cout << "forgets:            " << a1 << endl;
  cout << "forgets 0:          " << a3 << endl;

  // environment change

  a0 = abstract1(m,env,va,ia);
  a2 = a0;
  change_environment(m,a1,a0,env2);
  a0.change_environment(m,env2);
  assert(a0==a1);
  cout << "change env:         " << a0 << endl << a0.get_environment();
  change_environment(m,a3,a2,env2,true);
  a2.change_environment(m,env2,true);
  assert(a2==a3);
  cout << "change env 0:       " << a2 << endl << a2.get_environment();
  assert(a2<=a0);

  minimize_environment(m,a1,a0);
  a0.minimize_environment(m);
  assert(a0==a1);
  cout << "minimize env:       " << a0 << endl << a0.get_environment();

  var v1[2] = { "i1", "f2" };
  var v2[2] = { "truc", "f0" };
  a0 = abstract1(m,env,va,ia);
  a2 = a0;
  rename(m,a1,a0,2,v1,v2);
  a0.rename(m,2,v1,v2);
  cout << "rename:             " << a0 << endl;
  assert(a0==a1);
  vector<var> w1(2,"i1");   w1[1] = "f2";
  vector<var> w2(2,"truc"); w2[1] = "f0";
  rename(m,a1,a0,w2,w1);
  a0.rename(m,w2,w1);
  assert(a0==a1);
  assert(a0==a2);

  // expand fold

  a1 = abstract1(m,env,va,ia);
  a0 = a1;
  expand(m,a2,a1,"i1",2,v2);
  a1.expand(m,"i1",2,v2);
  assert(a1==a2);
  cout << "expand:             " << a1 << endl;
  expand(m,a2,a0,"i1",w2);
  a0.expand(m,"i1",w2);
  assert(a0==a1);
  assert(a1==a2);
  var v3[3] = { "truc", "i1", "f0" };
  vector<var> w3(3,"truc"); w3[1] = "i1"; w3[2] = "f0";
  fold(m,a1,a0,3,v3);
  a0.fold(m,3,v3);
  assert(a0==a1);
  cout << "fold:               " << a0 << endl;
  fold(m,a3,a2,w3);
  a2.fold(m,w3);
  assert(a2==a3);
  assert(a0==a2);
  var rn1[1] = { "truc" };
  var rn2[1] = { "i1" };
  a0.rename(m,1,rn1,rn2);
  assert(a0==abstract1(m,env,va,ia));

  // widening

  a1 = abstract1(m,env,va,ia);
  a2 = abstract1(m,ca);
  a2 += a1;
  widening(m,a3,a1,a2);
  cout << "widening:           " << a3 << endl;
  assert(a1<=a3 && a2<=a3);
  widening(m,a3,a1,a2,ca);
  cout << "widening thr:       " << a3 << endl;
  assert(a1<=a3 && a2<=a3);

  a1.free(m);
  a2.free(mm);

}

void set_algo(manager& m, int algo)
{
  for (int i=AP_FUNID_COPY;i<=AP_FUNID_CLOSURE;i++)
    m.get_funopt(ap_funid_t(i)).algorithm = algo;
}

void test_manager_algo(manager& m, manager& mm, int algo)
{
  cout << "library: " << m.get_library() << endl;
  cout << "version: " << m.get_version() << endl;
  cout << "algo:    " << algo << endl;
  try { m.fpu_init(); mm.fpu_init(); } 
  catch (runtime_error& c) { cout << "fpu_init failed: " << c.what() << endl; }
  cout << endl;
  set_algo(m,algo);
  set_algo(mm,algo);
  test_abstract0(m,mm);
  test_abstract1(m,mm);
}

void test_manager(manager& m, manager& mm)
{
  test_manager_algo(m,mm,0);
  test_manager_algo(m,mm,-1);
}

void test_box()
{
  cout << endl << "Box" 
       << endl << "===" << endl << endl;
  box_manager m,mm;
  test_manager(m,mm);
}

void test_octagon()
{
  cout << endl << "Octagon" 
       << endl << "=======" << endl << endl;
  oct_manager m,mm;
  test_manager(m,mm);
}

void test_polka()
{
  cout << endl << "Polka (loose)" 
       << endl << "=============" << endl << endl;
  polka_manager m,mm;
  test_manager(m,mm);
  cout << endl << "Polka (strict)" 
       << endl << "==============" << endl << endl;
  m = polka_manager(true);
  mm = polka_manager(true);
  test_manager(m,mm);
}

void test_t1p()
{
  cout << endl << "Taylor1plus" 
       << endl << "=======" << endl << endl;
  t1p_manager m,mm;
  test_manager(m,mm);
}


#ifdef HAS_PPL
void test_ppl()
{
  cout << endl << "PPL (polyhedra, loose)" 
       << endl << "======================" << endl << endl;
  ppl_poly_manager m,mm;
  test_manager(m,mm);
  cout << endl << "PPL (polyhedra, strict)" 
       << endl << "=======================" << endl << endl;
  m = ppl_poly_manager(true);
  mm = ppl_poly_manager(true);
  test_manager(m,mm);
  cout << endl << "PPL (grids)" 
       << endl << "===========" << endl << endl;
  m = ppl_grid_manager();
  mm = ppl_grid_manager();
  test_manager(m,mm);
  cout << endl << "Product Polka (loose) * PPL grids" 
       << endl << "=================================" << endl << endl;
  m = pkgrid_manager(false);
  mm = pkgrid_manager(false);
  test_manager(m,mm);
  cout << endl << "Product Polka (strict) * PPL grids" 
       << endl << "==================================" << endl << endl;
  m = pkgrid_manager(true);
  mm = pkgrid_manager(true);
  test_manager(m,mm);
}
#endif

int main()
{
  mpfr_init_set_d(mpfr1,1.25,GMP_RNDU);
  mpfr_init_set_d(mpfr2,99,GMP_RNDU);
  mpfr_init(mpfr3);
  test_scalar();
  test_interval();
  test_interval_array();
  test_coeff();
  test_dimchange();
  test_dimperm();
  test_linexpr0();
  test_lincons0();
  test_lincons0_array();
  test_generator0();
  test_generator0_array();
  test_texpr0();
  test_tcons0();
  test_tcons0_array();
  test_var();
  test_environment();
  test_linexpr1();
  test_lincons1();
  test_lincons1_array();
  test_generator1();
  test_generator1_array();
  test_texpr1();
  test_tcons1();
  test_tcons1_array();
  test_box();
  test_polka();
  test_octagon();
#ifdef HAS_PPL
  test_ppl();
#endif
  mpfr_clear(mpfr1);
  mpfr_clear(mpfr2);
  mpfr_clear(mpfr3);
  return 0;
}
