#include "stdafx.h"
#include "CppUnitTest.h"

#include <physics/PhysicsManager.h>

#include <types/vectors.h>

#include <iostream>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::physics;
using namespace perfectpixel;

namespace tests
{
	TEST_CLASS(test_PhysicsManager)
	{
	public:
		world::EntityManager *m_entityManager;
		PhysicsManager *m_manager;

	public:
		TEST_METHOD_INITIALIZE(setup)
		{
			m_entityManager = new world::EntityManager();
			m_manager = new PhysicsManager(m_entityManager);
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			delete m_manager;
			delete m_entityManager;
		}



		void AssertFloatApprox(types::PpFloat expected, types::PpFloat actual, const wchar_t *message = 0, const Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo *lineinfo = 0)
		{
			Assert::IsTrue(std::abs(expected - actual) < 0.01, message, lineinfo);
		}

		TEST_METHOD(test_PhysicsManager_checkCollision_rect_rect)
		{
			world::Entity a{ m_entityManager->create() }, b{ m_entityManager->create() };
			
			PhysicsManager::CollisionData data;
			types::Vector2 *overlap = &data.m_data_RectRectOverlap;

			ColliderComponent ca = ColliderComponent(a, types::Rectangle({ 10, 10 }));
			ColliderComponent cb = ColliderComponent(b, types::Rectangle({ 20, 10 }));

			Assert::IsTrue(m_manager->checkCollision(ca, cb, &data), L"Collision missed", LINE_INFO());
			AssertFloatApprox(overlap->m_x, 15);
			AssertFloatApprox(overlap->m_y, 10);

			m_manager->getTransform(b).m_position = { 0, 10, 0};
			Assert::IsFalse(m_manager->checkCollision(ca, cb, &data), L"Incorrect collison detected", LINE_INFO());

			// Checking partial collision
			m_manager->getTransform(b).m_position = { 10, 0, 0 };
			Assert::IsTrue(m_manager->checkCollision(ca, cb, &data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->m_x, 5);

			// Checking that both sides create negative offset
			m_manager->getTransform(b).m_position = { -10, 0, 0 };
			Assert::IsTrue(m_manager->checkCollision(ca, cb, &data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->m_x, 5);

			// Check partial on both axes
			m_manager->getTransform(b).m_position = { 14, -9, 0 };
			Assert::IsTrue(m_manager->checkCollision(ca, cb, &data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->m_x, 1);
			AssertFloatApprox(overlap->m_y, 1);
		}

		TEST_METHOD(test_PhysicsManager_singleAxisReposition)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check equal
			m_manager->singleAxisReposition(1, 1, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 2.5f);
			AssertFloatApprox(magnitude2, 2.5f);

			// Check 2 larger
			m_manager->singleAxisReposition(1, 4, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 4);
			AssertFloatApprox(magnitude2, 1);

			// Check 1 larger
			m_manager->singleAxisReposition(8, 2, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 1);
			AssertFloatApprox(magnitude2, 4);
		}

		TEST_METHOD(test_PhysicsManager_singleAxisReposition_zeros)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check 1 zero
			m_manager->singleAxisReposition(0, 2, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 10);
			AssertFloatApprox(magnitude2, 0);

			// Check 2 zero
			m_manager->singleAxisReposition(.5f, 0, 15, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 15);

			// Check both zero, should not move
			m_manager->singleAxisReposition(0, 0, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 0);
		}

		TEST_METHOD(test_PhysicsManager_singleAxisRepositionty_infinity)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check 1 inf
			m_manager->singleAxisReposition(types::Infinity, 2, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 10);

			// Check 2 inf
			m_manager->singleAxisReposition(3, types::Infinity, 7.5f, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 7.5f);
			AssertFloatApprox(magnitude2, 0);

			// Check both inf, should not move
			m_manager->singleAxisReposition(types::Infinity, types::Infinity, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 0);
		}
	};
}
