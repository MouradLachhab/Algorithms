for algo in {"brute","seuil"}; do
    for ex in $(ls testset1); do
        size=$(echo $ex | cut -d_ -f2)
        t=$(./tp.sh -e ./testset1/${ex} -a $algo -t)
        echo $algo,$size,$t
    done
done >> results.csv