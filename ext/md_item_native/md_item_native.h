#ifndef MD_ITEM_NATIVE_H
#define MD_ITEM_NATIVE_H

#include <ruby.h>
#include </System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/Metadata.framework/Versions/A/Headers/MDItem.h>
#include </System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers/CFDate.h>
#include </System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers/CFNumber.h>

VALUE createInstanceFromMDItem(MDItemRef item);

#endif
