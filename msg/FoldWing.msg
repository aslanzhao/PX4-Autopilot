uint64 timestamp # time since system start (microseconds)

int8 WING_FOLD = 1 # fold wing
int8 WING_UNFOLD = -1 # unfold wing
int8 WING_KEEP = 0 # keep the current state

int8 fold_wing	# 可以赋值为上面的几个 WING_FOLD, WING_UNFOLD, WING_KEEP
