FILE(REMOVE_RECURSE
  "CMakeFiles/unittest_system"
  "../../runtime/lua/tests/system.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/unittest_system.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
