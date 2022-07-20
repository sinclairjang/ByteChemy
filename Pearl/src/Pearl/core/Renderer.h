#pragma once

namespace Pearl
{
	class MasterConfigFile;

	class Renderer
	{
	public:
		virtual ~Renderer();

		virtual void StartUp();
		virtual void ShutDown();
		virtual void SetRenderState(MasterConfigFile cfg);
		virtual void Reset();
	};
}