#include <ruby.h>
#include </System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/Metadata.framework/Versions/A/Headers/MDQuery.h>

struct QueryObject {
  CFStringRef queryString;
  MDQueryRef query;
};

CFStringRef CString2CFString(char *str) {
  return CFStringCreateWithCString(kCFAllocatorDefault,
                                   str,
                                   CFStringGetSystemEncoding());
}

static VALUE cMDQueryNative_new(int argc, VALUE *argv, VALUE klass)
{
  VALUE queryString;
  VALUE obj;

  rb_scan_args(argc, argv, "1", &queryString);

  struct QueryObject *q = malloc(sizeof(struct QueryObject));
  q->queryString = CString2CFString(STR2CSTR(queryString));
  q->query = MDQueryCreate(kCFAllocatorDefault, q->queryString, NULL, NULL);
  obj = Data_Wrap_Struct(klass, 0, 0, q);

  return obj;
}

static VALUE cMDQueryNative_set_search_scopes(int argc, VALUE *argv, VALUE self)
{
  VALUE scopes;
  int i;
  struct QueryObject *queryObject;
  CFStringRef *itemsList;
  CFArrayRef scopesList;

  rb_scan_args(argc, argv, "1", &scopes);
  itemsList = (CFStringRef *)malloc(sizeof(CFStringRef) * (RARRAY(scopes)->len));
  Data_Get_Struct(self, struct QueryObject, queryObject);

  for(i = 0; i < RARRAY(scopes)->len; i ++) {
    itemsList[i] = (CFStringRef)CString2CFString(STR2CSTR(RARRAY(scopes)->ptr[i]));
  }

  scopesList = CFArrayCreate(kCFAllocatorDefault,
                                        (const void**)itemsList,
                                        RARRAY(scopes)->len,
                                        NULL);
  MDQuerySetSearchScope(queryObject->query, scopesList, 0);

  CFRelease(scopesList);
  free(itemsList);

  return self;
}

static VALUE cMDQueryNative_execute(int argc, VALUE *argv, VALUE self)
{
  struct QueryObject *queryObject;
  MDQueryRef query;
  VALUE result;
  int resultCount, i;
  MDItemRef item;
  CFStringRef key;

  Data_Get_Struct(self, struct QueryObject, queryObject);
  query = queryObject->query;

  MDQueryExecute(query, kMDQuerySynchronous);
  MDQueryStop(query);

  result = rb_ary_new();
  resultCount = MDQueryGetResultCount(query);
  for(i = 0; i < resultCount; i ++) {
    item = (MDItemRef)MDQueryGetResultAtIndex(query, i);
    key = (CFStringRef)CString2CFString("kMDItemPath");
    rb_ary_push(result,
                rb_str_new2(CFStringGetCStringPtr((CFStringRef)MDItemCopyAttribute(item, key),
                                                  CFStringGetSystemEncoding())));
  }

  return result;
}

void Init_md_query_native(void){
  VALUE rb_cMDQueryNative;
  
  rb_cMDQueryNative = rb_define_class("MDQueryNative", rb_cObject);
  rb_define_singleton_method(rb_cMDQueryNative, "new", cMDQueryNative_new, -1);
  rb_define_method(rb_cMDQueryNative, "set_search_scopes", cMDQueryNative_set_search_scopes, -1);
  rb_define_method(rb_cMDQueryNative, "execute", cMDQueryNative_execute, -1);
}
