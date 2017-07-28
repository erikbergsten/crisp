#ifndef __crisp_show_h__
#define __crisp_show_h__

#include <crisp/object.h>

void cr_show(char * dest, cr_object * obj);
#define cr_showS(d, o) cr_show(d, (cr_object *) o)

#endif /* __crisp_show_h__ */
