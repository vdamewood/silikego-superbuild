/* SyntaxTree.h: Abstract syntax tree classes
 * Copyright 2012-2025 Vincent Damewood
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

#if !defined SILIKEGO_SYNTAX_TREE_H
#define SILIKEGO_SYNTAX_TREE_H

#include <memory>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT SyntaxTreeNode
	{
	public:
		virtual ~SyntaxTreeNode();
		virtual Silikego::Value Evaluate(FunctionCaller&) = 0;
		virtual void Negate() = 0;
		virtual bool IsError() = 0;
	};

	class SILIKEGOCORE_EXPORT LeafNode : public SyntaxTreeNode
	{
	public:
		LeafNode(const LeafNode&);
		LeafNode(Value NewValue);
		virtual ~LeafNode();

		const LeafNode& operator=(const LeafNode&);

		virtual Silikego::Value Evaluate(FunctionCaller& caller);
		virtual void Negate();
		virtual bool IsError();
	private:
		class State;
		State *S;
	};

	class SILIKEGOCORE_EXPORT BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(const std::string& NewFunctionId);
		virtual ~BranchNode();

		virtual Silikego::Value Evaluate(FunctionCaller& caller);
		virtual void Negate();
		virtual bool IsError();

		void PushLeft(std::unique_ptr<SyntaxTreeNode>);
		void PushRight(std::unique_ptr<SyntaxTreeNode>);
	private:
		BranchNode(const BranchNode&);
		const BranchNode& operator=(const BranchNode&);
		class State;
		State *S;
	};
};

#endif // SILIKEGO_SYNTAX_TREE_H
