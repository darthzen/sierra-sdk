FILE(REMOVE_RECURSE
  "CMakeFiles/agent_asscon"
  "../../../runtime/lua/agent/asscon/init.lua"
  "../../../runtime/lua/agent/asscon/datamanager.lua"
  "../../../runtime/lua/agent/asscon/sms.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_asscon.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
