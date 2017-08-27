FILE(REMOVE_RECURSE
  "CMakeFiles/test_fwkemb"
  "../../runtime/lua/tests/luafwktests.lua"
  "../../runtime/lua/tests/agenttests.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test_fwkemb.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
