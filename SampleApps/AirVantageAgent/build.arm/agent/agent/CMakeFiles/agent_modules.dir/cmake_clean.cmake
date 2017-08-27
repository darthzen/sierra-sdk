FILE(REMOVE_RECURSE
  "CMakeFiles/agent_modules"
  "../../runtime/lua/agent/init.lua"
  "../../runtime/lua/agent/modem.lua"
  "../../runtime/lua/agent/time.lua"
  "../../runtime/lua/agent/config.lua"
  "../../runtime/lua/agent/mediation.lua"
  "../../runtime/lua/agent/migration.lua"
  "../../runtime/lua/agent/appcon.lua"
  "../../runtime/lua/agent/bearer.lua"
  "../../runtime/lua/agent/boot.lua"
  "../../runtime/lua/agent/autoexec.lua"
  "../../runtime/lua/agent/system.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_modules.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
