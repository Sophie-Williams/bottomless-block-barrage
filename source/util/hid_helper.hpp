#ifndef HID_HELPER_HPP
#define HID_HELPER_HPP

#include <3ds.h>
#include <map>
#include <memory>

#define KEY_SENTINEL 0xFFFFFFFFU

struct KeyRepeatItem
{
    int key;
    u64 frame = 0;
    int step = 0;
};

struct KeyRepeatStore
{
    KeyRepeatItem& get(int key);
    u64& get_frame(int key);
    int& get_step(int key);
    std::map<int, KeyRepeatItem> key_repeat_store_map;
};

bool hidKeyRepeat(int key, u64& old_time, unsigned int repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeat(KeyRepeatItem& kri, unsigned int repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(int key, u64& old_time, int& step, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(KeyRepeatItem& kri, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms, u32 fake_held = KEY_SENTINEL);

class InputDataSourceInterface
{
public:
    InputDataSourceInterface() {}
    virtual ~InputDataSourceInterface() {}
    virtual u32 trigger() const = 0;
    virtual u32 held() const = 0;
    virtual void update() = 0;
};

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
