#ifndef REPLAY_SIMULATION_HPP
#define REPLAY_SIMULATION_HPP

#include <cassert>
#include <cstdint>
#include <memory>
#include "panel_source.hpp"
#include "panel_table.hpp"
#include "trace_state.hpp"
#include "frame_state.hpp"

class ReplayPanelSource : public PanelSource
{
public:
    ReplayPanelSource(const std::vector<Panel::Type>& board, const std::vector<Panel::Type>& next);
    ~ReplayPanelSource() override {}
    std::vector<Panel::Type> board() override {return table;}
    Panel::Type panel() override {return Panel::Type::EMPTY;}
    std::vector<Panel::Type> line() override;
    void reset() {index = 0;}
private:
    std::vector<Panel::Type> table;
    std::vector<Panel::Type> next;
    int index;
};

class ReplaySimulation
{
public:
    ReplaySimulation() : frame(0) {}
    virtual ~ReplaySimulation() {}
    void Step();
    void Run(bool debug = false);
    uint32_t GetFrame() const {return frame;}

    virtual void Print() = 0;
    virtual bool Finished() const = 0;
protected:
    virtual void DoStep() = 0;
    uint32_t frame;
};

class TraceReplaySimulation : public ReplaySimulation
{
public:
    TraceReplaySimulation(const TraceManager& trace_manager, const PanelSpeedSettings& settings);
    void Print() override;
    bool Finished() const override {return frame > traces.GetFinalFrame();}
    const TraceState* GetTraceState() const {return traces.GetState(frame);}
    const TraceInput* GetInput() const {return traces.GetInput(frame);}
    const PanelTable& GetPanelTable() const {return *table;}
protected:
    void DoStep() override;
private:
    std::unique_ptr<PanelTable> table;
    TraceManager traces;
    TraceInput last_input;
};

class FrameReplaySimulation : public ReplaySimulation
{
public:
    FrameReplaySimulation(const FrameStateManager& frame_manager, const PanelSpeedSettings& settings);
    void Print() override;
    bool Finished() const override {return frame >= frames.GetFinalFrame();}
    const FrameState& GetState() const {return frames.GetState(frame);}
    const PanelTable& GetPanelTable() const {return *table;}
    void GetSelectorCoords(int& i, int& j) const;
protected:
    void DoStep() override;
private:
    std::unique_ptr<PanelTable> table;
    FrameStateManager frames;
    TraceInput last_input;
    MatchInfo info;
    int x;
    int y;
};

#endif
