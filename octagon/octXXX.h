/*
 * octXXX.h
 *
 * Public definitions.
 * The only file you need to include to use octagons.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __OCTXXX_H
#define __OCTXXX_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t* octXXX_manager_alloc(void);
/* Creates a new manager for the octagon library. */

#ifdef __cplusplus
}
#endif

#endif /* __OCTXXX_H */
