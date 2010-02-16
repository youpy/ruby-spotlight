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

void cMDItemNative_free(void *ptr) {
  MDItemRef item = (MDItemRef)(((struct ItemObject *)ptr)->item);

  if(item != NULL) {
    CFRelease(item);
  }

  free(ptr);
/*   printf("cMDItemNative_free()\n"); */
}

VALUE createInstanceFromMDItem(MDItemRef item) {
  struct ItemObject *itemObject;

  itemObject = malloc(sizeof(struct ItemObject));
  itemObject->item = item;

  return Data_Wrap_Struct(rb_cMDItemNative, 0, cMDItemNative_free, itemObject);
}

static VALUE cMDItemNative_new(int argc, VALUE *argv, VALUE klass)
{
  VALUE filename, obj;
  MDItemRef item;
  CFStringRef cfFilename;

  rb_scan_args(argc, argv, "1", &filename);

  cfFilename = CString2CFString(StringValuePtr(filename));
  item = MDItemCreate(kCFAllocatorDefault, cfFilename);

  if(item != NULL) {
    obj = createInstanceFromMDItem(item);
  } else {
    rb_raise(rb_eArgError, "no such file or directory");
  }

  CFRelease(cfFilename);

  return obj;
}

static VALUE cMDItemNative_get(int argc, VALUE *argv, VALUE self)
{
  MDItemRef item = getItem(self);
  CFStringRef itemValue, cfAttrName;
  VALUE attrName, result;
  char *tmpptr;
  int stringSize;
  
  rb_scan_args(argc, argv, "1", &attrName);

  if(TYPE(attrName) == T_SYMBOL) {
    cfAttrName = (CFStringRef)CString2CFString(rb_id2name(SYM2ID(attrName)));
  } else {
    cfAttrName = (CFStringRef)CString2CFString(StringValuePtr(attrName));
  }

  itemValue = (CFStringRef)MDItemCopyAttribute(item, cfAttrName);

  if(itemValue != NULL) {
    stringSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(itemValue), kCFStringEncodingUTF8) + 1;
    tmpptr = (char *)malloc(sizeof(char) * stringSize);

    CFStringGetCString(itemValue, tmpptr, stringSize, kCFStringEncodingUTF8);

    result = rb_str_new2(tmpptr);

    free(tmpptr);
    CFRelease(cfAttrName);
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
