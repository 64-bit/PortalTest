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

	struct BestSplit
	{
		int Axis;
		float Poinnt;
	};

	struct SplitBucket
	{
		int ElementCount = 0;
		BoundingBox Bounds = BoundingBox();

		void MergeWith(const SplitBucket& other)
		{
			ElementCount += other.ElementCount;
			Bounds.EnlargeByBounds(other.Bounds);
		}

		float GetCost() 
		{
			return ElementCount * Bounds.SurfaceAreaApprox();
		}
	};

	template<typename T>
	inline void FindBestSplitOnAxis(std::vector<T>& elements, BVHNode<T>& currentNode, BestSplit& bestSplit,int axis, float& currentBestCost)
	{
		//Try x buckets
		constexpr int nBuckets = 8;
		SplitBucket buckets[nBuckets];


		auto first = elements.begin() + currentNode.ElementStartIndex;
		auto last = first + currentNode.ElementCount;

		float nodeSizeAlongAxis = currentNode.Bounds.Max[axis] - currentNode.Bounds.Min[axis];
		if (nodeSizeAlongAxis == 0.0f)
		{
			//There are no splits along this axis.
			return;
		}

		//for each element, add to bucket
		for (auto it = first; it != last; ++it)
		{
			T element = *it;
			glm::vec3 elementCenter = element.GetCenter();
			
			float normalizedPosition = (elementCenter[axis] - currentNode.Bounds.Min[axis]) / nodeSizeAlongAxis;
			int bucketIndex = (normalizedPosition * (float)nBuckets);
			if(bucketIndex >= nBuckets)
			{
				bucketIndex = nBuckets - 1;
			}

			buckets[bucketIndex].ElementCount++;
			buckets[bucketIndex].Bounds.EnlargeByBounds(element.GetBounds());
		}

		//There is a single sweep in each direction N time method, TODO: code it later
		// 
		//Check each split and if it's better than the old cost overwrite it
		for (int i = 1; i < nBuckets; i++)
		{
			SplitBucket left;
			SplitBucket right;

			for (int j = 0; j < nBuckets; j++)
			{
				if (j < i)
				{
					left.MergeWith(buckets[j]);
				}
				else
				{
					right.MergeWith(buckets[j]);
				}
			}

			float cost = left.GetCost() + right.GetCost();
			if (cost < currentBestCost)
			{
				currentBestCost = cost;
				bestSplit.Axis = axis;
				bestSplit.Poinnt = currentNode.Bounds.Min[axis] + (i/(float)nBuckets) * nodeSizeAlongAxis;
			}
		}
	}

	template<typename T>
	inline void FindBestSplitAxisAndPoint(std::vector<T>& elements, BVHNode<T>& currentNode, BestSplit& bestSplit)
	{
		float currentBestCost = FLT_MAX;
		for (int i = 0; i < 3; i++)
		{
			FindBestSplitOnAxis(elements, currentNode, bestSplit, i, currentBestCost);
		}
	}

	template<typename T>
	inline int FindSplitIndex(std::vector<T>& elements, BVHNode<T>& currentNode, BestSplit& bestSplit)
	{
		auto first = elements.begin() + currentNode.ElementStartIndex;
		auto last = first + currentNode.ElementCount;

		if (bestSplit.Axis == 0)
		{
			std::sort(first, last, CompareNodesByX<T>);
		}
		else if (bestSplit.Axis == 1)
		{
			std::sort(first, last, CompareNodesByY<T>);
		}
		else
		{
			std::sort(first, last, CompareNodesByZ<T>);
		}

		int endIndex = currentNode.ElementStartIndex + currentNode.ElementCount;
		for (int i = currentNode.ElementStartIndex+1; i < endIndex; i++)
		{
			float elementCenter = elements[i].GetCenter()[bestSplit.Axis];
			if (elementCenter > bestSplit.Poinnt)
			{
				return i;
			}
		}

		//Sommething went really wrong here, return the midpoint I guess
		return currentNode.ElementStartIndex + (currentNode.ElementCount / 2);

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
		BestSplit bestSplit;

		FindBestSplitAxisAndPoint(elements, currentNode, bestSplit);

		int bestSplitIndex = FindSplitIndex(elements, currentNode, bestSplit);

		printf("Best Split: Axis:%d -- Point:%f -- Index:%d \n", bestSplit.Axis, bestSplit.Poinnt, bestSplitIndex);

		//Select an index to split upon
		int splitCount = bestSplitIndex - currentNode.ElementStartIndex;

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
		//Construct initial root node

		nodeContainer.push_back(BVHNode<T>());
		BVHNode<T>& rootNode = nodeContainer.back();
		rootNode.ElementStartIndex = 0;
		rootNode.ElementCount = elements.size();

		//Build BVH recursively
		ConstructBVH(elements, nodeContainer, 0, bvhArgs);
	}
	template<typename T>
	inline bool BVHNode<T>::HasChildren() const
	{
		return ElementCount > 0;
	}
}

