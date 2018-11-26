#ifndef MD_ITEM_NATIVE_H
#define MD_ITEM_NATIVE_H

#include <ruby.h>
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>

VALUE createInstanceFromMDItem(MDItemRef item);

#endif

#ifndef RUBY_19
#ifndef RFLOAT_VALUE
#define RFLOAT_VALUE(v) (RFLOAT(v)->value)
#endif
#ifndef RARRAY_LEN
#define RARRAY_LEN(v) (RARRAY(v)->len)
#endif
#ifndef RARRAY_PTR
#define RARRAY_PTR(v) (RARRAY(v)->ptr)
#endif
#endif
