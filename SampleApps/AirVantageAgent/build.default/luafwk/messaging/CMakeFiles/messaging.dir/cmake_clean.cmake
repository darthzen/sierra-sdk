FILE(REMOVE_RECURSE
  "CMakeFiles/messaging"
  "../../runtime/lua/messaging.lua"
  "../../runtime/lua/messaginghal.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/messaging.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
