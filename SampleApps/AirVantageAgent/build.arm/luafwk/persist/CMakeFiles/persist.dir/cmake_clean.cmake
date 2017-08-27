FILE(REMOVE_RECURSE
  "CMakeFiles/persist"
  "../../runtime/lua/persist/init.lua"
  "../../runtime/lua/persist/file.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/persist.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
