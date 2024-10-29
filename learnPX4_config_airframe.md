# 添加一个新的airframe
PX4编译出的firmware里，以px4为后缀的文件里，含有配置信息，用QGC烧录firmware的时候，它提示解压了airframe.xml和parameter.xml。也就是在PX4的代码里就能配制出在QGC需要用到的参数配置界面。

核心是改两个工程
1. ROMFS/px4fmu_common/init.d
2. src/modulues/control_allocator

对第一个工程的修改包含四处
1. init.d/airframes 下添加一个新的airframe，比如22001_morphibing_bird
2. init.d/airframes/CMakeLists.txt，合适的模块下添加22001_morphibing_bird
3. init.d下添加rc.mb_defaults
4. init.d/CMakeLists.txt，合适的模块下添加rc.mb_defaults

对第二个工程的修改包含三处
1. control_allocator/module.yaml下修改Control Surface和CA_AIRFRAME
2. 修改control_allocator的几个源文件
