# wq:rate_ctrl
1. control_allocator,
2. dshot
3. fmav_rate_control
4. vehicle_angular_velocity

# wq:nav_and_controllers
1. ekf2_selector, 300Hz
2. flight_mode_manager, 50Hz
3. land_detector, 100Hz
4. mc_att_control, 200Hz
5. mc_pos_control, 100Hz
6. mc_hover_thrust_estimator, 100Hz
7. sensors, 200Hz
8. vehicle_acceleration 200Hz
9. vehicle_air_data 28Hz
10. vehicle_gps_position 3.4Hz
11. vehicle_magnetometer, 20Hz

# parameters

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
