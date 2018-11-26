#include <ruby.h>
#include "../md_item_native/md_item_native.h"

struct QueryObject {
  CFStringRef queryString;
  MDQueryRef query;
};

CFStringRef CString2CFString(char *str) {
  return CFStringCreateWithCString(kCFAllocatorDefault,
                                   str,
                                   CFStringGetSystemEncoding());
}

MDQueryRef getQuery(VALUE obj) {
  struct QueryObject *queryObject;

  Data_Get_Struct(obj, struct QueryObject, queryObject);
  return queryObject->query;
}

void cMDQueryNative_free(void *ptr) {
  MDQueryRef query = (MDQueryRef)(((struct QueryObject *)ptr)->query);
  CFStringRef queryString = (CFStringRef)(((struct QueryObject *)ptr)->queryString);

  if(query != NULL) {
    CFRelease(query);
  }

  if(queryString != NULL) {
    CFRelease(queryString);
  }

  free(ptr);
/*   printf("cMDQueryNative_free()\n"); */
}

static VALUE cMDQueryNative_new(int argc, VALUE *argv, VALUE klass)
{
  VALUE queryString;
  VALUE obj;
  struct QueryObject *q;

  rb_scan_args(argc, argv, "1", &queryString);

  q = malloc(sizeof(struct QueryObject));
  q->queryString = CString2CFString(StringValuePtr(queryString));
  q->query = MDQueryCreate(kCFAllocatorDefault, q->queryString, NULL, NULL);
  obj = Data_Wrap_Struct(klass, 0, cMDQueryNative_free, q);

  return obj;
}

static VALUE cMDQueryNative_set_search_scopes(int argc, VALUE *argv, VALUE self)
{
  VALUE scopes;
  CFStringRef *itemsList;
  CFArrayRef scopesList;
  int i;

  rb_scan_args(argc, argv, "1", &scopes);
  itemsList = (CFStringRef *)malloc(sizeof(CFStringRef) * (RARRAY_LEN(scopes)));

  for(i = 0; i < RARRAY_LEN(scopes); i ++) {
    itemsList[i] = (CFStringRef)CString2CFString(StringValuePtr(RARRAY_PTR(scopes)[i]));
  }

  scopesList = CFArrayCreate(kCFAllocatorDefault,
                             (const void**)itemsList,
                             RARRAY_LEN(scopes),
                             NULL);
  MDQuerySetSearchScope(getQuery(self), scopesList, 0);

  CFRelease(scopesList);
  free(itemsList);

  return self;
}

static VALUE cMDQueryNative_execute(int argc, VALUE *argv, VALUE self)
{
  MDQueryRef query = getQuery(self);
  MDItemRef item;
  VALUE result = rb_ary_new();
  VALUE rItem;
  int resultCount, i;

  MDQueryExecute(query, kMDQuerySynchronous);
  MDQueryStop(query);
  resultCount = MDQueryGetResultCount(query);
  for(i = 0; i < resultCount; i ++) {
    item = (MDItemRef)MDQueryGetResultAtIndex(query, i);
    if(item != NULL) {
      CFRetain(item);
      rItem = createInstanceFromMDItem(item);
      rb_ary_push(result, rItem);
    }
  }

  return result;
}

void Init_md_query_native(void){
  VALUE rb_mSpotlight;
  VALUE rb_cMDQueryNative;
  
  rb_mSpotlight = rb_define_module("Spotlight");
  rb_cMDQueryNative = rb_define_class_under(rb_mSpotlight, "MDQueryNative", rb_cObject);
  rb_define_singleton_method(rb_cMDQueryNative, "new", cMDQueryNative_new, -1);
  rb_define_method(rb_cMDQueryNative, "set_search_scopes", cMDQueryNative_set_search_scopes, -1);
  rb_define_method(rb_cMDQueryNative, "execute", cMDQueryNative_execute, -1);
}
