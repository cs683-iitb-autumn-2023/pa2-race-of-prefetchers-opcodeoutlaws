for file in $(ls /home/abhishekjagushte/Labs/COA/ass2/traces)
do 
    bin/champsim --warmup_instructions 25000000 --simulation_instructions 25000000 /home/abhishekjagushte/Labs/COA/ass2/traces/$file > logs/${file}_stream_throttling.txt
done
