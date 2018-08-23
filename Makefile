
CFLAGS = -std=c++11 -lstdc++ -Wall -Werror -g -I../src/ -L../

all:
    $(CXX) $(CFLAGS) *.cpp -o main -lsqlparser
    
clean:
    rm -f *.o main
