FILE(REMOVE_RECURSE
  "CMakeFiles/sched_platform"
  "../../runtime/lua/sched/platform.lua"
  "../../runtime/lua/sched/fd.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/sched_platform.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
