FILE(REMOVE_RECURSE
  "CMakeFiles/coxpcall"
  "../../runtime/lua/coxpcall.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/coxpcall.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
