#include "stdafx.h"
#include "CppUnitTest.h"

#include <EntityComponentSystem/Entity.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::world;

namespace tests
{
	TEST_CLASS(test_Entity)
	{
	public:

		TEST_METHOD(test_entity_create_get)
		{
			Entity entity_0_0 = entityCreate(0, 0);
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityGeneration(entity_0_0), L"Entity generation did not match expected value", LINE_INFO());
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityIndex(entity_0_0), L"Entity index did not match expected value", LINE_INFO());

			Entity entity_100_0 = entityCreate(100, 0);
			Assert::AreEqual(static_cast<std::uint32_t>(100), entityGeneration(entity_100_0), L"Entity generation did not match expected value", LINE_INFO());
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityIndex(entity_100_0), L"Entity index did not match expected value", LINE_INFO());

			Entity entity_0_25 = entityCreate(0, 25);
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityGeneration(entity_0_25), L"Entity generation did not match expected value", LINE_INFO());
			Assert::AreEqual(static_cast<std::uint32_t>(25), entityIndex(entity_0_25), L"Entity index did not match expected value", LINE_INFO());
		}

		TEST_METHOD(test_entity_overflow)
		{
			uint32_t generationMax = (1 << GENERATION_BITS) - 1;
			Entity entity_1_0 = entityCreate(generationMax + 2, 0);
			Assert::AreEqual(static_cast<std::uint32_t>(1), entityGeneration(entity_1_0), L"Entity generation did overflow correctly", LINE_INFO());
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityIndex(entity_1_0), L"Generation overflow affected index", LINE_INFO());

			uint32_t indexMax = (1 << INDEX_BITS) - 1;
			Entity entity_0_1 = entityCreate(0, indexMax + 2);
			Assert::AreEqual(static_cast<std::uint32_t>(0), entityGeneration(entity_0_1), L"Index overflow affected generation", LINE_INFO());
			Assert::AreEqual(static_cast<std::uint32_t>(1), entityIndex(entity_0_1), L"Index did overflow correctly", LINE_INFO());

		}

	};
}
