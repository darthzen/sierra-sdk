FILE(REMOVE_RECURSE
  "CMakeFiles/log_tools"
  "../../runtime/lua/log/tools.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/log_tools.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
