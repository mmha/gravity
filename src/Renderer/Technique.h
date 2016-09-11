#ifndef GE_TECHNIQUE_H
#define GE_TECHNIQUE_H

#include <memory>
#include "internal/ProgramPipeline.h"
#include "internal/Buffer.h"

namespace ge
{
	namespace gl4
	{
		//TODO void specialization in case no parameters are required
		
		template<typename STD140POD, typename = typename std::enable_if_t<std::is_pod<STD140POD>::value>> //TODO implement a concept for that
		class Technique: public STD140POD
		{
			public:
				template<typename... Args>
				Technique(std::shared_ptr<ProgramPipeline> pipeline, Args... &&args)
				: STD140POD(std::forward<Args>(args)...)
				, pipeline_(pipeline)
				, gpuParameterData_(sizeof(STD140POD))
				{

				}

				void use()
				{
					gpuParameterData_.upload(static_cast<STD140POD*>(this)); //meh...
					gpuParameterData_.bindAs(GL_UNIFORM_BUFFER, (uint32_t) UniformLocations::GE_ULOCATION_TECHNIQUE_PARAMETER);
					pipeline_->use();
				}

			private:
			std::shared_ptr<ProgramPipeline> pipeline_;
			Buffer gpuParameterData_;
		};
	}
}

#endif