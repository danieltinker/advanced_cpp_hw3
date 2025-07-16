# How To Run [from root directory]:
1. make clean && make
2. ./simulator_315634022 \
    --comparative \
    --map ../maps/example.txt \
    --manager ../GameManager/libGameManager_315634022.so \
    --algos ../Algorithm/libAlgorithm_315634022.so \
    --threads 4 \
    --verbose 

# Or, equivalently, with the short flags:
    ./simulator_315634022 \
    -c \
    -m ../maps/example.txt \
    -g ../GameManager/libGameManager_315634022.so \
    -a ../Algorithm/libAlgorithm_315634022.so \
    -t 4 \
    -v