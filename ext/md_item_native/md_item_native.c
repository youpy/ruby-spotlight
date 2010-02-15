#include "md_item_native.h"

static VALUE rb_cMDItemNative;

struct ItemObject {
  MDItemRef item;
};

CFStringRef CString2CFString(char *str) {
  return CFStringCreateWithCString(kCFAllocatorDefault,
                                   str,
                                   CFStringGetSystemEncoding());
}

MDItemRef getItem(VALUE obj) {
  struct ItemObject *itemObject;

  Data_Get_Struct(obj, struct ItemObject, itemObject);
  return itemObject->item;
}

VALUE createInstanceFromMDItem(MDItemRef item) {
  struct ItemObject *itemObject;

  itemObject = malloc(sizeof(struct ItemObject));
  itemObject->item = item;

  return Data_Wrap_Struct(rb_cMDItemNative, 0, -1, itemObject);
}

static VALUE cMDItemNative_new(int argc, VALUE *argv, VALUE klass)
{
  VALUE filename, obj;
  MDItemRef item;

  rb_scan_args(argc, argv, "1", &filename);
  item = MDItemCreate(kCFAllocatorDefault, CString2CFString(STR2CSTR(filename)));
  if(item != NULL) {
    obj = createInstanceFromMDItem(item);
  } else {
    rb_raise(rb_eArgError, "no such file or directory");
  }

  return obj;
}

static VALUE cMDItemNative_get(int argc, VALUE *argv, VALUE self)
{
  MDItemRef item = getItem(self);
  CFStringRef itemValue;
  VALUE attrName, result;
  char *tmpptr;
  int stringSize;
  
  rb_scan_args(argc, argv, "1", &attrName);

  if(TYPE(attrName) == T_SYMBOL) {
    itemValue = (CFStringRef)MDItemCopyAttribute(item, CString2CFString(rb_id2name(SYM2ID(attrName))));
  } else {
    itemValue = (CFStringRef)MDItemCopyAttribute(item, CString2CFString(STR2CSTR(attrName)));
  }

  if(itemValue != NULL) {
    stringSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(itemValue), kCFStringEncodingUTF8) + 1;
    tmpptr = (char *)malloc(sizeof(char) * stringSize);

    CFStringGetCString(itemValue, tmpptr, stringSize, kCFStringEncodingUTF8);

    result = rb_str_new2(tmpptr);

    free(tmpptr);
    CFRelease(itemValue);
  } else {
    return Qnil;
  }

  return result;
}

void Init_md_item_native(void){
  VALUE rb_mSpotlight;
  
  rb_mSpotlight = rb_define_module("Spotlight");
  rb_cMDItemNative = rb_define_class_under(rb_mSpotlight, "MDItemNative", rb_cObject);
  rb_define_singleton_method(rb_cMDItemNative, "new", cMDItemNative_new, -1);
  rb_define_method(rb_cMDItemNative, "get", cMDItemNative_get, -1);
}
