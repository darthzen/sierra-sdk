FILE(REMOVE_RECURSE
  "CMakeFiles/testsuites"
  "../../runtime/lua/tests/tsagent.lua"
  "../../runtime/lua/tests/tsluafwk.lua"
  "../../runtime/lua/tests/tsintegration.lua"
  "../../runtime/lua/tests/crypto.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/testsuites.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
