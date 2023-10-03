for file in $(ls /home/abhishekjagushte/Labs/COA/ass2/traces)
do     
    make clean
    ./config.sh champsim_config.json
    make
    
    bin/champsim --warmup_instructions 25000000 --simulation_instructions 25000000 /home/abhishekjagushte/Labs/COA/ass2/traces/$file > results/ip_stride/$file/no.txt
done