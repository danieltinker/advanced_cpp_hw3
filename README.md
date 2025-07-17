# How To Run [from root directory]:
1. make clean && make

Usage: simulator_<ID> <-comparative|-competition>
       game_map=<file> | game_maps_folder=<dir>
       game_managers_folder=<dir> | game_manager=<file>
       algorithm1=<so> algorithm2=<so> | algorithms_folder=<dir>
       [num_threads=<N>] [--verbose]

# Competition Mode:
./simulator_315634022 \
  --competition \
  game_maps_folder=../maps/ \
  game_manager=../GameManager/sos/libGameManager_315634022.so \
  algorithms_folder=../Algorithm/sos \
  num_threads=4 \
  --verbose
# Comparative Mode:
./simulator_315634022 \
  --comparative \
  game_map=../maps/example.txt \
  game_managers_folder=../GameManager/sos/ \                   
  algorithm1=../Algorithm/sos/libAlgorithm_315634022.so \
  algorithm2=../Algorithm/sos/libAlgorithmAlt_315634022.so \
  num_threads=4 \
  --verbose

