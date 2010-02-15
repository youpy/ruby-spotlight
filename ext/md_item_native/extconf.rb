require "mkmf"
require "rbconfig"

$LDFLAGS += ' -framework CoreFoundation -framework CoreServices'

have_header("md_item_native")
create_makefile("md_item_native")
