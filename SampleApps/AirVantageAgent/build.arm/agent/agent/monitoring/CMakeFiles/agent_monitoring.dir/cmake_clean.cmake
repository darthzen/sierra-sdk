FILE(REMOVE_RECURSE
  "CMakeFiles/agent_monitoring"
  "../../../runtime/lua/agent/monitoring/init.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_monitoring.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
