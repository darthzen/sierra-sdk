FILE(REMOVE_RECURSE
  "CMakeFiles/sched"
  "../../runtime/lua/sched/init.lua"
  "../../runtime/lua/sched/timer.lua"
  "../../runtime/lua/sched/pipe.lua"
  "../../runtime/lua/sched/lock.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/sched.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
