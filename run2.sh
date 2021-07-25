for side in 1000 10000; do
   for nw in 1 2 3 4 5 6 7 8 ; do
      echo $side $nw
      for ((iter = 0; iter <= 5; iter++)) do
        ./build/dyn_thread $side $side 5 $iter $nw >> ./results_mac/dyn_thread/${side}_${nw}.txt
      done
   done
done
