#define BOOST_TEST_MAIN
#if !defined(WINDOWS)
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <panel_table.hpp>

PanelSpeedSettings speed_settings = {1, 1, 1, 1, 1, 1, false};

void set_data(PanelTable& table, const int* data)
{
    for (unsigned int i = 0; i < table.panels.size(); i++)
        table.panels[i].value = (Panel::Type) data[i];
}

MatchInfo Update(PanelTable& table)
{
    MatchInfo ret = table.update(1, 4, false);
    //printf("%s\n", ret.str().c_str());
    //PrintPanelTable(table);
    return ret;
}

BOOST_AUTO_TEST_CASE(TestFall)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.swap(8, 0);
    BOOST_TEST_CHECKPOINT("Swapping (8,0) and (8,1)");

    Panel& swapee = table.get(8, 0);
    Panel& swaper = table.get(8, 1);


    // Swapping normal->swap
    BOOST_CHECK_EQUAL(swapee.state, Panel::State::LEFT_SWAP);
    BOOST_CHECK_EQUAL(swaper.state, Panel::State::RIGHT_SWAP);
    Update(table);

    BOOST_TEST_CHECKPOINT("Swapped (8,0) and (8,1)");
    BOOST_CHECK_EQUAL(swapee.state, Panel::State::SWAPPED);
    BOOST_CHECK_EQUAL(swaper.state, Panel::State::PENDING_FALL);
    Update(table);

    BOOST_TEST_CHECKPOINT("Fall (8,1)");
    // swapped -> idle
    BOOST_CHECK_EQUAL(swapee.state, Panel::State::IDLE);
    // swapped -> pending fall
    BOOST_CHECK_EQUAL(swaper.state, Panel::State::PENDING_FALL);

    BOOST_CHECK_EQUAL(swapee.value, Panel::Type::EMPTY);
    BOOST_CHECK_EQUAL(swaper.value, (Panel::Type) 2);
    Update(table);

    // pending fall -> falling
    BOOST_TEST_CHECKPOINT("Idle (8,1)");
    BOOST_CHECK_EQUAL(swaper.state, Panel::State::IDLE);

    // normal -> falling
    BOOST_TEST_CHECKPOINT("Falling (9,1)");
    Panel& lower = table.get(9, 1);
    BOOST_CHECK_EQUAL(lower.state, Panel::State::FALLING);

    Update(table);
    // fall end -> normal (swaper)
    BOOST_CHECK_EQUAL(lower.state, Panel::State::IDLE);

    Update(table);
    Panel& lower2 = table.get(10, 1);
    BOOST_CHECK_EQUAL(lower2.state, Panel::State::END_FALL);

    Update(table);
    BOOST_CHECK_EQUAL(lower2.state, Panel::State::IDLE_FELL);

    Update(table);
    BOOST_CHECK_EQUAL(lower2.state, Panel::State::IDLE);

    const int expected[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 2, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
    };

    for (int i = 0; i < table.height() + 1; i++)
    {
        for (int j = 0; j < table.width(); j++)
        {
            BOOST_CHECK_EQUAL(table.get(i, j).value, expected[j + i * table.width()]);
        }
    }
}

BOOST_AUTO_TEST_CASE(TestStackDown)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.swap(9, 0);

    Panel& swapee = table.get(9, 0);
    Panel& swaper = table.get(9, 1);

    // Swapping normal->swap
    BOOST_CHECK(swapee.is_left_swap());
    BOOST_CHECK(swaper.is_right_swap());
    Update(table);

    BOOST_CHECK(swapee.is_swapped());


    for (int i = 0; i < 50; i++)
    {
        Update(table);
        /*for (int i = 0; i < 72; i++)
        {
            printf("%d(%d) ", (int)table.panels[i].value, table.panels[i].state);
            if (i % 6 == 5) printf("\n");
        }
        //printf("\n\n");*/
    }
    const int expected[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        3, 0, 0, 0, 0, 0,
        2, 1, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
    };

    for (int i = 0; i < table.height() + 1; i++)
    {
        for (int j = 0; j < table.width(); j++)
        {
            BOOST_CHECK_EQUAL(table.get(i, j).value, expected[j + i * table.width()]);
        }
    }
}

