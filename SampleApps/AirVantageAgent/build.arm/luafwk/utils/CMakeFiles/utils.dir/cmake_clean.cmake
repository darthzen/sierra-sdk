FILE(REMOVE_RECURSE
  "CMakeFiles/utils"
  "../../runtime/lua/utils/loader.lua"
  "../../runtime/lua/utils/path.lua"
  "../../runtime/lua/utils/table.lua"
  "../../runtime/lua/utils/loweralias.lua"
  "../../runtime/lua/utils/ltn12/source.lua"
  "../../runtime/lua/utils/system.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/utils.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
