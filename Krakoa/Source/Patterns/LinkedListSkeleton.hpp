#pragma once

#include <type_traits>

namespace patterns
{
	template<typename T>
	struct Node
	{
		T data;
		Node* pNext;
	};

	template<typename T>
	struct DoubleNode
	{
		T data;
		DoubleNode* pNext;
		DoubleNode* pPrev;
	};

	template<typename T, typename NodeType, class = std::enable_if_t<
		std::is_same_v<NodeType, Node<T>>
		|| std::is_same_v<NodeType, DoubleNode<T>>>>
	struct LinkedListSkeleton
	{
		using Node_Type = NodeType;
		
		Node_Type* pHead;
		Node_Type** ppCurrent;
	};
}
