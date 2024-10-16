CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -stdlib=libc++ -Wc++17-extensions -framework CoreFoundation -framework Carbon

driver: Driver.cpp Automation.o Translate.o
	$(CXX) $(CXXFLAGS) Automation.o Translate.o Driver.cpp -o driver

Automation.o: Automation.cpp Automation.h Translate.o
	$(CXX) $(CXXFLAGS) -c Automation.cpp

Translate.o: Translate.h Translate.cpp
	$(CXX) $(CXXFLAGS) -c Translate.cpp