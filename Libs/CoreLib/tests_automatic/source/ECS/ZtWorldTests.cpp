#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"
#include "Zinet/Core/ZtClock.hpp"

namespace zt::core::ecs::tests
{
	using namespace zt::core::tests;

	class ECSWorldTests : public ::testing::Test
	{
	protected:

		World world;
	};

	TEST_F(ECSWorldTests, SpawnEntityIDTest)
	{
		// Entity must have at least one component
		Entity entity_0 = world.spawn(Sprite{});
		ASSERT_EQ(entity_0.getID(), 0);

		Entity entity_1 = world.spawn(Sprite{});
		ASSERT_EQ(entity_1.getID(), 1);
	}

	TEST_F(ECSWorldTests, SpawnOneEntityWithOneComponentTest)
	{
		const Sprite expected{ 56 };
		Entity entity = world.spawn(Sprite(expected));

		ASSERT_EQ(world.getArchetypesCount(), 1);

		const auto actual = world.getComponent<Sprite>(entity);
		ASSERT_TRUE(actual);
		ASSERT_EQ(*actual, expected);
	}

	TEST_F(ECSWorldTests, SpawnOneEntityWithMultipleComponentsTest)
	{
		const Sprite expectedSprite{ 56 };
		const Position expectedPosition{ 2, 4 };
		const Velocity expectedVelocity{ 10, 20 };
		Entity entity = world.spawn(Sprite(expectedSprite), Position(expectedPosition), Velocity(expectedVelocity));

		ASSERT_EQ(world.getArchetypesCount(), 1);

		const auto actualSprite = world.getComponent<Sprite>(entity);
		const auto actualPosition = world.getComponent<Position>(entity);
		const auto actualVelocity = world.getComponent<Velocity>(entity);
		
		ASSERT_TRUE(actualPosition);
		ASSERT_EQ(*actualPosition, expectedPosition);

		ASSERT_TRUE(actualSprite);
		ASSERT_EQ(*actualSprite, expectedSprite);

		ASSERT_TRUE(actualVelocity);
		ASSERT_EQ(*actualVelocity, expectedVelocity);
	}

	TEST_F(ECSWorldTests, SpawnMultipleEntitiesWithMultipleComponentsTest)
	{
		std::vector<Entity> entities;
		const auto count = 5;
		for (size_t i = 0; i < count; i++)
		{
			const auto value = static_cast<float>(i);
			auto entity = world.spawn(Position{ value, value }, Velocity{ value, value }, Sprite{ static_cast<int>(i) });
			entities.push_back(entity);
		}

		for (size_t i = 0; i < count; i++)
		{
			auto& entity = entities[i];

			const auto actualSprite = world.getComponent<Sprite>(entity);
			const auto actualPosition = world.getComponent<Position>(entity);
			const auto actualVelocity = world.getComponent<Velocity>(entity);

			const auto value = static_cast<float>(i);

			ASSERT_TRUE(actualPosition);
			const auto expectedPosition = Position{ value, value };
			ASSERT_EQ(*actualPosition, expectedPosition);

			ASSERT_TRUE(actualVelocity);
			const auto expectedVelocity = Velocity{ value, value };
			ASSERT_EQ(*actualVelocity, expectedVelocity);

			ASSERT_TRUE(actualSprite);
			ASSERT_EQ(*actualSprite, Sprite{ static_cast<int>(i) });
		}
	}

	TEST_F(ECSWorldTests, SpawnMultipleArchetypesTest)
	{
		const auto entityStaticSprite = world.spawn(Position{}, Sprite{});
		const auto entitySprite = world.spawn(Position{}, Velocity{}, Sprite{});

		ASSERT_EQ(world.getArchetypesCount(), 2);

		ASSERT_TRUE(world.getComponent<Position>(entityStaticSprite));
		ASSERT_TRUE(world.getComponent<Sprite>(entityStaticSprite));
		ASSERT_FALSE(world.getComponent<Velocity>(entityStaticSprite));

		ASSERT_TRUE(world.getComponent<Position>(entitySprite));
		ASSERT_TRUE(world.getComponent<Sprite>(entitySprite));
		ASSERT_TRUE(world.getComponent<Velocity>(entitySprite));
	}

	TEST_F(ECSWorldTests, HasEntityTest)
	{
		Entity entity{ {}, {} };
		ASSERT_FALSE(world.hasEntity(entity));

		entity = world.spawn(Position{});
		ASSERT_TRUE(world.hasEntity(entity));

		EXPECT_TRUE(world.remove(entity));
		ASSERT_FALSE(world.hasEntity(entity));
	}

