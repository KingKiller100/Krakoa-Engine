#pragma once

#include "../Core/EngineCore.h"

#include "../Patterns/SimpleSingleton.h"

#include <memory>
#include <unordered_map>

namespace kTest
{
	class Tester;
	
	using TestMap = std::unordered_map<const char*, std::unique_ptr<Tester>>;
	
	class KRAKOA_API TesterManager : public Pattern::SimpleSingleton<TesterManager>
	{
	public:
		TesterManager(Token);
		~TesterManager();
		
		void Initialize();
		void RunAll();

	protected:
		void Add(Tester* test);
		
	private:
		TestMap tests;
	};
}