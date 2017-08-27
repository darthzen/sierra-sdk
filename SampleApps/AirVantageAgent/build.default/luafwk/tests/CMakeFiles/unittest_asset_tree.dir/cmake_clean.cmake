FILE(REMOVE_RECURSE
  "CMakeFiles/unittest_asset_tree"
  "../../runtime/lua/tests/asset_tree.lua"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/unittest_asset_tree.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
