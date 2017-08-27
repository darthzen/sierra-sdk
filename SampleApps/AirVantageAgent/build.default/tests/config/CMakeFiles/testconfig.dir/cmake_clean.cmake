FILE(REMOVE_RECURSE
  "CMakeFiles/testconfig"
  "../../runtime/lua/tester/configlinux_x86.lua"
  "../../runtime/lua/tester/confignative.lua"
  "../../runtime/lua/tester/defaulttargetconfig.lua"
  "../../runtime/lua/tester/defaulttestconfig.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/testconfig.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
