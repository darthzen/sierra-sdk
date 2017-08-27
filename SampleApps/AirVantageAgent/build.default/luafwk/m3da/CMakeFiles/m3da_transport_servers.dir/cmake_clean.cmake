FILE(REMOVE_RECURSE
  "CMakeFiles/m3da_transport_servers"
  "../../runtime/lua/m3da/transport/httpserver.lua"
  "../../runtime/lua/m3da/transport/tcpserver.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/m3da_transport_servers.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
