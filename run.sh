for side in 1000 10000; do
   for nw in 1 2 3 4 5 6 7 8 ; do
      echo $side $nw
      for ((iter = 0; iter <= 5; iter++)) do
         ./build/ff $side $side 5 $iter $nw 0 >> ./results_mac/parfor_ff/${side}_${nw}.txt
         ./build/ff $side $side 5 $iter $nw 1 >> ./results_mac/farm_ff/${side}_${nw}.txt
         ./build/thread $side $side 5 $iter $nw >> ./results_mac/thread/${side}_${nw}.txt
         ./build/dyn_thread $side $side 5 $iter $nw >> ./results_mac/dyn_thread/${side}_${nw}.txt
      done
   done
   for ((iter = 0; iter <= 5; iter++)) do
      ./build/seq $side $side 5 $iter >> ./results_mac/seq/${side}.txt
   done
done
