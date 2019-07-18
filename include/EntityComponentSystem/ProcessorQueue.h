#pragma once

#include <EntityComponentSystem/Processor.h>

#include <Bedrock/numbers.h>

#include <vector>
#include <map>

namespace perfectpixel {
	namespace ecs {

	class ProcessorQueue
	{
	public:
		static const bedrock::PpInt USER_PROCESSOR_DEFAULT_PRIORITY;

	public:
		ProcessorQueue();
		virtual ~ProcessorQueue();

		void processAll(bedrock::PpFloat deltaT);
		void renderAll(bedrock::PpFloat deltaT);

		void registerProcessor(Processor *processor, bedrock::PpInt priority, bool managed);

	private:
		std::map<bedrock::PpInt, std::vector<Processor*>> m_processors;
		std::vector<bedrock::PpInt> m_priorities;
		std::vector<Processor *> m_managedProcessors;
	};

} }
