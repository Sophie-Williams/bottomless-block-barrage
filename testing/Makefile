LIBS = -lboost_unit_test_framework
SOURCE := ../source
CPPFLAGS := -Wall -I$(SOURCE) -std=c++11 -fpermissive -g


panel_table_test : panel_table_test.o panel_table.o panel.o
	g++ $^ $(CPPFLAGS) -o $@ $(LIBS)

replay : replay_traces.o trace_state.o panel_table.o panel.o
	g++ $^ $(CPPFLAGS) -o $@

panel_table_test.o : panel_table_test.cpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp $(SOURCE)/random.hpp
replay_traces.o : replay_traces.cpp trace_state.hpp
trace_state.o : trace_state.cpp trace_state.hpp

panel_table.o : $(SOURCE)/panel_table.cpp $(SOURCE)/panel_table.hpp $(SOURCE)/panel.hpp $(SOURCE)/random.hpp
	g++ -c $(CPPFLAGS) $<
panel.o : $(SOURCE)/panel.cpp $(SOURCE)/panel.hpp $(SOURCE)/random.hpp
	g++ -c $(CPPFLAGS) $<

clean :
	rm -rf panel_table_test.o panel_table.o panel.o panel_table_test replay replay_traces.o trace_state.o
