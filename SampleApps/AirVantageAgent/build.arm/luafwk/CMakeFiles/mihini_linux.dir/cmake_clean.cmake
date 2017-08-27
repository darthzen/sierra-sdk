FILE(REMOVE_RECURSE
  "CMakeFiles/mihini_linux"
  "../runtime/lua/fdwrapper.lua"
  "../runtime/lua/posixsignal.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/mihini_linux.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
