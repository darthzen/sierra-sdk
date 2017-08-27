FILE(REMOVE_RECURSE
  "CMakeFiles/m3da_transport"
  "../../runtime/lua/m3da/transport/http.lua"
  "../../runtime/lua/m3da/transport/tcp.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/m3da_transport.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
