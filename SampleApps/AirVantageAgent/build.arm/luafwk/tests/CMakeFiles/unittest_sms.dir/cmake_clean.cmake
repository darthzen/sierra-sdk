FILE(REMOVE_RECURSE
  "CMakeFiles/unittest_sms"
  "../../runtime/lua/tests/sms.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/unittest_sms.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
