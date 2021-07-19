for side in 1000 10000; do
   for nw in 1 2 3 4 5 6 7 8 ; do
      echo $side $nw
      for ((iter = 0; iter <= 5; iter++)) do
         ./build/ff $side $side 5 $iter $nw 0 >> ./results_mac/ff_${side}_${nw}.txt
         ./build/ff $side $side 5 $iter $nw 1 >> ./results_mac/poolff_${side}_${nw}.txt
         ./build/thread $side $side 5 $iter $nw >> ./results_mac/thread_${side}_${nw}.txt
         ./build/threadpool $side $side 5 $iter $nw >> ./results_mac/threadpool_${side}_${nw}.txt
      done
   done
   for ((iter = 0; iter <= 5; iter++)) do
      ./build/seq $side $side 5 $iter >> ./results_mac/seq_${side}.txt
   done
done
