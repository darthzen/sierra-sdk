FILE(REMOVE_RECURSE
  "CMakeFiles/socket"
  "../../runtime/lua/ltn12.lua"
  "../../runtime/lua/mime.lua"
  "../../runtime/lua/socket.lua"
  "../../runtime/lua/socket/http.lua"
  "../../runtime/lua/socket/url.lua"
  "../../runtime/lua/socket/ftp.lua"
  "../../runtime/lua/socket/tp.lua"
  "../../runtime/lua/socket/smtp.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/socket.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
