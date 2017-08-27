FILE(REMOVE_RECURSE
  "CMakeFiles/agent_devman"
  "../../../runtime/lua/agent/devman/init.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_devman.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
