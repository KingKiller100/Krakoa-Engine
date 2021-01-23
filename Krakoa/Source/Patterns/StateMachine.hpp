#pragma once

#include <functional>

namespace patterns
{
	template<typename State>
	class StateMachine
	{
	public:
		using StateChangeFunc_t = std::function<void(const State&, const State&)>;
		using State_t = State;
		
	public:
		explicit constexpr StateMachine(State&& initalState, const StateChangeFunc_t& func = nullptr)
			: currentState(std::forward<State>(initalState))
			, changeStateFunc(func)
		{}

		constexpr const State_t& GetState() const
		{
			return currentState;
		}

		constexpr void ChangeState(const State_t& nextState)
		{
			if (nextState == currentState)
				return;
			
			if (changeStateFunc != nullptr)
				changeStateFunc(currentState, nextState);
			currentState = nextState;
		}
		
		constexpr bool CompareState(const State_t& otherState)
		{
			return currentState == otherState;
		}
		
		constexpr bool BitMaskState(const State_t& otherState)
		{
			return currentState | otherState;
		}

		constexpr void SetChangeStateFunc(const StateChangeFunc_t& func)
		{
			changeStateFunc = func;
		}
		
		constexpr bool operator==(const StateMachine& other)
		{
			return currentState == other.currentState && changeStateFunc == other.changeStateFunc;
		}

		constexpr bool operator!=(const StateMachine& other)
		{
			return !(*this == other);
		}

		template<typename U>
		constexpr bool operator==(const U& other)
		{
			return currentState == other;
		}

		template<typename U>
		constexpr bool operator!=(const U& other)
		{
			return !(*this == other);
		}

		template<typename U>
		constexpr bool operator<(const U& other)
		{
			return currentState < other;
		}

		template<typename U>
		constexpr bool operator<=(const U& other)
		{
			return currentState <= other;
		}

	private:
		State currentState;
		StateChangeFunc_t changeStateFunc;
	};
}
