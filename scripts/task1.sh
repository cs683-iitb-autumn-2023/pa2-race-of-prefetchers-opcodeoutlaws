# make sure you initialize constexpr int PREFETCH_DEGREE = 1 in iip_stride.cc before running this script

mkdir logs

for file in $(ls /home/abhishekjagushte/Labs/COA/ass2/traces)
do 
    for i in 2 3 4 5 
    do
        prev=$((i-1))
        sed -i "s/constexpr int PREFETCH_DEGREE = $prev/constexpr int PREFETCH_DEGREE = $i/" prefetcher/ip_stride/ip_stride.cc
        
        make clean
        ./config.sh champsim_config.json
        make
        
        bin/champsim --warmup_instructions 25000000 --simulation_instructions 25000000 /home/abhishekjagushte/Labs/COA/ass2/traces/$file > logs/${file}_ip_stride_degree_$i.txt
        # echo "bin/champsim --warmup_instructions 25000000 --simulation_instructions 25000000 /home/abhishekjagushte/Labs/COA/ass2/traces/$file > logs/ip_stride/$file/${file}_ip_stride_degree_$i.txt"
    done
    sed -i "s/constexpr int PREFETCH_DEGREE = 5/constexpr int PREFETCH_DEGREE = 1/" prefetcher/ip_stride/ip_stride.cc
done
