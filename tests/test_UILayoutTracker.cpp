#include "testutil.h"

#include "graphics/UILayoutTracker.h"

using namespace perfectpixel::graphics::_internal;
using namespace perfectpixel::bedrock;

GTEST_TEST(test_UILayoutTracker, BasicAbsToRel)
{
    UILayoutTracker tracker;

    tracker.makeRoot({20.0f, 10.0f});

    AssertVectorEq(Vector2{0.0f, 0.0f}, tracker.absToRel({0.0f, 0.0f}));
    AssertVectorEq(Vector2{0.5f, 1.0f}, tracker.absToRel({10.0f, 10.0f}));
    AssertVectorEq(Vector2{1.0f, 2.0f}, tracker.absToRel({20.0f, 20.0f}));
    AssertVectorEq(Vector2{-0.5f, -1.0f}, tracker.absToRel({-10.0f, -10.0f}));
}

GTEST_TEST(test_UILayoutTracker, BasicRelToAbs)
{
    UILayoutTracker tracker;

    tracker.makeRoot({20.0f, 10.0f});

    AssertVectorEq(Vector2{0.0f, 0.0f}, tracker.relToAbs({0.0f, 0.0f}));
    AssertVectorEq(Vector2{10.0f, 10.0f}, tracker.relToAbs({0.5f, 1.0f}));
    AssertVectorEq(Vector2{20.0f, 20.0f}, tracker.relToAbs({1.0f, 2.0f}));
    AssertVectorEq(Vector2{-10.0f, -10.0f}, tracker.relToAbs({-0.5f, -1.0f}));
}

GTEST_TEST(test_UILayoutTracker, NestedAbsToRel)
{
    UILayoutTracker tracker;

    tracker.makeRoot({20.0f, 10.0f});

    tracker.addLayout(
        {0.0, 0.5}, {0.5, 1.0}, UILayoutTracker::LayoutDirection::NONE, 1.0f);

    AssertVectorEq(Vector2{0.5f, 0.0f}, tracker.absToRel({5.0f, 5.0f}));
    AssertVectorEq(Vector2{1.0f, 1.0f}, tracker.absToRel({10.0f, 10.0f}));
}

GTEST_TEST(test_UILayoutTracker, NestedRelToAbs)
{
    UILayoutTracker tracker;

    tracker.makeRoot({20.0f, 10.0f});

    tracker.addLayout(
        {0.0, 0.5}, {0.5, 1.0}, UILayoutTracker::LayoutDirection::NONE, 1.0f);

    AssertVectorEq(Vector2{0.5f, 0.0f}, tracker.absToRel({5.0f, 5.0f}));
    AssertVectorEq(Vector2{1.0f, 1.0f}, tracker.absToRel({10.0f, 10.0f}));
}
