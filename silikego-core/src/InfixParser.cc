/* InfixParser.cc: Recursive-descent infix parser
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

#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/InfixParser.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	static std::unique_ptr<SyntaxTreeNode> GetExprssion(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprssionRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetTerm(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetTermRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExponent(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExponentRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetRoll(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetRollRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetAtom(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetNumber(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetUnsignedNumber(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetFunctionCall(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetArguments(Lexer&, const std::string&);


	std::unique_ptr<SyntaxTreeNode> ParseInfix(std::unique_ptr<DataSource> NewSource)
	{
		Lexer MyLexer(std::move(NewSource));

		std::unique_ptr<SyntaxTreeNode> rVal = GetExprssion(MyLexer);
		if (MyLexer.GetToken().Type() != Token::EOL
			&& !rVal->IsError())
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));

		return rVal;
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprssion(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> Left = GetTerm(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprssionRest(MyLexer);

		if (Rest.get() == nullptr)
			return Left;

		if (dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Left)))
			return Rest;

		return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprssionRest(Lexer& MyLexer)
	{
		const char *FunctionId;
		switch (MyLexer.GetToken().Type())
		{
		case '+':
			FunctionId = "add";
			break;
		case '-':
			FunctionId = "subtract";
			break;
		default:
			return std::unique_ptr<SyntaxTreeNode>(nullptr);
		}

		MyLexer.Next();

		std::unique_ptr<BranchNode> Branch(new BranchNode(FunctionId));
		Branch->PushRight(nullptr);
		Branch->PushRight(GetTerm(MyLexer));
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprssionRest(MyLexer);

		if (Rest.get() == nullptr)
		{
			return std::move(Branch);
		}
		else
		{
            dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Branch));
			return Rest;
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetTerm(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> Left = GetExponent(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetTermRest(MyLexer);

		if (Rest.get() == nullptr)
			return Left;

        if (dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Left)))
			return Rest;

		return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
	}

	static std::unique_ptr<SyntaxTreeNode> GetTermRest(Lexer& MyLexer)
	{
		const char *FunctionId;
		switch (MyLexer.GetToken().Type())
		{
			case '*':
				FunctionId = "multiply";
				break;
			case '/':
				FunctionId = "divide";
				break;
			default:
				return std::unique_ptr<SyntaxTreeNode>(nullptr);
		}

		MyLexer.Next();

        std::unique_ptr<BranchNode> Branch(new BranchNode(FunctionId));
		Branch->PushRight(0);
        Branch->PushRight(GetExponent(MyLexer));
		std::unique_ptr<SyntaxTreeNode> Rest = GetTermRest(MyLexer);

		if (Rest.get() == nullptr)
		{
            return std::move(Branch);
		}
		else
		{
            dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Branch));
			return Rest;
		}
	}


	static std::unique_ptr<SyntaxTreeNode> GetExponent(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> leftValue = GetRoll(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExponentRest(MyLexer);

		if (Rest.get() == nullptr)
			return leftValue;

        std::unique_ptr<BranchNode> rVal(new BranchNode("power"));
		rVal->PushRight(std::move(leftValue));
		rVal->PushRight(std::move(Rest));
		return std::move(rVal);
	}

	static std::unique_ptr<SyntaxTreeNode> GetExponentRest(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != '^')
			return std::unique_ptr<SyntaxTreeNode>(nullptr);

		MyLexer.Next();

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
		case '-':
		case Token::ID:
		case '(':
			return GetExponent(MyLexer);
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetRoll(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> leftValue = GetAtom(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetRollRest(MyLexer);

		if (Rest.get() == nullptr)
			return leftValue;

        std::unique_ptr<BranchNode> rVal(new BranchNode("dice"));
		rVal->PushRight(std::move(leftValue));
		rVal->PushRight(std::move(Rest));
        return std::move(rVal);
	}

	static std::unique_ptr<SyntaxTreeNode> GetRollRest(Lexer& MyLexer)
	{
		if(MyLexer.GetToken().Type() != 'd')
			return std::unique_ptr<SyntaxTreeNode>(nullptr);

		MyLexer.Next();

		if (MyLexer.GetToken().Type() == Token::INTEGER)
		{
			std::unique_ptr<SyntaxTreeNode> rVal(new LeafNode(MyLexer.GetToken().Integer()));
			MyLexer.Next();
			return rVal;
		}
		else
		{
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetAtom(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> value;

		switch(MyLexer.GetToken().Type())
		{
		case '-':
		case Token::INTEGER:
		case Token::FLOAT:
			return GetNumber(MyLexer);
		case '(':
			MyLexer.Next();
			value = GetExprssion(MyLexer);

			if (MyLexer.GetToken().Type() != ')')
			{
				return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
			}

			MyLexer.Next();
			return value;
		case Token::ID:
			return GetFunctionCall(MyLexer);
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetNumber(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> rVal;

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
			return GetUnsignedNumber(MyLexer);
		case '-':
			MyLexer.Next();
			rVal = GetUnsignedNumber(MyLexer);
			rVal->Negate();
			return rVal;
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetUnsignedNumber(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> rVal;

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
            rVal = std::unique_ptr<SyntaxTreeNode>(new LeafNode(MyLexer.GetToken().Integer()));
			MyLexer.Next();
			return rVal;
		case Token::FLOAT:
            rVal = std::unique_ptr<SyntaxTreeNode>(new LeafNode(MyLexer.GetToken().Float()));
			MyLexer.Next();
			return rVal;
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetFunctionCall(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != Token::ID)
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));

        std::string FunctionName = MyLexer.GetToken().Id();
        MyLexer.Next();

		if (MyLexer.GetToken().Type() != '(')
            return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));

        MyLexer.Next();

        std::unique_ptr<SyntaxTreeNode> rVal(GetArguments(MyLexer, FunctionName));

		if (MyLexer.GetToken().Type() != ')')
            return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));

        MyLexer.Next();
        return rVal;
	}

	static std::unique_ptr<SyntaxTreeNode> GetArguments(Lexer& MyLexer, const std::string& FName)
	{
		std::unique_ptr<BranchNode> rVal(new BranchNode(FName));
		while(true)
		{
			std::unique_ptr<SyntaxTreeNode> Expression = GetExprssion(MyLexer);
            bool IsError = Expression->IsError();
            rVal->PushRight(std::move(Expression));

			if (IsError
				|| MyLexer.GetToken().Type() == ')')
			{
				break;
			}
			else if (MyLexer.GetToken().Type() != ',')
			{
                rVal->PushRight(std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR)));
				break;
			}
			MyLexer.Next();
		}
        return std::move(rVal);
	}
}
