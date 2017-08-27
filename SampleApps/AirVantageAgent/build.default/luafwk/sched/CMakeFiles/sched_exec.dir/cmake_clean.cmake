FILE(REMOVE_RECURSE
  "CMakeFiles/sched_exec"
  "../../runtime/lua/sched/exec.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/sched_exec.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
