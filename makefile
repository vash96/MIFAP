EASYLOCAL = /home/vash/Git/easylocal-3

SOURCE_FOLDER = src

SOURCE_FILES = $(wildcard $(SOURCE_FOLDER)/*.cc)
OBJECT_FILES = $(SOURCE_FILES:.cc=.o)
DEPENDENCIES = $(OBJECT_FILES:.o=.d)

# FLAGS = -std=c++17 -Wall -Wfatal-errors -O3
FLAGS = -std=c++17 -DNDEBUG -O3
CXXFLAGS = -MMD -I$(EASYLOCAL)/include $(FLAGS)
LINKOPTS = -lboost_program_options -pthread

main: $(OBJECT_FILES)
	g++ -o $@ $^ $(LINKOPTS)

-include $(DEPENDENCIES)

clean:
	rm -f $(OBJECT_FILES) main

cleandep:
	rm -f $(DEPENDENCIES)

cleanall: clean cleandep

.PHONY: clean cleandep cleanall