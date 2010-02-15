#ifndef MD_ITEM_NATIVE_H
#define MD_ITEM_NATIVE_H

#include <ruby.h>
#include </System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/Metadata.framework/Versions/A/Headers/MDItem.h>

VALUE createInstanceFromMDItem(MDItemRef item);

#endif
