#!\bin\bash
val=200
while(( val< 8000 ))
do
./parallel-pthreads.o $val
val=$[val+200]
done
