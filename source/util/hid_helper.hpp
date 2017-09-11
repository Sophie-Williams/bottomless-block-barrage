#ifndef HID_HELPER_HPP
#define HID_HELPER_HPP

#include <3ds.h>
#include <map>
#include <memory>
#include "input_data_source_interface.hpp"

#define KEY_SENTINEL 0xFFFFFFFFU

struct KeyRepeatItem
{
    u32 key;
    u64 frame = 0;
    int step = 0;
};

struct KeyRepeatStore
{
    KeyRepeatItem& get(u32 key);
    u64& get_frame(u32 key);
    int& get_step(u32 key);
    std::map<u32, KeyRepeatItem> key_repeat_store_map;
};

bool hidKeyRepeat(u32 key, u64& old_time, u32 repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeat(KeyRepeatItem& kri, u32 repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(u32 key, u64& old_time, int& step, u32 repeat_ms, int triggers_until_quick, u32 repeat_quick_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(KeyRepeatItem& kri, u32 repeat_ms, int triggers_until_quick, u32 repeat_quick_ms, u32 fake_held = KEY_SENTINEL);

class InputSource
{
public:
    InputSource(InputDataSourceInterface* source) : data_source(source) {}
    void set_data_source(InputDataSourceInterface* source) {data_source = source;}
    u32 trigger() const {return data_source->trigger();}
    u32 held() const {return data_source->held();}
    void update() {return data_source->update();}

    bool trigger(u32 key) const {return (trigger() & key) == key;}
    bool held(u32 key) const {return (held() & key) == key;}
    bool repeat(u32 key, u32 repeat_ms)
    {
        return hidKeyRepeat(repeat_store.get(key), repeat_ms, held());
    }
    bool repeat_quick(u32 key, u32 repeat_ms, u32 triggers_until_quick, u32 repeat_quick_ms)
    {
        return hidKeyRepeatQuick(repeat_store.get(key), repeat_ms, triggers_until_quick, repeat_quick_ms, held());
    };
private:
    KeyRepeatStore repeat_store;
    // Not owned
    InputDataSourceInterface* data_source;
};

class InputDataSource : public InputDataSourceInterface
{
public:
    InputDataSource() {}
    ~InputDataSource() {}
    u32 trigger() const override {return trigger_;}
    u32 held() const override {return held_;}
    void update() override
    {
        held_ = hidKeysHeld();
        trigger_ = hidKeysDown();
    }
private:
    u32 held_;
    u32 trigger_;
};

#endif
