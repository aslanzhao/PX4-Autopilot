# controller
A controller is a component that takes a setpoint and a measurement or estimated state (process variable) as input. Its goal is to adjust the value of the process variable such that it matches the setpoint. The output is a correction to eventually reach that setpoint. For example the position controller takes position setpoints as inputs, the process variable is the currently estimated position, and the output is an attitude and thrust setpoint that move the vehicle towards the desired position.

# mixer
A mixer takes force commands (three torque, three thrust) at body reference frame (fixed on the vehichle) and translates them into individual motor commands, while ensuring that some limits are not exceeded. This translation is specific for a vehicle type and depends on various factors, such as the motor arrangements with respect to the center of gravity, or the vehicle's rotational inertia.

# 名词解释
在 px4_task_spawn_cmd() 这个函数名中，单词 spawn 的含义是“创建”或“生成”的意思。在编程和操作系统领域，spawn 通常指代创建一个新的进程或线程。
