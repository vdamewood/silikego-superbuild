/* Lexer.h: Lexical analyzer
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

#if !defined SILIKEGO_LEXER_H
#define SILIKEGO_LEXER_H

#include <memory>

#include <Silikego/Api.h>
#include <Silikego/DataSource.h>
#include <Silikego/Token.h>

namespace Silikego
{
	class SILIKEGO_EXPORT Lexer
	{
	public:
		Lexer(std::unique_ptr<DataSource> InputSource);
		~Lexer();

		Silikego::Token &GetToken();
		void Next();

	private:
		Lexer(const Lexer&) = delete;
		const Lexer& operator=(const Lexer&) = delete;
		class State;
		State *S;
	};
};

#endif // SILIKEGO_LEXER_H
