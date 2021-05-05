echo -----------------Compiling
mkdir -p build/
cd build/
cmake ../. -DCMAKE_BUILD_TYPE=Debug
make -j4

echo -----------------Starting
./illuzion --inputs="./data/yes.png" --outputs="./data/yes_r" --resize=100,x2