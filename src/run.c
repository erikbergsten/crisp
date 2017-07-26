#include <crisp/run.h>
#include <crisp/object.h>
#include <crisp/eval.h>
#include <crisp/debug.h>
#include <crisp/env.h>
#include <crisp/core.h>
#include <crisp/show.h>
#include <crisp/gc.h>

void _define(cr_imlist * list, cr_env * env){
  cr_symbol * name = (cr_symbol *) list->value;
  if(name->prototype->type != cr_symbol_type){
    cr_debug_warn("Trying to assign non symbol!");
  }
  list = list->next;
  cr_object * preval = list->value;
  cr_object * value = cr_eval(list->value, env);
  char buf[128];
  cr_show(buf, preval);
  cr_debug_info("Defining %s as %s", name->name, buf);
  cr_env_set(env, name, value);
}

void cr_run_list(cr_list * statements){
  cr_env * global = cr_ref(cr_env_new(NULL));
  cr_debug_info("Initializing global environment");
  cr_symbol * def = cr_ref(cr_symbol_new("def"));
  cr_core_init(global);
  int n = 1;
  for(cr_node * node = statements->head; node; node = node->next){
    int skip = 0;
    cr_object * obj = (cr_object *) node->value;
    if(obj->prototype->type == cr_list_type){
      cr_imlist * list = (cr_imlist *) obj;
      if(list != cr_imlist_empty){
        if(list->value == def){
          _define(list->next, global);
          skip = 1;
        cr_free(list);
        }
      }
    }
    if(! skip){
      cr_ref(obj);
      cr_debug_info("Evaluating statement #%i with %i refs", n++, cr_ref_count(obj));
      char buf[256];
      cr_show(buf, obj);
      cr_debug_info("Statement: %s", buf);
      cr_object * val = cr_eval(obj, global);
      cr_ref(val);
      cr_show(buf, val);
      cr_debug_info("Value: %s witd %i refs", buf, cr_ref_count(val));
      cr_free(obj);
      cr_free(val);
    }
  }
  cr_debug_info("Clearing global binds");
  cr_env_free_binds(global);
  cr_free(global);
  cr_force_free(cr_imlist_empty);
}
