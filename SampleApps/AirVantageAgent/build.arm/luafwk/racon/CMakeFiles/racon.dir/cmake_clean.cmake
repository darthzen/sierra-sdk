FILE(REMOVE_RECURSE
  "CMakeFiles/racon"
  "../../runtime/lua/racon/asset/init.lua"
  "../../runtime/lua/racon/asset/tree.lua"
  "../../runtime/lua/racon/common.lua"
  "../../runtime/lua/racon/empparser.lua"
  "../../runtime/lua/racon/init.lua"
  "../../runtime/lua/racon/ipc.lua"
  "../../runtime/lua/racon/table.lua"
  "../../runtime/lua/racon/system.lua"
  "../../runtime/lua/racon/devicetree.lua"
  "../../runtime/lua/racon/sms.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/racon.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
