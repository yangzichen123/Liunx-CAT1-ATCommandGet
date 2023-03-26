cmake_minimum_required(VERSION 3.0)

# 项目名称
project(UartAtMoudleGetInfo)

# 设置 C 语言版本为 C11
set(CMAKE_C_STANDARD 11)

#编译选项
add_compile_options(-std=c11 -Wundef -Wshadow -Wconversion -Werror=return-type -Werror=implicit-function-declaration)

# 添加源文件
aux_source_directory(./src SRC_LIST)
# 添加头文件
include_directories("include/")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST})

# 设置可执行文件的输出路径
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

