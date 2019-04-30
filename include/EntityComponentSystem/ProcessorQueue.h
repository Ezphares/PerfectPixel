#pragma once

#include <EntityComponentSystem/Processor.h>

#include <types/numbers.h>

#include <vector>
#include <map>

namespace perfectpixel {
	namespace ecs {

	class ProcessorQueue
	{
	public:
		static const types::PpInt USER_PROCESSOR_DEFAULT_PRIORITY;

	public:
		ProcessorQueue();
		virtual ~ProcessorQueue();

		void processAll(types::PpFloat deltaT);
		void renderAll(types::PpFloat deltaT);

		void registerProcessor(Processor *processor, types::PpInt priority, bool managed);

	private:
		std::map<types::PpInt, std::vector<Processor*>> m_processors;
		std::vector<types::PpInt> m_priorities;
		std::vector<Processor *> m_managedProcessors;
	};

} }
