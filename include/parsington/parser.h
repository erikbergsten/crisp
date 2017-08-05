#ifndef __parsington_parser_h__
#define __parsington_parser_h__

#include <parsington/token.h>
#include <parsington/tokenizer.h>
#include <crisp/object.h>


cr_object * pt_parser_next(pt_tokenizer * tz);
cr_object * pt_parser_parse_str(char * str);

#endif /* __parsington_parser_h__ */
