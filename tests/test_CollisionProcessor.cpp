#include "stdafx.h"
#include "CppUnitTest.h"

#include "testutil.h"

#include <physics/CollisionProcessor.h>

#include <types/vectors.h>

#include <iostream>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace perfectpixel::physics;
using namespace perfectpixel;

namespace tests
{
	TEST_CLASS(test_CollisionProcessor)
	{
	public:
		ecs::EntityManager *m_entityManager;
		CollisionProcessor *m_processor;

	public:
		TEST_METHOD_INITIALIZE(setup)
		{
			m_entityManager = new ecs::EntityManager();
			m_processor = new CollisionProcessor(m_entityManager);
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			delete m_processor;
			delete m_entityManager;
		}

		TEST_METHOD(test_CollisionProcessor_checkCollision_rect_rect)
		{
			ecs::Entity a{ m_entityManager->create() }, b{ m_entityManager->create() };
			
			CollisionProcessor::CollisionData data;
			types::Vector2 *overlap = &data.m_data_RectRectOverlap;

			ColliderComponent::Register(a);
			ColliderComponent::Register(b);

			ColliderComponent::setMaskRectangle(a, types::AARectangle({ 10, 10 }));
			ColliderComponent::setMaskRectangle(a, types::AARectangle({ 20, 10 }));

			Assert::IsTrue(m_processor->checkCollision(a, b, data), L"Collision missed", LINE_INFO());
			AssertFloatApprox(overlap->x(), 15);
			AssertFloatApprox(overlap->y(), 10);

			ecs::TransformComponent::Position(b) = { 0, 10, 0};
			Assert::IsFalse(m_processor->checkCollision(a, b, data), L"Incorrect collison detected", LINE_INFO());

			// Checking partial collision
			ecs::TransformComponent::Position(b) = { 10, 0, 0 };
			Assert::IsTrue(m_processor->checkCollision(a, b, data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->x(), 5);

			// Checking that both sides create negative offset
			ecs::TransformComponent::Position(b) = { -10, 0, 0 };
			Assert::IsTrue(m_processor->checkCollision(a, b, data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->x(), 5);

			// Check partial on both axes
			ecs::TransformComponent::Position(b) = { 14, -9, 0 };
			Assert::IsTrue(m_processor->checkCollision(a, b, data), L"Incorrect collison detected", LINE_INFO());
			AssertFloatApprox(overlap->x(), 1);
			AssertFloatApprox(overlap->y(), 1);
		}
		
		TEST_METHOD(test_CollisionProcessor_singleAxisReposition)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check equal
			m_processor->singleAxisReposition(1, 1, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 2.5f);
			AssertFloatApprox(magnitude2, 2.5f);

			// Check 2 larger
			m_processor->singleAxisReposition(1, 4, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 4);
			AssertFloatApprox(magnitude2, 1);

			// Check 1 larger
			m_processor->singleAxisReposition(8, 2, 5, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 1);
			AssertFloatApprox(magnitude2, 4);
		}
		
		TEST_METHOD(test_CollisionProcessor_singleAxisReposition_zeros)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check 1 zero
			m_processor->singleAxisReposition(0, 2, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 10);
			AssertFloatApprox(magnitude2, 0);

			// Check 2 zero
			m_processor->singleAxisReposition(.5f, 0, 15, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 15);

			// Check both zero, should not move
			m_processor->singleAxisReposition(0, 0, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 0);
		}

		TEST_METHOD(test_CollisionProcessor_singleAxisRepositionty_infinity)
		{
			types::PpFloat magnitude1, magnitude2;

			// Check 1 inf
			m_processor->singleAxisReposition(types::Infinity, 2, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 10);

			// Check 2 inf
			m_processor->singleAxisReposition(3, types::Infinity, 7.5f, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 7.5f);
			AssertFloatApprox(magnitude2, 0);

			// Check both inf, should not move
			m_processor->singleAxisReposition(types::Infinity, types::Infinity, 10, &magnitude1, &magnitude2);
			AssertFloatApprox(magnitude1, 0);
			AssertFloatApprox(magnitude2, 0);
		}
	};
}
