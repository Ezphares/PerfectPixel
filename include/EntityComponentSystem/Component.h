#pragma once

#include <EntityComponentSystem/Field.h>
#include <EntityComponentSystem/IComponentStorage.h>
#include <EntityComponentSystem/EntityManager.h>

#include <Bedrock/Singleton.h>
#include <Bedrock/Bitset.h>

#include <map>

namespace perfectpixel { namespace ecs {

	template <typename T>
	class Component : public bedrock::Singleton<T>
	{
	protected:
		Component()
		{
#if defined(PP_CLEANUP_CALLBACKS)
			EntityManager::getInstance()->addKillCallback(&SafeDelete);
#endif
		}

	private:
		uint32_t objects;
		uint32_t lastIndex;
		std::map<int32_t, IField*> fields;

	protected:
		static Field<Component<T>, Entity> Owner;

		virtual void purge(uint32_t idx) {};
		virtual void initialize(uint32_t idx)
		{
			for (auto field : fields)
			{
				field.second->reset(idx);
			}
		};

	public:
		const static size_t Id;

		static Entity at(uint32_t idx)
		{
			return Owner.at(idx);
		}

		static bool AddField(int32_t id, IField *field)
		{
			auto &fields = getInstance()->fields;
			// Inline statics are weird, we have to check for duplicates
			bool add = fields.find(id) == fields.end();
			if (add)
			{
				fields[id] = field;
			}

			return add;
		}

		static IField *Lookup(int32_t id)
		{
			return getInstance()->fields[id];
		}

		static bool Has(Entity entity)
		{
			return getInstance()->_has(entity);
		}

		static uint32_t Index(Entity entity)
		{
			return getInstance()->_index(entity);
		}

		static void Register(Entity entity)
		{
			T *instance = getInstance();
			uint32_t idx = instance->_register(entity, instance->lastIndex);

			if (idx >= instance->lastIndex)
			{
				instance->lastIndex++;
			}

			instance->initialize(idx);
			instance->objects++;
		}

		static void Delete(Entity entity)
		{
			uint32_t idx = getInstance()->_delete(entity);

			getInstance()->purge(idx);

			getInstance()->objects--;
		}

		static void SafeDelete(Entity entity)
		{
			if (getInstance()->_has(entity)) Delete(entity);
		}

		static void Filter(bedrock::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType)
		{
			getInstance()->_filter(mask, filterType);
		}

		static void SetToDefault(Entity entity)
		{
			T *instance = getInstance();
			instance->initialize(instance->_index(entity));
		}

		static void Clean()
		{
			getInstance()->_clean();
		}
	};

	template<typename T> Field<Component<T>, Entity> Component<T>::Owner = {FieldTable::NoReflection};

	template <typename T>
	class HintComponent : public Component<T>, public IComponentStorage
	{
	public:
		virtual bool _has(Entity entity) const
		{
			uint32_t idx = entity.index;
			return m_mask.size() > idx && m_mask[idx];
		}

		virtual uint32_t _index(Entity entity) const 
		{
			return entity.index;
		}

		virtual uint32_t _register(Entity entity, uint32_t currentSize)
		{
			uint32_t idx = entity.index;
			if (m_mask.size() <= idx)
			{
				m_mask.resize(idx + 1);
			}
			m_mask[idx] = true;
			return idx;
		}
		
		virtual uint32_t _delete(Entity entity)
		{
			m_mask[entity.index] = false;
			return 0;
		}

		virtual uint32_t _safeDelete(Entity entity)
		{
			return _delete(entity);
		}

		virtual void _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
		{
			mask &= (filterType == IComponentStorage::WITH) ? m_mask : ~m_mask;
		}

		virtual void _clean()
		{}

	private:
		bedrock::BitSet m_mask;
	};

} }