BOOST_AUTO_TEST_CASE(TestUpdateMatches)
{
    std::unique_ptr<PanelTable> panel_table;
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 1, 1,
        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.update_matches();

    Panel& p1 = table.get(10, 0);
    Panel& p2 = table.get(10, 1);
    Panel& p3 = table.get(10, 2);

    BOOST_CHECK_EQUAL(p1.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::PENDING_MATCH);

    Update(table);
    BOOST_CHECK_EQUAL(p1.state, Panel::State::MATCHED);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::MATCHED);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::MATCHED);

    Update(table);
    BOOST_CHECK_EQUAL(p1.state, Panel::State::REMOVED);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::MATCHED);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::MATCHED);

    Update(table);
    BOOST_CHECK_EQUAL(p1.state, Panel::State::REMOVED);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::REMOVED);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::MATCHED);

    Update(table);
    BOOST_CHECK_EQUAL(p1.state, Panel::State::REMOVED);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::REMOVED);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::REMOVED);

    Update(table);
    const int expected[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1,
        8, 8, 8, 8, 8, 8,
    };

    for (int i = 0; i < table.height() + 1; i++)
    {
        for (int j = 0; j < table.width(); j++)
        {
            BOOST_CHECK_EQUAL(table.get(i, j).value, expected[j + i * table.width()]);
        }
    }
}



BOOST_AUTO_TEST_CASE(TestChains)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 2, 5, 8, 8,
        8, 8, 1, 4, 8, 8,
        8, 8, 1, 3, 8, 8,
        8, 2, 1, 2, 8, 8,
        8, 8, 8, 3, 8, 8,
        8, 5, 5, 3, 4, 4,

        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    MatchInfo a = table.update_matches();

    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 0);
    BOOST_CHECK(a.swap_match);
    BOOST_CHECK(!a.fall_match);

    a = Update(table);
    BOOST_CHECK_EQUAL(a.combo, 0);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 0);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(!a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 0);
    BOOST_CHECK_EQUAL(table.clink, 0);

    Update(table);
    Update(table);
    Update(table); // removal

    Update(table); // end match

    Update(table);
    Update(table);
    Update(table); // fall
    a = Update(table); // match
    Panel& p = table.get(8, 2);
    BOOST_CHECK(p.chain);
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 2);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 1);
    BOOST_CHECK_EQUAL(table.clink, 0);

    Update(table);
    Update(table);
    Update(table);
    Update(table);
    Update(table); // removed

    Update(table); // fall

    a = Update(table); // pending match
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 3);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 2);
    BOOST_CHECK_EQUAL(table.clink, 0);

    Update(table); // matched
    Update(table);
    Update(table);
    Update(table); // removal

    Update(table); // end match / pending fall

    Update(table); // idle being fall
    Update(table);
    Update(table); // fall

    a = Update(table); // end_fall ->match
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 4);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 3);
    BOOST_CHECK_EQUAL(table.clink, 0);

    Update(table); // matched
    Update(table);
    Update(table);
    Update(table); // removal

    Update(table); // end match / pending fall

    Update(table); // fall

    a = Update(table); // end_fall ->match
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 5);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 4);
    BOOST_CHECK_EQUAL(table.clink, 0);

    Update(table); // matched
    Update(table);
    Update(table);
    Update(table);
    Update(table); // removal
    Update(table); // now idle
    Update(table); // fall
    Update(table); // end fall
    Update(table); // idle fall

    a = Update(table); // end_fall ->match
    BOOST_CHECK_EQUAL(a.combo, 0);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 0);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(!a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 0);
    BOOST_CHECK_EQUAL(table.clink, 0);
}

