FILE(REMOVE_RECURSE
  "CMakeFiles/web"
  "../../runtime/lua/web/server.lua"
  "../../runtime/lua/web/auth.lua"
  "../../runtime/lua/web/compile.lua"
  "../../runtime/lua/web/session.lua"
  "../../runtime/lua/web/template.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/web.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
