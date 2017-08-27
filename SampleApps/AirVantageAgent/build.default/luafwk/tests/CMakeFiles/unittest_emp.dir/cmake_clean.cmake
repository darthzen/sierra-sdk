FILE(REMOVE_RECURSE
  "CMakeFiles/unittest_emp"
  "../../runtime/lua/tests/emp.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/unittest_emp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
