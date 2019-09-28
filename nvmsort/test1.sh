chmod -R 777 *
rm -rf *10000.txt
rm -rf *100000.txt
rm -rf *1000000*
rm -rf *10000000*
g++ -std=c++11 DataGen.cpp -o DataGen
make

./DataGen 100000000
./main 0.2 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "0"
./main 0.25 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "1"
./main 0.3 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "2"
./main 0.35 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "3"
./main 0.4 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "4"
./main 0.45 "Randomdata100000000.txt">>Randomresult100000000.txt
echo "5"