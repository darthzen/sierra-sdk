FILE(REMOVE_RECURSE
  "CMakeFiles/mihini_misc"
  "../runtime/lua/print.lua"
  "../runtime/lua/strict.lua"
  "../runtime/lua/timer.lua"
  "../runtime/lua/unittest.lua"
  "../runtime/lua/niltoken.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/mihini_misc.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
