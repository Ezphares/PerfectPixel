#include "stdafx.h"
#include "CppUnitTest.h"

#include <EntityComponentSystem/EntityManager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::ecs;

namespace tests
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(test_entitymanager_create_kill_alive)
		{
			EntityManager manager;

			Entity e1 = manager.create();
			Assert::IsTrue(manager.isAlive(e1), L"Newly created entity is not alive", LINE_INFO());
			
			Entity e2 = manager.create();
			Assert::IsTrue(manager.isAlive(e2), L"Second created entity is not alive", LINE_INFO());
			
			manager.kill(e1);
			Assert::IsFalse(manager.isAlive(e1), L"Killed entity is still alive", LINE_INFO());
			Assert::IsTrue(manager.isAlive(e2), L"Unintended entity was killed", LINE_INFO());
			
			manager.kill(e2);
			Assert::IsFalse(manager.isAlive(e2), L"Second killed entity is still alive", LINE_INFO());

		}

		TEST_METHOD(test_entitymanager_reuse_default)
		{
			EntityManager manager;

			Entity e1 = manager.create();
			manager.kill(e1);
			Entity e2 = manager.create();
			Assert::AreNotEqual(entityIndex(e1), entityIndex(e2), L"Manager instantly reuses indices by default", LINE_INFO());
		}

		TEST_METHOD(test_entitymanager_reuse_instant)
		{
			EntityManager manager(0);

			Entity e_instant1 = manager.create();
			manager.kill(e_instant1);
			Entity e_instant2 = manager.create();

			Assert::AreEqual(entityIndex(e_instant1), entityIndex(e_instant2), L"Manager failed to reuse indices", LINE_INFO());
			Assert::AreEqual(entityGeneration(e_instant1) + 1, entityGeneration(e_instant2), L"Manager failed to increment generation", LINE_INFO());
		}

		TEST_METHOD(test_entitymanager_reuse_settings)
		{
			EntityManager manager(10);

			Entity start = manager.create();
			manager.kill(start);

			// Cycle 10 indices before reusing
			for (int i = 0; i < 10; ++i)
			{
				Entity intermediate = manager.create();
				Assert::AreNotEqual(entityIndex(start), entityIndex(intermediate), L"Manager reuses indices too soon", LINE_INFO());
				manager.kill(intermediate);
			}

			Entity last = manager.create();
			Assert::AreEqual(entityIndex(start), entityIndex(last), L"Manager reuses indices too late", LINE_INFO());
		}

	};
}