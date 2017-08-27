FILE(REMOVE_RECURSE
  "CMakeFiles/platformserver"
  "../../runtime/lua/platform/backend.lua"
  "../../runtime/lua/platform/favicon.lua"
  "../../runtime/lua/platform/init.lua"
  "../../runtime/lua/platform/pages.lua"
  "../../runtime/lua/platform/response_templates.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/platformserver.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
