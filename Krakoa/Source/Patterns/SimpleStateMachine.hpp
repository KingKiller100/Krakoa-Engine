#pragma once

#include <functional>

namespace patterns
{
	template<typename State>
	class SimpleStateMachine
	{
	public:
		using StateChangeFunc_t = std::function<void(const State&, const State&)>;
		using State_t = State;

	public:
		explicit constexpr SimpleStateMachine(State&& initalState, const StateChangeFunc_t& func = nullptr)
			: currentState(std::forward<State>(initalState))
			, changeStateFunc(func)
		{}

		constexpr const State_t& GetState() const
		{
			return currentState;
		}

		constexpr bool CheckState(const State_t& otherState)
		{
			return currentState == otherState;
		}

		constexpr bool BitMaskState(const State_t& otherState)
		{
			return currentState | otherState;
		}

		constexpr bool operator==(const SimpleStateMachine& other)
		{
			return currentState == other.currentState && changeStateFunc == other.changeStateFunc;
		}

		constexpr bool operator!=(const SimpleStateMachine& other)
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

	protected:
		constexpr bool TransitionState(const State_t& from, const State_t& to)
		{
			const auto correctPreviousState = CheckState(from);
			SetState(to);
			return correctPreviousState;
		}
		
		constexpr void SetState(const State_t& nextState)
		{
			if (nextState == currentState)
				return;

			if (changeStateFunc != nullptr)
				changeStateFunc(currentState, nextState);

			currentState = nextState;
		}

		constexpr void SetChangeStateFunc(const StateChangeFunc_t& func)
		{
			changeStateFunc = func;
		}

	private:
		State currentState;
		StateChangeFunc_t changeStateFunc;
	};
}
