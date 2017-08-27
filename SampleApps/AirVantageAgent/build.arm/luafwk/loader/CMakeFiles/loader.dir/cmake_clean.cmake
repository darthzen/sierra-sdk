FILE(REMOVE_RECURSE
  "CMakeFiles/loader.dir/loader.c.o"
  "../../runtime/lua/loader.pdb"
  "../../runtime/lua/loader.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/loader.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
