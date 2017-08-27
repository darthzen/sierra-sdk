FILE(REMOVE_RECURSE
  "CMakeFiles/testspecific"
  "../../runtime/lua/agent/defaultconfig.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/testspecific.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
