FILE(REMOVE_RECURSE
  "CMakeFiles/agent_treemgr"
  "../../../runtime/lua/agent/treemgr/init.lua"
  "../../../runtime/lua/agent/treemgr/db.lua"
  "../../../runtime/lua/agent/treemgr/build.lua"
  "../../../runtime/lua/agent/treemgr/table.lua"
  "../../../runtime/lua/agent/treemgr/handlers/constant.lua"
  "../../../runtime/lua/agent/treemgr/handlers/table.lua"
  "../../../runtime/lua/agent/treemgr/handlers/functable.lua"
  "../../../runtime/lua/agent/treemgr/handlers/cellular.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_treemgr.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
