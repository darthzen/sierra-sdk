FILE(REMOVE_RECURSE
  "CMakeFiles/agent_platform"
  "../../runtime/lua/agent/defaultconfig.lua"
  "../../runtime/lua/agent/platform.lua"
  "../../runtime/lua/agent/netman.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_platform.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
