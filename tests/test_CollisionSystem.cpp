#include "testutil.h"

#include <physics/CollisionSystem.h>

#include <Bedrock/vectors.h>

#include <cmath>

using namespace perfectpixel::physics;
using namespace perfectpixel;

GTEST_TEST(test_CollisionSystem, RectRectCollision)
{
	ecs::Entity 
		a{ ecs::EntityManager::getInstance()->create() }, 
		b{ ecs::EntityManager::getInstance()->create() };
	std::vector<ecs::Entity>entities;
	entities.push_back(a);
	entities.push_back(b);

	ecs::TransformComponent::Register(a);
	ecs::TransformComponent::Register(b);
	ColliderComponent::Register(a);
	ColliderComponent::Register(b);

	ColliderComponent::SetMaskRectangle(a, bedrock::AARectangle({ 10, 10 }));
	ColliderComponent::SetMaskRectangle(b, bedrock::AARectangle({ 20, 10 }));

	CollisionSystem::onUpdate(entities.begin(), entities.end(), 1.0f);
        ASSERT_EQ(1u, CollisionSystem::getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 15);
	//AssertFloatApprox(overlap->y(), 10);

	ecs::TransformComponent::Position(b) = { 0, 10, 0 };
	CollisionSystem::onUpdate(entities.begin(), entities.end(), 1.0f);
	ASSERT_EQ(0u, CollisionSystem::getCollisionsLastUpdate());

	// Checking partial collision
	ecs::TransformComponent::Position(b) = { 10, 0, 0 };
	CollisionSystem::onUpdate(entities.begin(), entities.end(), 1.0f);
	ASSERT_EQ(1u, CollisionSystem::getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 5);

	// Checking that both sides create negative offset
	ecs::TransformComponent::Position(b) = { -10, 0, 0 };
	CollisionSystem::onUpdate(entities.begin(), entities.end(), 1.0f);
	ASSERT_EQ(1u, CollisionSystem::getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 5);

	// Check partial on both axes
	ecs::TransformComponent::Position(b) = { 14, -9, 0 };
	CollisionSystem::onUpdate(entities.begin(), entities.end(), 1.0f);
	ASSERT_EQ(1u, CollisionSystem::getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 1);
	//AssertFloatApprox(overlap->y(), 1);
}
		
GTEST_TEST(test_CollisionSystem, SingleAxisReposition)
{
	float magnitude1, magnitude2;

	// Check equal
	CollisionSystem::singleAxisReposition(1, 1, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 2.5f);
	ASSERT_FLOAT_EQ(magnitude2, 2.5f);

	// Check 2 larger
	CollisionSystem::singleAxisReposition(1, 4, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 4);
	ASSERT_FLOAT_EQ(magnitude2, 1);

	// Check 1 larger
	CollisionSystem::singleAxisReposition(8, 2, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 1);
	ASSERT_FLOAT_EQ(magnitude2, 4);
}
		
GTEST_TEST(test_CollisionSystem, SingleAxisReposition_Zeros)
{
	float magnitude1, magnitude2;

	// Check 1 zero
	CollisionSystem::singleAxisReposition(0, 2, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 10);
	ASSERT_FLOAT_EQ(magnitude2, 0);

	// Check 2 zero
	CollisionSystem::singleAxisReposition(.5f, 0, 15, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 15);

	// Check both zero, should not move
	CollisionSystem::singleAxisReposition(0, 0, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 0);
}

GTEST_TEST(test_CollisionSystem, SingleAxisReposition_Infinity)
{
	float magnitude1, magnitude2;

	// Check 1 inf
	CollisionSystem::singleAxisReposition(bedrock::Infinity, 2, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 10);

	// Check 2 inf
	CollisionSystem::singleAxisReposition(3, bedrock::Infinity, 7.5f, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 7.5f);
	ASSERT_FLOAT_EQ(magnitude2, 0);

	// Check both inf, should not move
	CollisionSystem::singleAxisReposition(bedrock::Infinity, bedrock::Infinity, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 0);
}
