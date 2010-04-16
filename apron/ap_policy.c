#include "ap_policy.h"

void ap_policy_manager_free(ap_policy_manager_t* man)
{
  if (man->count>1){
    man->count--;
  }
  else {
    if (man->internal != NULL){
      man->internal_free(man->internal);
      man->internal = NULL;
    }
    free(man);
  }
}

ap_policy_manager_t* ap_policy_manager_alloc(void* internal, void (*internal_free)(void*)) {
  ap_policy_manager_t* man;

  man = (ap_policy_manager_t*)malloc(sizeof(ap_policy_manager_t));
  man->internal_free = internal_free;
  man->count = 1;
  return man;
}

#if 0
ap_policy_manager_t _ap_policy_manager_instance;

static inline
void* ap_free_all_policies(ap_policy_manager_t* man) {
   typedef void* (*fun_policy_free_all_t)(ap_policy_manager_t* man);
   fun_policy_free_all_t fun_free_all = (fun_policy_free_all_t) man->funpolicy_array[AP_FUNPOLICYID_FREE_ALL];
   return (*fun_free_all)(man);
}

void ap_policy_manager_free(ap_policy_manager_t* man) {
  if (man->standard_manager.count>1){
    man->standard_manager.count--;
  }
  else {
    typedef void* (*fun_policy_free_all_t)(ap_policy_manager_t* man);
    if (man->standard_manager.internal != NULL){
      man->standard_manager.internal_free(man->standard_manager.internal);
      man->standard_manager.internal = NULL;
    }
    ap_result_clear(&man->standard_manager.result);

    fun_policy_free_all_t fun_free_all = (fun_policy_free_all_t) man->funpolicy_array[AP_FUNPOLICYID_FREE_ALL];
    (*fun_free_all)(man);

    free(man);
  }
}

void*
transfer_policy_meet(ap_manager_t* standard_manager, bool destructive, ap_scalar_t* a1,
      ap_scalar_t* a2) {
   typedef void* (*function_meet_policy)(ap_policy_manager_t*, ap_current_policy_t* policy, bool destructive, void* a1, void* a2);

   ap_policy_manager_t* manager = 0;
   assert(standard_manager);
   manager = as_policy_manager(standard_manager);
   function_meet_policy ptr = (function_meet_policy) manager->funpolicy_array[AP_FUNPOLICYID_MEET];
   void* result = (*ptr)(manager, &manager->current_policy, destructive, a1, a2);
   return result;
}

void*
transfer_policy_meet_lincons_array(ap_manager_t* standard_manager, bool destructive,
      ap_scalar_t* a, ap_lincons0_array_t* array) {
   typedef void* (*function_meet_policy_lincons_array)(ap_policy_manager_t* man, ap_current_policy_t* policy,
         bool destructive, void* a, ap_lincons0_array_t* array);
   ap_policy_manager_t* manager = 0;
   assert(standard_manager);
   manager = as_policy_manager(standard_manager);
   function_meet_policy_lincons_array ptr = (function_meet_policy_lincons_array) manager->funpolicy_array[AP_FUNPOLICYID_MEET_LINCONS_ARRAY];
   return (*ptr)(manager, &manager->current_policy, destructive, a, array);
}

void*
transfer_policy_meet_array(ap_manager_t* standard_manager, ap_scalar_t** tab, size_t size) {
   typedef void* (*function_meet_array_policy)(ap_policy_manager_t*, ap_current_policy_t* policy, ap_scalar_t** tab, size_t size);

   ap_policy_manager_t* manager = 0;
   assert(standard_manager);
   manager = as_policy_manager(standard_manager);
   function_meet_array_policy ptr = (function_meet_array_policy) manager->funpolicy_array[AP_FUNPOLICYID_MEET_ARRAY];
   
   return (*ptr)(manager, &manager->current_policy, tab, size);
}

void*
transfer_policy_meet_tcons_array(ap_manager_t* standard_manager, bool destructive,
      ap_scalar_t* a, ap_tcons0_array_t* array) {
   typedef void* (*function_meet_tcons_array_policy)(ap_policy_manager_t* man, ap_current_policy_t* policy, bool destructive, ap_scalar_t* a, ap_tcons0_array_t* array);

   ap_policy_manager_t* manager = 0;
   assert(standard_manager);
   manager = as_policy_manager(standard_manager);
   function_meet_tcons_array_policy ptr = (function_meet_tcons_array_policy) manager->funpolicy_array[AP_FUNPOLICYID_MEET_TCONS_ARRAY];
   return (*ptr)(manager, &manager->current_policy, destructive, a, array);
}

int ap_policy_get_index(ap_policy_manager_t* man)
   {  return man->current_policy.index; }

void* ap_policy_set_change_index(ap_policy_manager_t* man, int index) {
   man->current_policy.mode = AP_POLICY_CHANGE;
   man->current_policy.index = index;
   return &man->current_policy;
}

void* ap_policy_set_apply_index(ap_policy_manager_t* man, int index) {
   man->current_policy.mode = AP_POLICY_APPLY;
   man->current_policy.index = index;
   return &man->current_policy;
}
#endif

