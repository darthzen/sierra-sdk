FILE(REMOVE_RECURSE
  "CMakeFiles/agent_devman_cmds"
  "../../../../runtime/lua/agent/devman/cmds/ExecuteScript.lua"
  "../../../../runtime/lua/agent/devman/cmds/LogUpload.lua"
  "../../../../runtime/lua/agent/devman/cmds/ReadNode.lua"
  "../../../../runtime/lua/agent/devman/cmds/SoftwareUpdate.lua"
  "../../../../runtime/lua/agent/devman/cmds/TCPRemoteConnect.lua"
  "../../../../runtime/lua/agent/devman/cmds/appcon.lua"
  "../../../../runtime/lua/agent/devman/cmds/ResetToFactoryDefault.lua"
  "../../../../runtime/lua/agent/devman/cmds/Reboot.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/agent_devman_cmds.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
