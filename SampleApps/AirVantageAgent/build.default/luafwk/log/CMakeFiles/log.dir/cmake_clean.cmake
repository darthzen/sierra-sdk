FILE(REMOVE_RECURSE
  "CMakeFiles/log"
  "../../runtime/lua/log/init.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/log.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
