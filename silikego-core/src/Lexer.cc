/* Lexer.cc: Lexical analyzer
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

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <memory>

#include <Silikego/Lexer.h>

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static int isOperator(int character)
{
	return
	(
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')'
	);
}

static int isIdCharacter(int character)
{
	return (std::isalnum(character) || character == '_');
}

enum SilikegoDfaState
{
	DFA_ERROR = -1,
	DFA_START = 0,	/* "" */
	DFA_E,		/* "e" */
	DFA_PI_1,	/* "p" */
	DFA_PI_2,	/* "pi" */
	DFA_DICE,	/* "d" */
	DFA_ID,		/* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
	DFA_INTEGER,	/* [0-9]+ */
	DFA_FLOAT,	/* [0-9]+\.[0-9]+ */
	DFA_TERM_INTEGER,
	DFA_TERM_FLOAT,
	DFA_TERM_E,
	DFA_TERM_PI,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOI,
	DFA_END
};
namespace Silikego
{
	class Lexer::State
	{
	public:
		State(std::unique_ptr<DataSource> NewSource)
			: Source(std::move(NewSource))
		{
		}

		std::unique_ptr<DataSource> Source;
		Silikego::Token Token = Token::UNSET;
	};

	Lexer::Lexer(std::unique_ptr<DataSource> InputSource) : S(new State(std::move(InputSource)))
	{
		Next();
	}

	Lexer::~Lexer()
	{
		delete S;
	}

	void Lexer::Next()
	{
		SilikegoDfaState dfaState = DFA_START;
		std::string lexeme = std::string();

		if (S->Token.Type() == Token::EOL || S->Token.Type() == Token::ERROR)
			return;

		S->Token = Token::UNSET;

		while (dfaState != DFA_END)
		switch (dfaState)
		{
		case DFA_END:
			break;
		case DFA_START:
			if(isOperator(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_TERM_CHAR;
			}
			else if (S->Source->GetCurrent() == 'd')
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_DICE;
			}
			else if (S->Source->GetCurrent() == 'e')
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_E;
			}
			else if (S->Source->GetCurrent() == 'p')
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_PI_1;
			}
			else if (std::isdigit(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_INTEGER;
			}
			else if (std::isalpha(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_ID;
			}
			else if (std::isspace(S->Source->GetCurrent()))
			{
				S->Source->Advance();
			}
			else if (S->Source->GetCurrent() == '\0')
			{
				dfaState = DFA_TERM_EOI;
			}
			else
			{
				dfaState = DFA_ERROR;
			}
			break;
		case DFA_DICE:
			if (std::isalpha(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_E:
			if (std::isalnum(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_E;
			}
			break;
		case DFA_PI_1:
			if (S->Source->GetCurrent() == 'i')
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_PI_2;
			}
			else if (isIdCharacter(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_PI_2:
			if (isIdCharacter(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_PI;
			}
			break;
		case DFA_ID:
			if (isalnum(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_INTEGER:
			if (S->Source->GetCurrent() == '.')
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
				dfaState = DFA_FLOAT;
			}
			else if (std::isdigit(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
			}
			else
			{
				dfaState = DFA_TERM_INTEGER;
			}
			break;
		case DFA_FLOAT:
			if (std::isdigit(S->Source->GetCurrent()))
			{
				lexeme += S->Source->GetCurrent();
				S->Source->Advance();
			}
			else
			{
				dfaState = DFA_TERM_FLOAT;
			}
			break;
		case DFA_TERM_INTEGER:
			S->Token = Token(std::strtoll(lexeme.c_str(), 0, 10));
			dfaState = DFA_END;
			break;
		case DFA_TERM_FLOAT:
			S->Token = Token(std::atof(lexeme.c_str()));
			dfaState = DFA_END;
			break;
		case DFA_TERM_E:
			S->Token = Token(EULER);
			dfaState = DFA_END;
			break;
		case DFA_TERM_PI:
			S->Token = PI;
			dfaState = DFA_END;
			break;
		case DFA_TERM_CHAR:
			S->Token = static_cast<Token::TokenType>(lexeme[0]);
			dfaState = DFA_END;
			break;
		case DFA_TERM_STRING:
			S->Token = lexeme;
			dfaState = DFA_END;
			break;
		case DFA_TERM_EOI:
			S->Token = Token::EOL;
			dfaState = DFA_END;
			break;
		case DFA_ERROR:
			S->Token = Token::ERROR;
			dfaState = DFA_END;
			break;
		}
	}

	Token &Lexer::GetToken()
	{
		return S->Token;
	}
}
