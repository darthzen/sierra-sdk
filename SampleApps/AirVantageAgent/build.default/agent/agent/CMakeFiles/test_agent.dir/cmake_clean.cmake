FILE(REMOVE_RECURSE
  "CMakeFiles/test_agent"
  "../../runtime/lua/tests/config.lua"
  "../../runtime/lua/tests/monitoring.lua"
  "../../runtime/lua/tests/aleosstub.lua"
  "../../runtime/lua/tests/time.lua"
  "../../runtime/lua/tests/mediation.lua"
  "../../runtime/lua/tests/mediationtestserver.lua"
  "../../runtime/lua/tests/treemgr.lua"
  "../../runtime/lua/tests/appcon.lua"
  "../../runtime/lua/tests/treemgr_table1.lua"
  "../../runtime/lua/tests/treemgr_table2.lua"
  "../../runtime/lua/tests/update.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test_agent.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
