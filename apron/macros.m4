dnl This file is part of the APRON Library, released under LGPL license.
dnl Please read the COPYING file packaged in the distribution
dnl
changequote([[, ]])dnl
dnl ===========================================================================
dnl
dnl ===========================================================================
dnl
define([[DISPATCH_0_D]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3.MPFR);
    break;
  }]])dnl
dnl
define([[EDISPATCH_0_D]],
[[switch ($1){
  case AP_SCALAR_D:
    eitvD_$2($3.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_$2($3.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_$2($3.MPFR);
    break;
  }]])dnl
dnl
define([[EDISPATCH_0_ND]],
[[switch ($1){
  case AP_SCALAR_D:
    eitvD_$2($3,$4.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_$2($3,$4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_$2($3,$4.MPFR);
    break;
  }]])dnl
dnl
define([[DISPATCH_0_DN]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3.D,$4);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3.MPQ,$4);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3.MPFR,$4);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_0_DN]],
[[switch ($1){
  case AP_SCALAR_D:
    eitvD_$2($3.D,$4);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_$2($3.MPQ,$4);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_$2($3.MPFR,$4);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_0_DD]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3.D,$4.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3.MPQ,$4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3.MPFR,$4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_0_DD]],
[[switch ($1){
  case AP_SCALAR_D:
    eitvD_$2($3.D,$4.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_$2($3.MPQ,$4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_$2($3.MPFR,$4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_0_DDN]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3.D,$4.D,$5);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3.MPQ,$4.MPQ,$5);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3.MPFR,$4.MPFR,$5);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_0_NDN]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3,$4.D,$5);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3,$4.MPQ,$5);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3,$4.MPFR,$5);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_0_DNN]],
[[switch ($1){
  case AP_SCALAR_D:
    eitvD_$2($3.D,$4,$5);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_$2($3.MPQ,$4,$5);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_$2($3.MPFR,$4,$5);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_0_NDNN]],
[[switch ($1){
  case AP_SCALAR_D:
    itvD_$2($3,$4.D,$5,$6);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_$2($3,$4.MPQ,$5,$6);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_$2($3,$4.MPFR,$5,$6);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_N_D]],
[[switch ($1){
  case AP_SCALAR_D:
    $2 = itvD_$3($4.D);
    break;
  case AP_SCALAR_MPQ:
    $2 = itvMPQ_$3($4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2 = itvMPFR_$3($4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_N_D]],
[[switch ($1){
  case AP_SCALAR_D:
    $2 = eitvD_$3($4.D);
    break;
  case AP_SCALAR_MPQ:
    $2 = eitvMPQ_$3($4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2 = eitvMPFR_$3($4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_N_DD]],
[[switch ($1){
  case AP_SCALAR_D:
    $2 = itvD_$3($4.D,$5.D);
    break;
  case AP_SCALAR_MPQ:
    $2 = itvMPQ_$3($4.MPQ,$5.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2 = itvMPFR_$3($4.MPFR,$5.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_N_DD]],
[[switch ($1){
  case AP_SCALAR_D:
    $2 = eitvD_$3($4.D,$5.D);
    break;
  case AP_SCALAR_MPQ:
    $2 = eitvMPQ_$3($4.MPQ,$5.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2 = eitvMPFR_$3($4.MPFR,$5.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_N_DNN]],
[[switch ($1){
  case AP_SCALAR_D:
    $2 = eitvD_$3($4.D,$5,$6);
    break;
  case AP_SCALAR_MPQ:
    $2 = eitvMPQ_$3($4.MPQ,$5,$6);
    break;
  case AP_SCALAR_MPFR:
    $2 = eitvMPFR_$3($4.MPFR,$5,$6);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_D_0]],
[[switch ($1){
  case AP_SCALAR_D:
    $2.D = eitvD_$3();
    break;
  case AP_SCALAR_MPQ:
    $2.MPQ = eitvMPQ_$3();
    break;
  case AP_SCALAR_MPFR:
    $2.MPFR = eitvMPFR_$3();
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_D_N]],
[[switch ($1){
  case AP_SCALAR_D:
    $2.D = itvD_$3($4);
    break;
  case AP_SCALAR_MPQ:
    $2.MPQ = itvMPQ_$3($4);
    break;
  case AP_SCALAR_MPFR:
    $2.MPFR = itvMPFR_$3($4);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_D_D]],
[[switch ($1){
  case AP_SCALAR_D:
    $2.D = itvD_$3($4.D);
    break;
  case AP_SCALAR_MPQ:
    $2.MPQ = itvMPQ_$3($4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2.MPFR = itvMPFR_$3($4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[EDISPATCH_D_D]],
[[switch ($1){
  case AP_SCALAR_D:
    $2.D = eitvD_$3($4.D);
    break;
  case AP_SCALAR_MPQ:
    $2.MPQ = eitvMPQ_$3($4.MPQ);
    break;
  case AP_SCALAR_MPFR:
    $2.MPFR = eitvMPFR_$3($4.MPFR);
    break;
  default:
    abort();
  }]])dnl
dnl
define([[DISPATCH_D_DNN]],
[[switch ($1){
  case AP_SCALAR_D:
    $2.D = itvD_$3($4.D,$5,$6);
    break;
  case AP_SCALAR_MPQ:
    $2.MPQ = itvMPQ_$3($4.MPQ,$5,$6);
    break;
  case AP_SCALAR_MPFR:
    $2.MPFR = itvMPFR_$3($4.MPFR,$5,$6);
    break;
  default:
    abort();
  }]])dnl
dnl
