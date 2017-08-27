FILE(REMOVE_RECURSE
  "CMakeFiles/tester"
  "../../runtime/lua/tester/testMain.lua"
  "../../runtime/lua/tester/testManager.lua"
  "../../runtime/lua/tester/targetManager.lua"
  "../../runtime/lua/tester/testerTools.lua"
  "../../runtime/lua/tester/platformAPI.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/tester.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
