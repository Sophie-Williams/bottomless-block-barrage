LIBS = -lboost_unit_test_framework
SOURCE := ../source
CPPFLAGS := -Wall -I$(SOURCE) -std=c++11 -fpermissive -g

all : panel_table_test replay replay_test

panel_table_test : panel_table_test.o panel_source.o panel_table.o panel.o
	g++ $^ $(CPPFLAGS) -o $@ $(LIBS)

replay : replay.o replay_simulation.o trace_state.o panel_source.o panel_table.o panel.o
	g++ $^ $(CPPFLAGS) -o $@

replay_test : replay_test.o replay_simulation.o trace_state.o panel_source.o panel_table.o panel.o
	g++ $^ $(CPPFLAGS) -o $@ $(LIBS)


panel_table_test.o : panel_table_test.cpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp $(SOURCE)/panel_source.hpp
replay.o : replay.cpp trace_state.hpp replay_simulation.hpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp $(SOURCE)/panel_source.hpp
replay_test.o : replay_test.cpp trace_state.hpp replay_simulation.hpp
replay_simulation.o : replay_simulation.cpp replay_simulation.hpp trace_state.hpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp $(SOURCE)/panel_source.hpp
trace_state.o : trace_state.cpp trace_state.hpp

# Sources don't exist in the current directory so a rule is given.
panel_source.o : $(SOURCE)/panel_source.cpp $(SOURCE)/panel_source.hpp  $(SOURCE)/panel.hpp
	g++ -c $(CPPFLAGS) $<
panel_table.o : $(SOURCE)/panel_table.cpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp
	g++ -c $(CPPFLAGS) $<
panel.o : $(SOURCE)/panel.cpp $(SOURCE)/panel.hpp
	g++ -c $(CPPFLAGS) $<

clean :
	rm -rf panel_table_test.o panel_table.o panel_source.o panel.o panel_table_test replay replay.o replay_simulation.o trace_state.o
