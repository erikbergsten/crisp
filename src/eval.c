#include <crisp/eval.h>
#include <crisp/integer.h>
#include <crisp/macro.h>
#include <crisp/show.h>
#include <crisp/env.h>
#include <crisp/quote.h>
#include <crisp/imlist.h>
#include <crisp/function.h>
#include <crisp/debug.h>

cr_imlist * _eval_rest_args(cr_imlist * args, cr_runtime * runtime, cr_env * env ){
  cr_imlist * list = cr_imlist_empty;
  while(args != cr_imlist_empty){
    //evaluate the argument
    cr_object * value = cr_eval(args->value, runtime, env);
    list = cr_imlist_prepend(list, value);
  }
  //we must reverse the list since we created it in reverse order
  return cr_imlist_reverse(list);
}
cr_object * _eval_native(cr_fun_native * fun,
                         cr_imlist * args,
                         cr_runtime * runtime,
                         cr_env * env){
  //most of the work wil be done by the native function code, all we do
  //is call it!
  cr_debug_info("Calling native function");
  cr_object * value = fun->function(args, runtime, env);
#ifdef CR_DEBUG
  char buf[256];
  cr_show(buf, value);
  cr_debug_info("Native function returned: %s", buf);
#endif

  //thats all there is to it
  return value;
}
cr_object * _eval_fun(cr_fun * function,
                      cr_imlist * args,
                      cr_runtime * runtime,
                      cr_env * env){

  //we must first see if its a native function and use the native function
  //to evaluate if it is
  if(function->form == cr_function_native){
    return _eval_native((cr_fun_native *) function, args, runtime, env);
  }else{
    cr_debug_info("Evaluating function!");
    cr_fun_normal * fun = (cr_fun_normal *) function;
#ifdef CR_DEBUG
    char buf[128];
#endif
    //to evaluate a non-native function we create a new env called fun_env
    cr_env * fun_env = cr_env_new(fun->env);
    //The new env has the env in which it was defined
    //as its parent env, not the env in which it was called.
    //See examples/fun_info.cr for an explanation of why.

    //Every argument name in the functions arg list will now be bound
    //to a value in the calling list
    cr_imlist * arg_names = fun->args;
    while(arg_names != cr_imlist_empty && args != cr_imlist_empty){
      if(((cr_symbol *) arg_names) == cr_symbol_rest){
        //if we find the rest symbol "&" we will bind all the remaining
        //arguments in one list to the next argument name, see an explanation
        //in examples/varargs.cr

        //first get the next symbol
        arg_names = arg_names->next;
        cr_symbol * rest_name = (cr_symbol *) arg_names->value;

        //then evaluate the rest of the args and put them in a list
        cr_imlist * rest_values = _eval_rest_args(args, runtime, env);

        //bind the rest symbol to the list
        cr_env_setS(fun_env, rest_name, rest_values);

        //then we skip the rest of both lists!
        arg_names = cr_imlist_empty;
        args = cr_imlist_empty;
      }else{
        //no vararg symbol found

        //take the name of the next argument
        cr_symbol * arg_name = (cr_symbol *) arg_names->value;
        //evaluate the next argument
        //the arguments are evaluated in the calling env not the function env
        cr_object *arg_value = cr_eval(args->value, runtime, env);

#ifdef CR_DEBUG
        cr_show(buf, arg_value);
        cr_debug_info("Setting %s to %s in function environment.",
                       arg_name->name, buf);
#endif
        //set it in the fun_env
        cr_env_set(fun_env, arg_name, arg_value);

        //we move the pointers to both the arg_name list and the arg list forward
        arg_names = arg_names->next;
        args = args->next;
      }
      //after we have bound the arguments we are ready to evaluate the function
      cr_object * value = cr_eval((cr_object *) fun->body, //the body of the function
                                  runtime,   //the runtime
                                  fun_env);  //the function environment
#ifdef CR_DEBUG
      cr_show(buf, value);
      cr_debug_info("Function returned: %s", buf);
#endif
      //Thats it! we have evaluated the function
      return value;
    }
  }

}
cr_object * _eval_macro(cr_macro * macro,
                        cr_imlist * args,
                        cr_runtime * runtime,
                        cr_env * env){
  //NOT YET IMPLEMENTED WOOPS
  cr_debug_fail("Macros have not been implemented yet! woops!");
  return NULL;
}
cr_object * _eval_list(cr_imlist * list, cr_runtime * runtime, cr_env * env){
  if(list == cr_imlist_empty){
    //the empty list cannot be evaluated since.. its nothing
    //if you want nothing you have to use the quoted emty list: '()
    //or the symbol null (refered to by the C pointer cr_symbol_null)
    cr_debug_fail("Trying to evaluate the empty list!");
    return NULL;
  }
  // the head of the list needs to be evaluated
  cr_object * head = (cr_object *) list->value;
  cr_object * app = cr_eval(head, runtime, env);
  //if the evaluated head is either a macro or a function we apply it, otherwise
  //we throw an error
  if(cr_object_type(app, cr_function_type)){
    //run function
    return _eval_fun((cr_fun *) app, //the function we are running
                     list->next,     //the rest of the list - i.e. the args
                     runtime,        //the runtime pointer
                     env);           //the current evaluation environment
  }else if(cr_object_type(app, cr_macro_type)){
    //run macro
    return _eval_macro((cr_macro *) app,
                        list->next,
                        runtime,
                        env);
  }else{
    //since the only applicable types are macros and function we fail
    //if the list starts with any other type of value
    cr_debug_fail("Trying to apply non-function non-macro value!");
    return NULL;
  }
}
cr_object * _eval_sym(cr_symbol * symbol, cr_runtime * runtime, cr_env * env){
  cr_debug_info("Evaluating symbol: %s", symbol->name);
  //evaluating a symbol is simply done by looking it up in the env
  cr_object * value = cr_env_get(env, symbol);
#ifdef CR_DEBUG
  char buf[256];
  cr_show(buf, value);
  cr_debug_info("%s resolved to: %s", symbol->name, value);
#endif
  return value;
}
cr_object * cr_eval(cr_object * object, cr_runtime * runtime, cr_env * env){
  //only lists symbols and quotes evaluate to something other than themselves
  //numbers are just numbers, functions are just functions and tuples are just
  //tuples
  switch(object->prototype->type){
    case cr_symbol_type:
      return _eval_sym((cr_symbol *) object, runtime, env);
      break;
    case cr_list_type:
      return _eval_list((cr_imlist *) object, runtime, env);
      break;
    case cr_quote_type:
      //evaluating a quote is simply done by taking the value out of the quote
      return ((cr_quote *) object)->value;
      break;
    default:
      return object;
      break;
  }
}
