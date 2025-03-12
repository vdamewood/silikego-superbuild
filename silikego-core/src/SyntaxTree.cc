/* SyntaxTree.cc: Abstract syntax tree classes
 * Copyright 2012-2021 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <list>
#include <memory>

#include <Silikego/SyntaxTree.h>
#include <Silikego/FunctionCaller.h>

namespace Silikego
{
	SyntaxTreeNode::~SyntaxTreeNode() { }

	class LeafNode::State
	{
	public:
		State(long long int NewValue): MyValue(NewValue) { }
		State(double NewValue) : MyValue(NewValue) { }
		State(Value NewValue) : MyValue(NewValue) { }
		State(const State& RightSide) : MyValue(RightSide.MyValue) { }
		State& operator=(const State& RightSide)
		{
			MyValue = RightSide.MyValue;
			return *this;
		}
		Value MyValue;
	};

	LeafNode::LeafNode(Value NewValue) : S(new State(NewValue)) { }

	LeafNode::LeafNode(const LeafNode& RightSide)
	{
		S = new State(*RightSide.S);
	}

	LeafNode::~LeafNode()
	{
		delete S;
	}

	const LeafNode& LeafNode::operator=(const LeafNode& RightSide)
	{
		*S = *RightSide.S;
		return *this;
	}


	Value LeafNode::Evaluate()
	{
		return S->MyValue;
	}

	void LeafNode::Negate()
	{
		S->MyValue.Negate();
	}

	bool LeafNode::IsError()
	{
		return !S->MyValue.IsNumber();
	}

	class BranchNode::State
	{
	public:
		State(const std::string& newName) : Id(newName) {}

		bool IsNegated = false;
		std::string Id;
		std::list< std::unique_ptr<SyntaxTreeNode> > Children;
	};

	BranchNode::BranchNode(const std::string& NewId) : S(new State(NewId))
	{
	}

	BranchNode::~BranchNode()
	{
		delete S;
	}

	Value BranchNode::Evaluate()
	{
		std::vector<Value> Arguments;

		if (S->Children.size())
			for (auto& i : S->Children)
			{
				Value Current = i->Evaluate();
				if (!Current.IsNumber())
					return Current;

				Arguments.push_back(Current);
			}

		Value rVal(FunctionCaller::Call(S->Id.c_str(), Arguments));
		if (S->IsNegated)
		{
			if (rVal.Status() == ValueStatus::INTEGER)
				rVal = rVal.Integer() * -1;
			else if (rVal.Status() == ValueStatus::FLOAT)
				rVal = rVal.Float() * -1.0;
		}

		return rVal;
	}

	void BranchNode::Negate()
	{
		S->IsNegated = !S->IsNegated;
	}

	bool BranchNode::IsError()
	{
		return false;
	}

	void BranchNode::PushLeft(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		S->Children.push_front(std::move(NewChild));
	}

	void BranchNode::PushRight(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		S->Children.push_back(std::move(NewChild));
	}

	bool BranchNode::GraftLeft(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.front() == nullptr)
		{
            S->Children.front() = std::move(NewChild);
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.front().get()))
                return ChildBranch->GraftLeft(std::move(NewChild));
			else
				return false;
		}
	}

	bool BranchNode::GraftRight(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.back() == nullptr)
		{
			S->Children.back() = std::move(NewChild);
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.back().get()))
				return ChildBranch->GraftRight(std::move(NewChild));
			else
				return false;
		}
	}
}
