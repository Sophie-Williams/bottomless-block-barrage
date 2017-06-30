#include "trace_state.hpp"
#include <string>

int main(int argc, char** argv)
{
    std::string trace = argv[1];

    TraceManager manager = read_trace_file(trace);
    for (const auto& trace : manager.GetTraces())
    {
        printf("frame: %d\ninput: %x\naddress: %04x (%d %d) = %02x\n", trace.frame, trace.input, trace.address, trace.y, trace.x, trace.value);
        for (unsigned int i = 0; i < 13; i++)
        {
            for (unsigned int j = 0; j < 6; j++)
            {
                printf("%08x ", trace.panels[j + i * 6]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

}
