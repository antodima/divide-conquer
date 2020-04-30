# memory optimization and thread sticking
LD_PRELOAD=/usr/local/bin/libjemalloc.so taskset -c 0,1,2,3 ./main 1000 4
