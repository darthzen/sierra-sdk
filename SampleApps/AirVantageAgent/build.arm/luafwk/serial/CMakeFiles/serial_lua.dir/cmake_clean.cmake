FILE(REMOVE_RECURSE
  "CMakeFiles/serial_lua"
  "../../runtime/lua/serial.lua"
  "../../runtime/lua/at.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/serial_lua.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
