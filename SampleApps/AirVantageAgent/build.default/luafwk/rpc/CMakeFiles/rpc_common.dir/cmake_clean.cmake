FILE(REMOVE_RECURSE
  "CMakeFiles/rpc_common"
  "../../runtime/lua/rpc/common.lua"
  "../../runtime/lua/rpc/init.lua"
  "../../runtime/lua/rpc/proxy.lua"
  "../../runtime/lua/rpc/builtinsignatures.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/rpc_common.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
