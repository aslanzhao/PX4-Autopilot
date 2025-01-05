# work queue

## wq:rate_ctrl
1. control_allocator,
2. dshot
3. fmav_rate_control
4. vehicle_angular_velocity

## wq:nav_and_controllers
1. ekf2_selector, 300Hz
2. flight_mode_manager, 50Hz
3. land_detector, 100Hz
4. fmav_att_control, 200Hz
5. fmav_pos_control, 100Hz,
6. fmav_hover_thrust_estimator, 100Hz
7. sensors, 200Hz
8. vehicle_acceleration 200Hz
9. vehicle_air_data 28Hz
10. vehicle_gps_position 3.4Hz
11. vehicle_magnetometer, 20Hz

## fmav_pos_control
pusblish vehicle_local_position_setpoint, vehicle_attitude_setpoint, takeoff_status
response to vehicle_local_position,
如果没有GPS信号来定位位置，只有IMU积分，位置的误差非常的大，这时候不应该激活位置控制模块，相应的flight_mode_manager，fmav_pos_control，fmav_hover_thrust_estimator这三个都可以不激活。

如果不激活位置环，那么只有attitude环和rate环被激活

## fmav_att_control
attitude controller，它订阅了并自动响应vehicle_attitude（由EKF2发布），这里是通过IMU积分得到的，它的更新频率由IMU_INTEG_RATE决定。

在manual模式下，attitude的输入由遥控杆输入，其中遥控杆输入的pitch和roll信号是vehicle的角度，而yaw输入的则是角速度。

## fmav_rate_control
rate controller，它订阅并自动响应vehicle_angular_velocity（有EKF2的IMU发布），并发布vehicle_thrust_setpoint, vehicle_torque_setpoint

## control_allocator
它订阅并自动响应vehicle_thrust_setpoint, vehicle_torque_setpoint，也就是说fmav_rate_control发布了新的推力和力矩相应需求后，control_allocator就自动响应。

# actuators
servo接收pwm波，50Hz，脉宽从500到2500us，平均值是1500，1500被设置为中位，trim是相对于1500设置的，-1时对应500，+1时对应2500。

# parameters
https://docs.px4.io/main/en/advanced_config/parameter_reference.html

## gyro rate
IMU_GYRO_RATEMAX,  Gyro control data maximum publication rate (inner loop rate) , [100, 2000]Hz

## disable magnetometer
SYS_HAS_MAG = 0, Control if the vehicle has a magnetometer

Comment: Set this to 0 if the board has no magnetometer. If set to 0, the preflight checks will not check for the presence of a magnetometer, otherwise N sensors are required.

Reboot required: true

## Auto-Disarming
By default vehicles will automatically disarm on landing, or if you take too long to take off after arming. The feature is configured using the following timeouts.

COM_DISARM_LAND, Time-out for auto disarm after landing. Default: 2s (-1 to disable).
COM_DISARM_PRFLT, Time-out for auto disarm if too slow to takeoff. Default: 10s (<=0 to disable).

## pitch和roll的最大倾斜角度
MPC_MAN_TILT_MAX, 设置pitch和roll的最大倾斜角度

# 分析工具
ulog文件在每次arm的时候自动记录在sd卡上，可以利用PlotJuggler来分析。在ubuntu下，plotjuggler可以通过snap直接安装
```bash
sudo snap install plotjuggler
```
但plotjuggler没有访问sd卡的权限，需要把分析的文件拷贝到比如Download这样的文件夹下，让它能打开文件。

plotjuggler有的功能不少，能对比曲线，还能分析fft。

# 手动伺服
新增加了一个manual_servo_control的工程，里面有个参数：MAN_SERVO_EN，设置为大于0的数，手柄的输入是力矩。设置为零，则手动的输入是角度。
