FILE(REMOVE_RECURSE
  "CMakeFiles/lua.dir/lua.c.o"
  "../../runtime/bin/lua.pdb"
  "../../runtime/bin/lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/lua.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