BOOST_AUTO_TEST_CASE(TestGapNoMatch)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 1, 8, 8, 8,
        3, 3, 1, 3, 8, 8,
        8, 8, 1, 8, 8, 8,

        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.update_matches();

    Panel& p1 = table.get(10, 2);
    Panel& p2 = table.get(9, 2);
    Panel& p3 = table.get(8, 2);

    BOOST_CHECK_EQUAL(p1.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::PENDING_MATCH);

    Update(table); // matched
    Update(table); // removed, matched, matched
    Update(table); // removed, removed, matched
    Update(table); // removed, removed, removed
    Update(table); // end_match
    Update(table); // idle
    table.swap(9, 2);
    Update(table); // swapped / fall end
    Update(table); // pending fall
    Update(table); // falling
    Update(table); // falling
    Update(table); // fell / idle
    Update(table); // idle

    const int expected[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 0, 0, 8, 8,
        8, 8, 0, 8, 8, 8,
        8, 8, 8, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        3, 3, 2, 8, 8, 8,
        8, 8, 3, 8, 8, 8,

        8, 8, 8, 8, 8, 8,
    };

    for (int i = 0; i < table.height() + 1; i++)
    {
        for (int j = 0; j < table.width(); j++)
        {
            BOOST_CHECK_EQUAL(table.get(i, j).value, expected[j + i * table.width()]);
        }
    }
}


BOOST_AUTO_TEST_CASE(TestSkillChainSlip)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 1, 8, 8, 8,
        8, 8, 1, 2, 8, 8,
        8, 8, 1, 8, 8, 8,

        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.update_matches();

    Panel& p1 = table.get(10, 2);
    Panel& p2 = table.get(9, 2);
    Panel& p3 = table.get(8, 2);

    BOOST_CHECK_EQUAL(p1.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::PENDING_MATCH);

    Update(table); // matched
    Update(table); // removed, matched, matched
    Update(table); // removed, removed, matched
    Update(table); // removed, removed, removed
    Update(table); // end_match
    Update(table); // idle
    table.swap(9, 2);
    Update(table); // swapped / fall end
    Update(table); // pending fall *BUG* lose chain flag here
    Update(table); // falling

    MatchInfo a = Update(table); // match / fall end
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 2);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 1);
    BOOST_CHECK_EQUAL(table.clink, 0);
}

BOOST_AUTO_TEST_CASE(TestSkillChainSlip2)
{
    const int columns = 6;
    const int rows = 11;
    const int colors = 6;
    const int data_size = columns * (rows + 1);
    const int data[data_size] = {
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 2, 8, 8, 8,
        8, 8, 1, 8, 8, 8,
        8, 8, 1, 8, 8, 8,
        8, 8, 1, 2, 8, 8,

        8, 8, 8, 8, 8, 8,
    };
    PanelTable table(rows, columns, colors, speed_settings);
    set_data(table, data);
    table.type = PanelTable::Type::PUZZLE;
    table.moves = 100;

    table.update_matches();

    Panel& p1 = table.get(10, 2);
    Panel& p2 = table.get(9, 2);
    Panel& p3 = table.get(8, 2);

    BOOST_CHECK_EQUAL(p1.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p2.state, Panel::State::PENDING_MATCH);
    BOOST_CHECK_EQUAL(p3.state, Panel::State::PENDING_MATCH);

    Update(table); // matched
    Update(table); // removed, matched, matched
    Update(table); // removed, removed, matched
    Update(table); // removed, removed, removed
    Update(table); // end_match
    Update(table); // idle
    table.swap(10, 2);
    Update(table); // swapped / fall end

    MatchInfo a = Update(table); // pending fall *BUG* lose chain flag here
    BOOST_CHECK_EQUAL(a.combo, 3);
    BOOST_CHECK_EQUAL(a.clink, 0);
    BOOST_CHECK_EQUAL(a.chain, 2);
    BOOST_CHECK(!a.swap_match);
    BOOST_CHECK(a.fall_match);
    BOOST_CHECK_EQUAL(table.chain, 1);
    BOOST_CHECK_EQUAL(table.clink, 0);
}
