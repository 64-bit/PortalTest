#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "glm.hpp"

#include "BoundingBox.h"

namespace BVH2
{
	struct BuildBVHArgs
	{
		int MaxLeafSize;
	};

	template <typename T>
	struct BVHNode
	{
	public:

		BoundingBox Bounds;

		union 
		{
			int RightNode;
			int ElementStartIndex;
		};

		int ElementCount;

		static void ConstructNewBVH(std::vector<T>& elements, std::vector<BVHNode<T>>& nodeContainer, BuildBVHArgs bvhArgs);

		inline bool HasChildren() const;


	};

	template <typename T>
	struct BVHConstructionNode
	{
	public:
		//TODO:Extra construction values here 

		BoundingBox Bounds;

		union
		{
			int RightNode;
			int ElementStartIndex;
		};

		int ElementCount;
		

	};

	template<typename T>
	bool CompareNodesByX(const T& a, const T& b) 
	{
		return a.GetCenter().x < b.GetCenter().x;
	}

	template<typename T>
	bool CompareNodesByY(const T& a, const T& b)
	{
		return a.GetCenter().y < b.GetCenter().y;
	}

	template<typename T>
	bool CompareNodesByZ(const T& a, const T& b)
	{
		return a.GetCenter().z < b.GetCenter().z;
	}

	template<typename T>
	inline void ConstructBVH(std::vector<T>& elements, std::vector<BVHNode<T>>& nodeContainer, int currentNodeIndex, BuildBVHArgs bvhArgs)
	{
		//Do the following recursively
		BVHNode<T>& currentNode = nodeContainer[currentNodeIndex];

		//Get bounds of this node.
		currentNode.Bounds = BoundingBox();
		int end = currentNode.ElementStartIndex + currentNode.ElementCount;
		for (int i = currentNode.ElementStartIndex; i < end; i++)
		{
			currentNode.Bounds.EnlargeByBounds(elements[i].GetBounds());
		}

		//Check and see if this node needs to be split, or is it a leaf
		if (currentNode.ElementCount <= bvhArgs.MaxLeafSize)
		{
			return;
		}

		//TODO:
		//Attemmpt all 3 axis splits and select the best one

		//For now, just split on the largest one
		//order data by best split axis

		auto startItr = elements.begin() + currentNode.ElementStartIndex;
		auto endItr = startItr + currentNode.ElementCount;

		int splitAxis;
		glm::vec3 size = currentNode.Bounds.GetSize();
		if (size.x > size.y && size.x > size.z)
		{
			std::sort(startItr, endItr, CompareNodesByX<T>);
			splitAxis = 0;
		}
		else if (size.y > size.z)
		{
			std::sort(startItr, endItr, CompareNodesByY<T>);
			splitAxis = 1;
		}
		else
		{
			std::sort(startItr, endItr, CompareNodesByZ<T>);
			splitAxis = 2;
		}

		//Select an index to split upon
		int splitCount = currentNode.ElementCount / 2;

		int leftStart = currentNode.ElementStartIndex;
		int leftCount = splitCount;

		int rightStart = leftStart + leftCount;
		int rightCount = currentNode.ElementCount - splitCount;

		//Recurse down each side generating nodes

		//Left side
		nodeContainer.push_back(BVHNode<T>());
		BVHNode<T>& leftNode = nodeContainer.back();
		leftNode.ElementStartIndex = leftStart;
		leftNode.ElementCount = leftCount;
		int leftIndex = nodeContainer.size() - 1;

		ConstructBVH(elements, nodeContainer, leftIndex, bvhArgs);

		//Right side
		nodeContainer.push_back(BVHNode<T>());
		BVHNode<T>& rightNode = nodeContainer.back();
		rightNode.ElementStartIndex = rightStart;
		rightNode.ElementCount = rightCount;
		int rightNodeIndex = nodeContainer.size() - 1;

		ConstructBVH(elements, nodeContainer, rightNodeIndex, bvhArgs);

		//Write the refernce to the right child to the owning node that called this here
		BVHNode<T>& currentNodeToModify = nodeContainer[currentNodeIndex];

		currentNodeToModify.RightNode = rightNodeIndex;
		currentNodeToModify.ElementCount = 0;

	}


	template<typename T>
	inline void BVHNode<T>::ConstructNewBVH(std::vector<T>& elements, std::vector<BVHNode<T>>& nodeContainer, BuildBVHArgs bvhArgs)
	{
		//Construct initial root node & construction vector
		//std::vector<BVHNode<T>> constructionVector;


		nodeContainer.push_back(BVHNode<T>());
		BVHNode<T>& rootNode = nodeContainer.back();
		rootNode.ElementStartIndex = 0;
		rootNode.ElementCount = elements.size();

		//Build BVH recursively
		ConstructBVH(elements, nodeContainer, 0, bvhArgs);


		//write root node to final place in construction array'
			//Or don't because we can use references

		//nodeContainer.reserve(constructionVector.size());
		////convert construction array to final
		//for (auto& node : constructionVector)
		//{
		//	nodeContainer.push_back(node);
		//}
	}
	template<typename T>
	inline bool BVHNode<T>::HasChildren() const
	{
		return ElementCount > 0;
	}
}

