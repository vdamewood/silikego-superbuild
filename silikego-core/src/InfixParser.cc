/* InfixParser.cc: Recursive-descent infix parser
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

#include <Silikego/Lexer.h>
#include <Silikego/InfixParser.h>
#include <Silikego/Value.h>

namespace Silikego
{
	static std::unique_ptr<SyntaxTreeNode> GetExprAddSub(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprAddSubRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprMulDiv(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprMulDivRest(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprExp(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprExpLeftFactor(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprDice(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetExprDiceLeftFactor(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetAtom(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetNumber(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetUNumber(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetFCall(Lexer&);
	static std::unique_ptr<SyntaxTreeNode> GetArguments(Lexer&, const std::string&);


	std::unique_ptr<SyntaxTreeNode> ParseInfix(std::unique_ptr<DataSource> NewSource)
	{
		Lexer MyLexer(std::move(NewSource));

		if (MyLexer.GetToken().Type() == Token::EOL)
		{
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(0));
		}
		else
		{
			std::unique_ptr<SyntaxTreeNode> rVal = GetExprAddSub(MyLexer);
			if (MyLexer.GetToken().Type() != Token::EOL
				&& !rVal->IsError())
				return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));

			return rVal;
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprAddSub(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> Left = GetExprMulDiv(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprAddSubRest(MyLexer);

		if (Rest.get() == nullptr)
			return Left;

		if (dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Left)))
			return Rest;

		return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprAddSubRest(Lexer& MyLexer)
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
		Branch->PushRight(GetExprMulDiv(MyLexer));
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprAddSubRest(MyLexer);

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

	static std::unique_ptr<SyntaxTreeNode> GetExprMulDiv(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> Left = GetExprExp(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprMulDivRest(MyLexer);

		if (Rest.get() == nullptr)
			return Left;

        if (dynamic_cast<BranchNode*>(Rest.get())->GraftLeft(std::move(Left)))
			return Rest;

		return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprMulDivRest(Lexer& MyLexer)
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
        Branch->PushRight(GetExprExp(MyLexer));
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprMulDivRest(MyLexer);

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


	static std::unique_ptr<SyntaxTreeNode> GetExprExp(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> leftValue = GetExprDice(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprExpLeftFactor(MyLexer);

		if (Rest.get() == nullptr)
			return leftValue;

        std::unique_ptr<BranchNode> rVal(new BranchNode("power"));
		rVal->PushRight(std::move(leftValue));
		rVal->PushRight(std::move(Rest));
		return std::move(rVal);
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprExpLeftFactor(Lexer& MyLexer)
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
			return GetExprExp(MyLexer);
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprDice(Lexer& MyLexer)
	{
		std::unique_ptr<SyntaxTreeNode> leftValue = GetAtom(MyLexer);
		std::unique_ptr<SyntaxTreeNode> Rest = GetExprDiceLeftFactor(MyLexer);

		if (Rest.get() == nullptr)
			return leftValue;

        std::unique_ptr<BranchNode> rVal(new BranchNode("dice"));
		rVal->PushRight(std::move(leftValue));
		rVal->PushRight(std::move(Rest));
        return std::move(rVal);
	}

	static std::unique_ptr<SyntaxTreeNode> GetExprDiceLeftFactor(Lexer& MyLexer)
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
			value = GetExprAddSub(MyLexer);

			if (MyLexer.GetToken().Type() != ')')
			{
				return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
			}

			MyLexer.Next();
			return value;
		case Token::ID:
			return GetFCall(MyLexer);
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
			return GetUNumber(MyLexer);
		case '-':
			MyLexer.Next();
			rVal = GetUNumber(MyLexer);
			rVal->Negate();
			return rVal;
		default:
			return std::unique_ptr<SyntaxTreeNode>(new LeafNode(ValueStatus::SYNTAX_ERR));
		}
	}

	static std::unique_ptr<SyntaxTreeNode> GetUNumber(Lexer& MyLexer)
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

	static std::unique_ptr<SyntaxTreeNode> GetFCall(Lexer& MyLexer)
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
			std::unique_ptr<SyntaxTreeNode> Expression = GetExprAddSub(MyLexer);
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
