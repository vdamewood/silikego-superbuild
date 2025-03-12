/* Value.cc: Data structure to represent a single value
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

#if defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <sstream>
#include <limits>

#include <Silikego/Value.h>

namespace Silikego
{
	class Value::State
	{
	public:
		State(long long int NewInteger) : Status(ValueStatus::INTEGER), Integer(NewInteger) { }
		State(double NewFloat) : Status(ValueStatus::FLOAT), Float(NewFloat) { }
		State(ValueStatus NewStatus) : Status(NewStatus) { }
		State(const State &RightSide) : Status(RightSide.Status)
		{
			switch (Status)
			{
			case ValueStatus::INTEGER:
				Integer = RightSide.Integer;
				break;
			case ValueStatus::FLOAT:
				Float = RightSide.Float;
				break;
			default:
				; // Do nothing. Silence warning.
			}
		}

		State& operator=(const State &RightSide)
		{
			Status = RightSide.Status;
			switch (Status)
			{
			case ValueStatus::INTEGER:
				Integer = RightSide.Integer;
				break;
			case ValueStatus::FLOAT:
				Float = RightSide.Float;
				break;
			default:
				; // Do nothing. Silence warning.
			}
			return *this;
		}

		ValueStatus Status;
		union
		{
			long long int Integer;
			double Float;
		};
	};

	Value::Value(ValueStatus NewStatus) : S(new State(NewStatus)) { }
	Value::Value(short NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Value::Value(int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Value::Value(long int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	Value::Value(long long int NewValue) : S(new State(NewValue)) { }
	Value::Value(float NewValue) : S(new State(static_cast<double>(NewValue))) { }
	Value::Value(double NewValue) : S(new State(NewValue)) { }
	Value::Value(const Value& RightSide) : S(new State(*RightSide.S)) { }
	Value::~Value()
	{
		delete S;
	}

	const Value& Value::operator=(const Value& RightSide)
	{
		*S = *RightSide.S;
		return *this;
	}

	const Value& Value::Negate()
	{
		switch (S->Status)
		{
		case (ValueStatus::INTEGER):
			S->Integer *= -1;
			break;
		case (ValueStatus::FLOAT):
			S->Float *= -1.0;
			break;
		default:
			; // Do nothing. Silence warning.
		}
		return *this;
	}

	ValueStatus Value::Status() const
	{
		return S->Status;
	}

	long long int Value::Integer() const
	{
		if (S->Status == ValueStatus::INTEGER)
			return S->Integer;
		else if (S->Status == ValueStatus::FLOAT)
			return static_cast<int>(S->Float);
		else
			return 0;
	}

	double Value::Float() const
	{
		if (S->Status == ValueStatus::INTEGER)
			return static_cast<double>(S->Integer);
		else if (S->Status == ValueStatus::FLOAT)
			return S->Float;
		else
			return std::numeric_limits<double>::quiet_NaN();
	}

	bool Value::IsNumber() const
	{
		return (S->Status == ValueStatus::INTEGER || S->Status == ValueStatus::FLOAT);
	}

	std::string Value::ToString() const
	{
		std::ostringstream tmp;
		switch (S->Status)
		{
		case ValueStatus::INTEGER:
			tmp << Integer();
			break;
		case ValueStatus::FLOAT:
			tmp << Float();
			break;
		case ValueStatus::MEMORY_ERR:
			tmp << "Error: Out of memory";
			break;
		case ValueStatus::SYNTAX_ERR:
			tmp << "Error: Syntax error";
			break;
		case ValueStatus::ZERO_DIV_ERR:
			tmp << "Error: Division by zero";
			break;
		case ValueStatus::BAD_FUNCTION:
			tmp << "Error: Function not found";
			break;
		case ValueStatus::BAD_ARGUMENTS:
			tmp << "Error: Bad argument count";
			break;
		case ValueStatus::DOMAIN_ERR:
			tmp << "Error: Domain error";
			break;
		case ValueStatus::RANGE_ERR:
			tmp << "Error: Range error";
		}

		return tmp.str();
	}
}