	TEST_F(ECSWorldTests, RemoveTest)
	{
		const auto entity = world.spawn(Position{}, Sprite{});
		ASSERT_TRUE(world.getComponent<Position>(entity));

		ASSERT_TRUE(world.remove(entity));
		ASSERT_FALSE(world.getComponent<Position>(entity));

		ASSERT_FALSE(world.remove(entity));
	}

	TEST_F(ECSWorldTests, TrySpawnEntityWithoutComponentsTest)
	{
		const auto entity = world.spawn();

		ASSERT_EQ(world.getEntitiesCount(), 0);
		ASSERT_EQ(entity.getID(), InvalidID);
		ASSERT_EQ(entity.getComponentsIndex(), InvalidIndex);
		ASSERT_EQ(world.getArchetypesCount(), 0);
	}

	TEST_F(ECSWorldTests, GetComponentsTest)
	{
		world.spawn(Sprite{0});
		world.spawn(Sprite{1}, Position{});
		world.spawn(Sprite{2}, Position{}, Velocity{});

		std::vector<TypeLessVector*> componentsPack = world.getComponentsOfType<Sprite>();
		ASSERT_EQ(componentsPack.size(), 3);

		size_t count = 0;
		for (const auto& segment : componentsPack)
		{
			for (size_t componentIndex = 0; componentIndex < segment->getObjectsCount(); ++componentIndex)
			{
				const auto component = segment->get<Sprite>(componentIndex);
				ASSERT_TRUE(component);
				EXPECT_EQ(component->id, count);
				++count;
			}
		}
	}

	TEST_F(ECSWorldTests, GetEntitiesCountTest)
	{
		world.spawn(Sprite{});
		ASSERT_EQ(world.getEntitiesCount(), 1);

		world.spawn(Sprite{});
		world.spawn(Sprite{});
		ASSERT_EQ(world.getEntitiesCount(), 3);
	}

	TEST_F(ECSWorldTests, GetComponentsCountTest)
	{
		world.spawn(Sprite{});
		world.spawn(Sprite{});
		ASSERT_EQ(world.getComponentCount(), 2);

		world.spawn(Sprite{}, Position{});
		ASSERT_EQ(world.getComponentCount(), 4);
	}

	TEST_F(ECSWorldTests, AddResourceTest)
	{
		auto added = world.addResource(ResourceTime{});
		ASSERT_TRUE(added);

		added = world.addResource(ResourceTime{});
		ASSERT_FALSE(added);
	}

	TEST_F(ECSWorldTests, GetResourceTest)
	{
		auto resource = world.getResource<ResourceTime>();
		ASSERT_FALSE(resource);

		const float expectedTime = 42.f;
		ResourceTime resourceSource = { expectedTime };
		auto added = world.addResource(resourceSource);
		ASSERT_TRUE(added);

		resource = world.getResource<ResourceTime>();
		ASSERT_TRUE(resource);

		ASSERT_EQ(*resource, ResourceTime{ expectedTime });
	}

	TEST_F(ECSWorldTests, HasResourceTest)
	{
		const TypeID typeID = GetTypeID<Position>();
		ASSERT_FALSE(world.hasResource(typeID));

		world.addResource(Position{});
		ASSERT_TRUE(world.hasResource(typeID));
	}

	TEST_F(ECSWorldTests, GetArchetypesWithTest)
	{
		world.spawn(Sprite{});
		world.spawn(Sprite{});
		world.spawn(Sprite{}, Position{});
		world.spawn(Sprite{}, Position{});
		world.spawn(Sprite{}, Position{}, Velocity{});
		world.spawn(Sprite{}, Position{}, Velocity{});
		world.spawn(Sprite{}, Position{}, Velocity{});

		std::vector<Archetype*> archetypes = world.getArchetypesWith<Position, Sprite>();
		ASSERT_EQ(archetypes.size(), 2);

		archetypes = world.getArchetypesWith<Sprite>();
		ASSERT_EQ(archetypes.size(), 3);

		archetypes = world.getArchetypesWith<Velocity>();
		ASSERT_EQ(archetypes.size(), 1);

		archetypes = world.getArchetypesWith<Velocity, Sprite, Position>();
		ASSERT_EQ(archetypes.size(), 1);
	}
}