#ifndef _ITV_CONFIG_H_
#define _ITV_CONFIG_H_

#ifdef __cplusplus
#define HAS_BOOL
extern "C" {
#endif

#ifndef HAS_BOOL
#define HAS_BOOL
typedef char bool;
static const bool false = 0;
static const bool true  = 1;
#endif

#ifdef __cplusplus
}
#endif

#endif
