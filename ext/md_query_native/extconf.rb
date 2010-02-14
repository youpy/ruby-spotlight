require "mkmf"
require "rbconfig"

$LDFLAGS += ' -framework CoreFoundation -framework CoreServices'

create_makefile("md_query_native")
