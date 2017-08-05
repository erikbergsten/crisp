cmake_minimum_required (VERSION 3.0)

set(SRC "debug.c"
        "function.c"
        "list.c"
        "object.c"
        "map.c"
        "eval.c"
        "env.c"
        "integer.c"
        "quote.c"
        "symbol.c"
        "double.c"
        "imlist.c"
        "tokenizer.c"
        "token.c"
        "show.c"
        "parser.c"
        "run.c"
        "core.c"
        "core_list.c"
        "lookup.c"
        "core_math.c"
        "tuple.c"
        "gc.c")

add_library(crisp SHARED ${SRC})
