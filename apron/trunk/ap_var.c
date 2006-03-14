/* ************************************************************************* */
/* ap_var.c: generic variables */
/* ************************************************************************* */

#include <string.h>
#include <limits.h>
#include "ap_manager.h"
#include "ap_var.h"

static inline char* strdup(const char* s){
  char* s2;

  s2 = malloc(strlen(s)+1);
  strcpy(s2,s);
  return s2;
}

static int var_compare_default(void* v1, void* v2){
  return (v1==v2) ? 0 : strcmp((char*)v1,(char*)v2);
}
static void* var_copy_default(void* v){
  return strdup((char*)v);
}
static void var_free_default(void* v){
  free(v);
}
static char* var_to_string_default(void* v){
  return strdup((char*)v);
}

struct ap_var_operations_t ap_var_operations_default = {
  var_compare_default,
  var_copy_default,
  var_free_default,
  var_to_string_default
};

ap_var_operations_t* ap_var_operations = &ap_var_operations_default;
