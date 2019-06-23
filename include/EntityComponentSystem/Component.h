#pragma once

#include <EntityComponentSystem/Field.h>
#include <EntityComponentSystem/IComponentStorage.h>

#include <types/Singleton.h>
#include <types/Bitset.h>

namespace perfectpixel { namespace ecs {

	template <typename T>
	class Component : public types::Singleton<T>
	{
	private:
		static uint32_t objects;
		static uint32_t lastIndex;
		static std::vector<IField*> fields;

	protected:
		inline static Field<Component<T>, Entity> Owner;

		virtual void purge(uint32_t idx) {};
		virtual void initialize(uint32_t idx)
		{
			for (auto field : fields)
			{
				field->reset(idx);
			}
		};

	public:
		const static size_t Id;

		static Entity at(uint32_t idx)
		{
			return Owner.at(idx);
		}

		static void AddField(IField *field)
		{
			fields.push_back(field);
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
			uint32_t idx = instance->_register(entity, lastIndex);

			if (idx >= lastIndex)
			{
				lastIndex++;
			}

			instance->initialize(idx);

			objects++;
		}

		static void Delete(Entity entity)
		{
			uint32_t idx = getInstance()->_delete(entity);

			getInstance()->purge(idx);

			objects--;
		}

		static void Filter(types::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType)
		{
			getInstance()->_filter(mask, filterType);
		}

		static void SetToDefault(Entity entity)
		{
			T *instance = getInstance();
			instance->initialize(instance->_index(entity));
		}
	};

	template<typename T> uint32_t Component<T>::objects = 0;
	template<typename T> uint32_t Component<T>::lastIndex = 0;
	template<typename T> const size_t Component<T>::Id = std::hash(typeid(T).name());
	template<typename T> std::vector<IField *> Component<T>::fields = std::vector<IField *>();

	template <typename T>
	class HintComponent : public Component<T>, public IComponentStorage
	{
	public:
		virtual bool _has(Entity entity) const
		{
			uint32_t idx = entityIndex(entity);
			return m_mask.size() > idx && m_mask[idx];
		}

		virtual uint32_t _index(Entity entity) const 
		{
			return entityIndex(entity);
		}

		virtual uint32_t _register(Entity entity, uint32_t currentSize)
		{
			uint32_t idx = entityIndex(entity);
			if (m_mask.size() <= idx)
			{
				m_mask.resize(idx + 1);
			}
			m_mask[idx] = true;
			return idx;
		}
		
		virtual uint32_t _delete(Entity entity)
		{
			m_mask[entityIndex(entity)] = false;
			return 0;
		}

		virtual void _filter(types::BitSet &mask, ComponentStorageFilterType filterType) const
		{
			mask &= (filterType == IComponentStorage::WITH) ? m_mask : ~m_mask;
		}

	private:
		types::BitSet m_mask;
	};

} }
