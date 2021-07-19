for side in 1000 10000; do
   for nw in 1 2 3 4 5 6 7 8 ; do
      echo $side $nw
      for ((iter = 0; iter <= 5; iter++)) do
        ./build/threadpool $side $side 5 $iter $nw >> ./results_mac/threadpool_${side}_${nw}.txt
        ./build/ff $side $side 5 $iter $nw 1 >> ./results_mac/poolff_${side}_${nw}.txt
      done
   done
done
