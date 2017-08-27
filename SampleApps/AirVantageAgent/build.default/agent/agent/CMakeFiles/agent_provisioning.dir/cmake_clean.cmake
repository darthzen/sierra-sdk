FILE(REMOVE_RECURSE
  "CMakeFiles/agent_provisioning"
  "../../runtime/lua/agent/provisioning.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_provisioning.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
