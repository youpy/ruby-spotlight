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

VALUE CFString2RString(CFStringRef str) {
  int stringSize;
  char *tmpptr;
  VALUE result;

  stringSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str), kCFStringEncodingUTF8) + 1;
  tmpptr = (char *)malloc(sizeof(char) * stringSize);
  CFStringGetCString(str, tmpptr, stringSize, kCFStringEncodingUTF8);
  result = rb_str_new2(tmpptr);
  free(tmpptr);

  return result;
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
  
  rb_scan_args(argc, argv, "1", &attrName);

  if(TYPE(attrName) == T_SYMBOL) {
    cfAttrName = (CFStringRef)CString2CFString(rb_id2name(SYM2ID(attrName)));
  } else {
    cfAttrName = (CFStringRef)CString2CFString(StringValuePtr(attrName));
  }

  itemValue = (CFStringRef)MDItemCopyAttribute(item, cfAttrName);

  CFRelease(cfAttrName);

  if(itemValue != NULL) {
    result = CFString2RString(itemValue);
    CFRelease(itemValue);
  } else {
    return Qnil;
  }

  return result;
}

static VALUE cMDItemNative_attribute_names(int argc, VALUE *argv, VALUE self)
{
  MDItemRef item = getItem(self);
  CFArrayRef cfAttributeNames;
  VALUE result = rb_ary_new();
  int numAttributes, i;

  rb_scan_args(argc, argv, "0");

  cfAttributeNames = MDItemCopyAttributeNames(item);

  if(cfAttributeNames != NULL) {
    numAttributes = (int)CFArrayGetCount(cfAttributeNames);
    for(i = 0; i < numAttributes; i ++) {
      rb_ary_push(result, CFString2RString((CFStringRef)CFArrayGetValueAtIndex(cfAttributeNames, i)));
    }
  }

  CFRelease(cfAttributeNames);

  return result;
}

void Init_md_item_native(void){
  VALUE rb_mSpotlight;
  
  rb_mSpotlight = rb_define_module("Spotlight");
  rb_cMDItemNative = rb_define_class_under(rb_mSpotlight, "MDItemNative", rb_cObject);
  rb_define_singleton_method(rb_cMDItemNative, "new", cMDItemNative_new, -1);
  rb_define_method(rb_cMDItemNative, "get", cMDItemNative_get, -1);
  rb_define_method(rb_cMDItemNative, "attribute_names", cMDItemNative_attribute_names, -1);
}
