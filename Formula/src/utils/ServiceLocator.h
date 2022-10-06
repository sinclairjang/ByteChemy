#pragma once

class Renderer;

class ServiceLocator
{
public:
	static Renderer* getRenderer();

	static void Provide(Renderer* renderer);

private:
	static Renderer* m_Renderer;
};

