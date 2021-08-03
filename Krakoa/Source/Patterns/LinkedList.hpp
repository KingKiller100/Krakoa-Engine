#pragma once

#include <type_traits>

namespace patterns
{
	template<typename T>
	struct UnidirectionalNode
	{
		using Type = T;

		T data;
		UnidirectionalNode* next = nullptr;
	};

	template<typename T>
	struct BidirectionalNode
	{
		using Type = T;
		
		T data;
		BidirectionalNode* next = nullptr;
		BidirectionalNode* prev = nullptr;
	};

	template<typename NodeType, typename T = typename NodeType::Type>
	struct BasicLinkedList
	{
		using Node_t = NodeType;
		
		Node_t* head = nullptr;
		Node_t* tail = nullptr;
	};

	template<typename T>
	using UniDirectionalLinkedList = BasicLinkedList<UnidirectionalNode<T>>;

	template<typename T>
	using BiDirectionalLinkedList = BasicLinkedList<BidirectionalNode<T>>;
}
