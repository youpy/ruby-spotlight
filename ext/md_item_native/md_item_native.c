#include <ruby.h>
#include </System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/Metadata.framework/Versions/A/Headers/MDItem.h>

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

static VALUE cMDItemNative_new(int argc, VALUE *argv, VALUE klass)
{
  struct ItemObject *itemObject;
  VALUE filename, obj;
  MDItemRef item;
  rb_scan_args(argc, argv, "1", &filename);
  struct ItemObject *i = malloc(sizeof(struct ItemObject));
  item = MDItemCreate(kCFAllocatorDefault, CString2CFString(STR2CSTR(filename)));
  if(item != NULL) {
    i->item = item;
    obj = Data_Wrap_Struct(klass, 0, -1, i);
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
  } else {
    result = Qnil;
  }

  CFRelease(itemValue);

  return result;
}

void Init_md_item_native(void){
  VALUE rb_mSpotlight;
  VALUE rb_cMDItemNative;
  
  rb_mSpotlight = rb_define_module("Spotlight");
  rb_cMDItemNative = rb_define_class_under(rb_mSpotlight, "MDItemNative", rb_cObject);
  rb_define_singleton_method(rb_cMDItemNative, "new", cMDItemNative_new, -1);
  rb_define_method(rb_cMDItemNative, "get", cMDItemNative_get, -1);
}
