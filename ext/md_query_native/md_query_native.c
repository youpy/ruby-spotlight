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

MDQueryRef getQuery(VALUE obj) {
  struct QueryObject *queryObject;

  Data_Get_Struct(obj, struct QueryObject, queryObject);
  return queryObject->query;
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
  CFStringRef *itemsList;
  CFArrayRef scopesList;
  int i;

  rb_scan_args(argc, argv, "1", &scopes);
  itemsList = (CFStringRef *)malloc(sizeof(CFStringRef) * (RARRAY(scopes)->len));

  for(i = 0; i < RARRAY(scopes)->len; i ++) {
    itemsList[i] = (CFStringRef)CString2CFString(STR2CSTR(RARRAY(scopes)->ptr[i]));
  }

  scopesList = CFArrayCreate(kCFAllocatorDefault,
                             (const void**)itemsList,
                             RARRAY(scopes)->len,
                             NULL);
  MDQuerySetSearchScope(getQuery(self), scopesList, 0);

  CFRelease(scopesList);
  free(itemsList);

  return self;
}

static VALUE cMDQueryNative_execute(int argc, VALUE *argv, VALUE self)
{
  MDQueryRef query = getQuery(self);
  VALUE result = rb_ary_new();
  int resultCount, i, stringSize;
  MDItemRef item;
  CFStringRef itemValue;
  char *tmpptr;

  MDQueryExecute(query, kMDQuerySynchronous);
  MDQueryStop(query);
  resultCount = MDQueryGetResultCount(query);
  for(i = 0; i < resultCount; i ++) {
    item = (MDItemRef)MDQueryGetResultAtIndex(query, i);
    itemValue = (CFStringRef)MDItemCopyAttribute(item, kMDItemPath);

    if(itemValue != NULL) {
      stringSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(itemValue), kCFStringEncodingUTF8) + 1;
      tmpptr = (char *)malloc(sizeof(char) * stringSize);

      CFStringGetCString(itemValue, tmpptr, stringSize, kCFStringEncodingUTF8);
      rb_ary_push(result, rb_str_new2(tmpptr));

      free(tmpptr);
    }

    CFRelease(itemValue);
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
