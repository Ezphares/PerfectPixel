#include <gtest/gtest.h>

#include <EntityComponentSystem/Entity.h>

using namespace perfectpixel::ecs;

GTEST_TEST(test_Entity, CreateGet)
{
    Entity entity_0_0 = entityCreate(0, 0);
    ASSERT_EQ(0u, entity_0_0.generation);
    ASSERT_EQ(0u, entity_0_0.index);

    Entity entity_100_0 = entityCreate(100, 0);
    ASSERT_EQ(100u, entity_100_0.generation);
    ASSERT_EQ(0u, entity_100_0.index);

    Entity entity_0_25 = entityCreate(0, 25);
    ASSERT_EQ(0u, entity_0_25.generation);
    ASSERT_EQ(25u, entity_0_25.index);
}

GTEST_TEST(test_Entity, Overflow)
{
    uint32_t generationMax = (1 << GENERATION_BITS) - 1;
    Entity entity_1_0      = entityCreate(generationMax + 2, 0);
    ASSERT_EQ(1u, entity_1_0.generation);
    ASSERT_EQ(0u, entity_1_0.index);

    uint32_t indexMax = (1 << INDEX_BITS) - 1;
    Entity entity_0_1 = entityCreate(0, indexMax + 2);
    ASSERT_EQ(0u, entity_0_1.generation);
    ASSERT_EQ(01, entity_0_1.index);
}
