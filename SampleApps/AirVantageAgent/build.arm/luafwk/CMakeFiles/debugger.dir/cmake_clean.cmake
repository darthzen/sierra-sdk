FILE(REMOVE_RECURSE
  "CMakeFiles/debugger"
  "../runtime/lua/debugger.lua"
  "../runtime/lua/debugintrospection.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/debugger.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
