/* Token.cc: Token passed from tokenizer to parser
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

#include <Silikego/Token.h>

namespace Silikego
{
	class Token::State
	{
	public:
		State(TokenType NewType) : Type(NewType) {}
		State(long long int NewValue) : Type(INTEGER), Integer(NewValue) {}
		State(double NewValue) : Type(FLOAT), Float(NewValue) {}
		State(const std::string& NewId) : Type(ID), Id(NewId) {}
		State(const State& RightSide) : Type(RightSide.Type)
		{
			switch (Type)
			{
			case INTEGER:
				Integer = RightSide.Integer;
				break;
			case FLOAT:
				Float = RightSide.Float;
				break;
			case ID:
				Id = RightSide.Id;
				break;
			default:
				; // Do nothing. Slience warning.
			}
		}

		State& operator=(const State& RightSide)
		{
			Type = RightSide.Type;
			switch (Type)
			{
			case INTEGER:
				Integer = RightSide.Integer;
				break;
			case FLOAT:
				Float = RightSide.Float;
				break;
			case ID:
				Id = RightSide.Id;
				break;
			default:
				; // Do nothing. Slience warning.
			}
			return *this;
		}

		TokenType Type;
		union
		{
			long long int Integer;
			double Float;
		};
		std::string Id;
	};

	Token::Token(TokenType NewType) : S(new State(NewType)) { }
	Token::Token(short NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Token::Token(int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Token::Token(long int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Token::Token(long long int NewValue) : S(new State(NewValue)) { }
	Token::Token(float NewValue) : S(new State(static_cast<double>(NewValue))) { }
	Token::Token(double NewValue) : S(new State(NewValue)) { }
	Token::Token(const std::string& NewId) : S(new State(NewId)) { }
	Token::Token(const Token& RightSide) : S(new State(*RightSide.S)) { }

	Token::~Token()
	{
		delete S;
	}

	const Token& Token::operator=(const Token& RightSide)
	{
		*S = *RightSide.S;
		return *this;
	}

	Token::TokenType Token::Type() const
	{
		return S->Type;
	}

	long long int Token::Integer() const
	{
		return S->Integer;
	}

	double Token::Float() const
	{
		return S->Float;
	}

	const char *Token::Id() const
	{
		return S->Id.c_str();
	}
}
