#ifndef REPLAY_SIMULATION_HPP
#define REPLAY_SIMULATION_HPP

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>

#include "input.hpp"
#include "panel_source.hpp"
#include "panel_table.hpp"
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

class FrameReplaySimulation
{
public:
    typedef std::function<bool(const FrameState&, const PanelTable&, uint32_t)> StepCallback;

    FrameReplaySimulation(const FrameStateManager& frame_manager, const PanelSpeedSettings& settings);
    void Run(bool debug = false);
    void Step(bool debug = false);
    bool Finished() const {return frame >= frames.GetFinalFrame();}
    void AddStepCallback(const StepCallback& callback) {callbacks.push_back(callback);}
    void Print();

    const FrameState& GetState() const {return frames.GetState(frame);}
    const PanelTable& GetPanelTable() const {return *table;}
    void GetSelectorCoords(int& i, int& j) const;
    uint32_t GetFrame() const {return frame;}
private:
    void DoStep();
    uint32_t frame;
    std::unique_ptr<PanelTable> table;
    FrameStateManager frames;
    InputManager input_manager;
    std::list<StepCallback> callbacks;
    MatchInfo info;
    int level;
    int next;
    int x;
    int y;
};

#endif
