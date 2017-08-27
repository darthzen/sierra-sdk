FILE(REMOVE_RECURSE
  "CMakeFiles/stagedb_lua"
  "../../runtime/lua/stagedb.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/stagedb_lua.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
