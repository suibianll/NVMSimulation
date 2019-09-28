
rm -rf *10000.txt
rm -rf *100000.txt
g++ -std=c++11 DataGen.cpp -o DataGen
make
echo "10000, Ramdom"
./DataGen 10000

./main 0.05 "Randomdata10000.txt" >>Randomresult10000.txt
echo "0"
./main 0.1 "Randomdata10000.txt">>Randomresult10000.txt
echo "1"
./main 0.15 "Randomdata10000.txt">>Randomresult10000.txt
echo "2"
./main 0.2 "Randomdata10000.txt">>Randomresult10000.txt
echo "3"
./main 0.25 "Randomdata10000.txt">>Randomresult10000.txt
echo "4"
./main 0.3 "Randomdata10000.txt">>Randomresult10000.txt
echo "5"
echo "10000, PartSort"
./main 0.05 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "0"
./main 0.1 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "1"
./main 0.15 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "2"
./main 0.2 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "3"
./main 0.25 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "4"
./main 0.3 "PartSortdata10000.txt">>PartSortresult10000.txt
echo "5"

echo "100000, RamdomMem"
./DataGen 100000

./main 0.05 "Randomdata100000.txt">>Randomresult100000.txt
echo "0"
./main 0.1 "Randomdata100000.txt">>Randomresult100000.txt
echo "1"
./main 0.15 "Randomdata100000.txt">>Randomresult100000.txt
echo "2"
./main 0.2 "Randomdata100000.txt">>Randomresult100000.txt
echo "3"
./main 0.25 "Randomdata100000.txt">>Randomresult100000.txt
echo "4"
./main 0.3 "Randomdata100000.txt">>Randomresult100000.txt
echo "5"

echo "100000, SegMem"
./main 0.05 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "0"
./main 0.1 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "1"
./main 0.15 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "2"
./main 0.2 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "3"
./main 0.25 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "4"
./main 0.3 "PartSortdata100000.txt">>PartSortresult100000.txt
echo "5"