FILE(REMOVE_RECURSE
  "CMakeFiles/racon_aliases"
  "../runtime/lua/system.lua"
  "../runtime/lua/airvantage.lua"
  "../runtime/lua/devicetree.lua"
  "../runtime/lua/sms.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/racon_aliases.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
