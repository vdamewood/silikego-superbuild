/* Value.h: Data structure to represent a single value
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

#if !defined SILIKEGO_VALUE_H
#define SILIKEGO_VALUE_H

#include <string>

#include <Silikego/Api.h>

namespace Silikego
{
	enum class ValueStatus
	{
		INTEGER,       /* Success: The value is an integer. */
		FLOAT,         /* Success: The valis is a double. */
		MEMORY_ERR,    /* Error: An attempt to allocate memory
		                    failed. */
		SYNTAX_ERR,    /* Error: A syntax error was
		                    encountered. */
		ZERO_DIV_ERR,  /* Error: Division by zero was
		                    attempted. */
		BAD_FUNCTION,  /* Error: A function call could not be
		                    resolved to a valid function. */
		BAD_ARGUMENTS, /* Error: A function call was made with
		                    a number of arguments that the
		                    function can't accept. */
		DOMAIN_ERR,    /* Error: A function call resulted in a
		                    domain error. */
		RANGE_ERR      /* Error: A function call resulted in a
		                    range error. */
	};

	class SILIKEGO_EXPORT Value
	{
	public:

		Value();
		Value(ValueStatus NewStatus);
		Value(short int newValue);
		Value(int NewValue);
		Value(long int NewValue);
		Value(long long int NewValue);
		Value(float NewValue);
		Value(double NewValue);
		Value(const Value&);
		~Value();

		const Value& operator=(const Value& other);
		const Value& Negate();

		ValueStatus Status() const;
		long long int Integer() const;
		double Float() const;

		bool IsNumber() const;
		std::string ToString() const;
	private:
		class State;
		State *S;
	};
}

#endif // SILIKEGO_VALUE_H
