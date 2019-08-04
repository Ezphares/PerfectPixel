#include "testutil.h"

#include <physics/CollisionProcessor.h>

#include <Bedrock/vectors.h>

#include <cmath>

using namespace perfectpixel::physics;
using namespace perfectpixel;

class test_CollisionProcessor : public ::testing::Test
{
protected:

	void SetUp() override
	{
		m_processor = new CollisionProcessor();
	}

	void TearDown() override
	{
		delete m_processor;
	}

	CollisionProcessor *m_processor;
};

TEST_F(test_CollisionProcessor, test_CollisionProcessor_checkCollision_rect_rect)
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

	m_processor->onUpdate(entities, 1.0f);
	ASSERT_EQ(1u, m_processor->getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 15);
	//AssertFloatApprox(overlap->y(), 10);

	ecs::TransformComponent::Position(b) = { 0, 10, 0 };
	m_processor->onUpdate(entities, 1.0f);
	ASSERT_EQ(0u, m_processor->getCollisionsLastUpdate());

	// Checking partial collision
	ecs::TransformComponent::Position(b) = { 10, 0, 0 };
	m_processor->onUpdate(entities, 1.0f);
	ASSERT_EQ(1u, m_processor->getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 5);

	// Checking that both sides create negative offset
	ecs::TransformComponent::Position(b) = { -10, 0, 0 };
	m_processor->onUpdate(entities, 1.0f);
	ASSERT_EQ(1u, m_processor->getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 5);

	// Check partial on both axes
	ecs::TransformComponent::Position(b) = { 14, -9, 0 };
	m_processor->onUpdate(entities, 1.0f);
	ASSERT_EQ(1u, m_processor->getCollisionsLastUpdate());
	//AssertFloatApprox(overlap->x(), 1);
	//AssertFloatApprox(overlap->y(), 1);
}
		
TEST_F(test_CollisionProcessor, test_CollisionProcessor_singleAxisReposition)
{
	float magnitude1, magnitude2;

	// Check equal
	m_processor->singleAxisReposition(1, 1, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 2.5f);
	ASSERT_FLOAT_EQ(magnitude2, 2.5f);

	// Check 2 larger
	m_processor->singleAxisReposition(1, 4, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 4);
	ASSERT_FLOAT_EQ(magnitude2, 1);

	// Check 1 larger
	m_processor->singleAxisReposition(8, 2, 5, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 1);
	ASSERT_FLOAT_EQ(magnitude2, 4);
}
		
TEST_F(test_CollisionProcessor, test_CollisionProcessor_singleAxisReposition_zeros)
{
	float magnitude1, magnitude2;

	// Check 1 zero
	m_processor->singleAxisReposition(0, 2, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 10);
	ASSERT_FLOAT_EQ(magnitude2, 0);

	// Check 2 zero
	m_processor->singleAxisReposition(.5f, 0, 15, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 15);

	// Check both zero, should not move
	m_processor->singleAxisReposition(0, 0, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 0);
}

TEST_F(test_CollisionProcessor, test_CollisionProcessor_singleAxisRepositionty_infinity)
{
	float magnitude1, magnitude2;

	// Check 1 inf
	m_processor->singleAxisReposition(bedrock::Infinity, 2, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 10);

	// Check 2 inf
	m_processor->singleAxisReposition(3, bedrock::Infinity, 7.5f, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 7.5f);
	ASSERT_FLOAT_EQ(magnitude2, 0);

	// Check both inf, should not move
	m_processor->singleAxisReposition(bedrock::Infinity, bedrock::Infinity, 10, &magnitude1, &magnitude2);
	ASSERT_FLOAT_EQ(magnitude1, 0);
	ASSERT_FLOAT_EQ(magnitude2, 0);
}
