# if [ $# -ne 1 ]; then
#     echo "Provide number of instructions as argument"
#     exit 1
# fi


# cat /dev/null > out.txt

# for file in $(ls /home/abhishekjagushte/Labs/COA/ass2/traces)
# do      
#     bin/champsim --warmup_instructions $1 --simulation_instructions $1 /home/abhishekjagushte/Labs/COA/ass2/traces/$file >> out.txt 
# done

# grep "***\|CPU 0 cumulative IPC\|cpu0_L2C PREFETCH  REQUESTED:\|MISS LATENCY"



for file in $(ls /home/abhishekjagushte/Labs/COA/ass2/traces)
do      
    bin/champsim --warmup_instructions $1 --simulation_instructions $1 /home/abhishekjagushte/Labs/COA/ass2/traces/$file >> out.txt 
done

grep  "CPU 0 cumulative IPC\|cpu0_L2C PREFETCH  REQUESTED:\|cpu0_L1D TOTAL\|cpu0_L2C TOTAL" out.txt
