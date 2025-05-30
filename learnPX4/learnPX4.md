# 常见错误
1. 编译的时候不要用sudo，也就是不要用root权限，否则容易创建一些目录是属于root的。
2. bootloader烧录的时候要烧录bin文件，它小一些，地址在0x8000000。
3. upload可以用make ***_default upload，如果遇到waiting for bootloader，那是以为只有硬件只在上电的时候检查，可以重新插拔上电解决。
4. usb线要注意有的线只是充电线，没有焊接数据线，可以用lsusb查看，如果看不到设备，大概率就意味着这根线有问题，不要浪费时间，换一根好一点的线。

# 参数文件
microair默认把参数文件存储在片上flash上，为了把它改到sd卡里，首先需要去掉了boards/micoair/h743/board_config.h中的
// #define FLASH_BASED_PARAMS
这样可以在mavlink console里利用param status命令查看参数文件的位置，默认应该是/fs/mtd_params，这个mtd似乎是一个片上硬件，不知道是啥，为了把它改到sd卡上，在board/micoair/h741/default.px4board里增加
CONFIG_BOARD_PARAM_FILE="/fs/microsd/params"
这个配置了param_file的位置在sd卡上。


# nsh
To see what is running in the work queues and at what rate, use:
``` bash
work_queue status
```
To debug uORB topics:
``` bash
uorb top
```
To inspect a specific uORB topic:
``` bash
listener <topic_name>
```

# controller
A controller is a component that takes a setpoint and a measurement or estimated state (process variable) as input. Its goal is to adjust the value of the process variable such that it matches the setpoint. The output is a correction to eventually reach that setpoint. For example the position controller takes position setpoints as inputs, the process variable is the currently estimated position, and the output is an attitude and thrust setpoint that move the vehicle towards the desired position.

# mixer
A mixer takes force commands (three torque, three thrust) at body reference frame (fixed on the vehichle) and translates them into individual motor commands, while ensuring that some limits are not exceeded. This translation is specific for a vehicle type and depends on various factors, such as the motor arrangements with respect to the center of gravity, or the vehicle's rotational inertia.

# 名词解释
在 px4_task_spawn_cmd() 这个函数名中，单词 spawn 的含义是“创建”或“生成”的意思。在编程和操作系统领域，spawn 通常指代创建一个新的进程或线程。

# Remote controller
src/modules/rc_update工程里发布manual_control_setpoint。对变体翼，可以在这里再定义一个表达变构型的参量，然后在control allocator里订阅，改变servos的trim值，也就是_actuator_trim。同时，是不是在这里就可以根据trim的值，改变切线刚度矩阵？

# morphing wing 的参数

## CA_ROTOR0_CT (FLOAT)

Thrust coefficient of rotor 0.

The thrust coefficient if defined as Thrust = CT * u^2, where u (with value between actuator minimum and maximum) is the output signal sent to the motor controller.
| Reboot | minValue | maxValue | increment | default | unit |
| 	 | 0	    | 100      | 1         | 6.5     |      |

## CA_ROTOR0_KM (FLOAT)

Moment coefficient of rotor 0.

The moment coefficient if defined as Torque = KM * Thrust. Use a positive value for a rotor with CCW rotation. Use a negative value for a rotor with CW rotation.
| Reboot | minValue | maxValue | increment | default | unit |
| 	 | -1	    | 1        | 0.01      | 0.05    |      |
