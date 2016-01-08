# This needs work

clear
g++ src/prog.cpp -std=c++11 -l curl -o xcurl

./xcurl /tmp/test # Assuming that a JSON config file is located at /tmp/test
