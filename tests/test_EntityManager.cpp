#include "gtest/gtest.h"

#include "ecs/EntityManager.h"

using namespace perfectpixel::ecs;

GTEST_TEST(test_EntityManager, CreateAliveKill)
{
	EntityManager manager;

	Entity e1 = manager.create();
	ASSERT_TRUE(manager.isAlive(e1));
			
	Entity e2 = manager.create();
	ASSERT_TRUE(manager.isAlive(e2));
			
	manager.kill(e1);
	ASSERT_FALSE(manager.isAlive(e1));
	ASSERT_TRUE(manager.isAlive(e2));
			
	manager.kill(e2);
	ASSERT_FALSE(manager.isAlive(e2));

}

GTEST_TEST(test_EntityManager, ReuseDefault)
{
	EntityManager manager;

	Entity e1 = manager.create();
	manager.kill(e1);
	Entity e2 = manager.create();
	ASSERT_NE(e1.index, e2.index);
}

GTEST_TEST(test_EntityManager, ReuseInstant)
{
	EntityManager manager(0);

	Entity e_instant1 = manager.create();
	manager.kill(e_instant1);
	Entity e_instant2 = manager.create();

	ASSERT_EQ(e_instant1.index, e_instant2.index);
	ASSERT_EQ(e_instant1.generation + 1, e_instant2.generation);
}

GTEST_TEST(test_EntityManager, ReuseSettings)
{
	EntityManager manager(10);

	Entity start = manager.create();
	manager.kill(start);

	// Cycle 10 indices before reusing
	for (int i = 0; i < 10; ++i)
	{
		Entity intermediate = manager.create();
		ASSERT_NE(start.index, intermediate.index);
		manager.kill(intermediate);
	}

	Entity last = manager.create();
	ASSERT_EQ(start.index, last.index);
}
