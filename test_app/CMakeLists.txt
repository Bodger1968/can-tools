add_executable("can_trial" "Main.cpp")
target_link_libraries("can_trial" PRIVATE Can-Tools::Can-Tools )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
