uint64 timestamp                        # time since system start (microseconds)
uint64 timestamp_sample                 # the timestamp of the raw data (microseconds)

# Any of the channels may not be available and be set to NaN
# to indicate that it does not contain valid data.

# main_wing_extent [-1, 1]
# -1: shoulder closed, wrist closed;
# 0: shoulder opened, wrist closed;
# 1: shoulder opened, wrist opened;
float32 main_wing_extent # through aux1

# tail_extent [-1, 1]
# -1: tail closed ;
# 1: tail opened;
float32 tail_extent # through aux2
