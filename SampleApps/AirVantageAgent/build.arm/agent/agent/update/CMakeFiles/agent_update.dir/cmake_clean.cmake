FILE(REMOVE_RECURSE
  "CMakeFiles/agent_update"
  "../../../runtime/lua/agent/update/init.lua"
  "../../../runtime/lua/agent/update/builtinupdaters.lua"
  "../../../runtime/lua/agent/update/pkgcheck.lua"
  "../../../runtime/lua/agent/update/updatemgr.lua"
  "../../../runtime/lua/agent/update/common.lua"
  "../../../runtime/lua/agent/update/downloader.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_update.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
