FILE(REMOVE_RECURSE
  "CMakeFiles/test_racon"
  "../../runtime/lua/tests/stagedb.lua"
  "../../runtime/lua/tests/devicetree.lua"
  "../../runtime/lua/tests/sms.lua"
  "../../runtime/lua/tests/system.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test_racon.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
