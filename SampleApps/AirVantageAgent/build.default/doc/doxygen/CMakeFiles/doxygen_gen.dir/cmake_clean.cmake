FILE(REMOVE_RECURSE
  "CMakeFiles/doxygen_gen"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/doxygen_gen.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
