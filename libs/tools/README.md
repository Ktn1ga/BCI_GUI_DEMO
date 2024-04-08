# **build_lib.cmake编译脚本**  
## **调用方式**  
~~~cmake
set(_module_path "${CMAKE_CURRENT_SOURCE_DIR}/../../libs")  
include(${_module_path}/tools/build_lib.cmake)  
project_share_lib(${_module_path} 
                SRCS main.c SRC_DIRS src2
                INCLUDE_DIRS include2
                REQUIRES osal cJSON)  
~~~
## **目录结构-开发模块**  
~~~
    ├── build
│   ├── CMakeCache.txt
│   ├── libtrial_manager.so
│   └── Makefile
├── libs
│   ├── dll
│   │   ├── linux
│   │   │   ├── cJSON
│   │   │   ├── osal
│   │   │   └── trial_manager
│   │   └── windows
│   │   │   ├── mingw/cJSON
│   │   │   ├── mingw/osal
│   │   │   └── mingw/trial_manager
│   ├── include
│   │   ├── cJSON
│   │   ├── osal
│   │   ├── pthread_win32
│   │   └── trial_manager
│   └── tools
│       ├── build_lib.cmake
│       ├── format-minimal.sh
│       └── format.sh
└── module
    └── trial-manager
        ├── build
        ├── CMakeLists.txt
        ├── include
        ├── src
        └── test
~~~   
## **脚本总体流程(以trial-manager为例)**  
+ project_share_lib(libs_path 源文件、模块依赖文件)  
  + 获得_module_name:trial-manager  
  + share_lib_add(_module_name libs_path)确定系统平台_platform_name和编译器_compiler_name 
  + _share_lib_add(module_name libs_path)
    + 定义一些选择、单值和多值关键词并赋值
    + 通过宏__parse_sources(sources)获取参与编译的源文件，保存在列表sources中  
    + _get_version(新版本变量  trial-manager/version.txt路径)获取新版本号  
    + _get_version(旧版本变量 libs/dll/_platform_name/_compiler_name/trial-manager/version.txt路径)获取旧版本号 
    + __add_dependencys_path(libs/dll/_platform_name/_compiler_name REQUIRES)指定Public依赖
    + __add_dependencys_path(libs/dll/_platform_name/_compiler_name __PRIV_REQUIRES)指定Private依赖 
    + add_executable()将指定的源文件来生成目标可执行文件  
    + __add_include_dirs(lib type dir)指定项目lib需要包含的头文件路径  
    + __add_dependencys(lib dirs depds type)指定项目lib链接的库  
    + install()命令指定安装路径和文件  
+ project_executable(libs_path )  
  + 获得_module_name
  + executable_add(_module_name libs_path)确定系统平台_platform_name和编译器_compiler_name 
  + _executable_add(module_name libs_path)  
    + 获取参与编译的源文件  
    + 获取版本号  
    + 指定依赖和头文件路径  
    + 安装install()
## **各个宏macro作用**  
### **macro(__parse_sources sources)**  
+ 当__SRC_DIRS和__SRCS都被指定时，sources为__SRCS中每个源文件的基本目录＋文件名  
+ 当__SRCS未指定__SRC_DIRS被指定时，sources为__SRC_DIRS中所有.c/.cpp/.S文件的基本目录＋文件名   
+ 删除sources中出现在__EXCLUDE_SRCS中的每个文件路径  
+ 去除sources中的重复元素  
### **macro(__add_include_dirs lib dirs type)**  
+ 获取dirs中每个头文件基本目录+文件名  
+ 通过target_include_directories(lib type dir)指定项目lib需要包含的头文件路径  
### **macro(__add_dependencys_path dirs depds)**  
输入：编译器路径 依赖  
获取depds中的每个动态链接库路径，通过link_directories命令指定第三方库所在路径
### **macro(__add_dependencys lib dirs depds type)**  
输入：lib名 lib路径 依赖 type  
对于每个依赖，通过target_link_libraries()命令指定目标链接的库 
+ PUBLIC:在public后面的库会被link到target中，并且里面的符号也会被导出，提供给第三方使用  
+ PRIVATE:在private后面的库仅被link到target中，第三方不能感知调了什么库  
+ INTERFACE:在interface后面引入的库不会被链接到target中，只会导出符号
### **macro(share_lib_add module_name libs_path)**  
输入：模块名称 libs路径
获取系统平台_platform_name和编译器_compiler_name  
### **macro(executable_add module_name libs_path)**  
输入：模块名称 libs路径  
获取系统平台_platform_name和编译器_compiler_name  
### **macro(project_share_lib libs_path)**  
输入：libs路径
获取模块名称_module_name
### **macro(project_executable libs_path)**   
输入：libs路径  
获取模块名称_module_name
### **function(_get_version version file_path)**  
输入：版本号变量  version.txt路径  
获取version.txt中的版本号  


