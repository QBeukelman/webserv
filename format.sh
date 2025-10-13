find inc -iname '*.hpp' | xargs clang-format -i;
find src -iname '*.cpp' | xargs clang-format -i;