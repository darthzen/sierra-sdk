FILE(REMOVE_RECURSE
  "CMakeFiles/test_luafwk"
  "../../runtime/lua/tests/bysant.lua"
  "../../runtime/lua/tests/luatobin.lua"
  "../../runtime/lua/tests/persist.lua"
  "../../runtime/lua/tests/rpc.lua"
  "../../runtime/lua/tests/sched.lua"
  "../../runtime/lua/tests/socket.lua"
  "../../runtime/lua/tests/logstore.lua"
  "../../runtime/lua/tests/crypto.lua"
  "../../runtime/lua/tests/posixsignal.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/test_luafwk.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
