/* FunctionCaller.cc: Function handling
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

#include <unordered_map>
#include <stdexcept>

#include <Silikego/FunctionCaller.h>

#include "Functions.h"

namespace Silikego
{

	// This is initialized as a null pointer rather than a static object so that
	// bindings can be created for language interpreters written in C.
	std::unordered_map<std::string, FunctionCaller::FunctionPointer> *lookup = 0;

	bool FunctionCaller::SetUp()
	{
		try
		{
			lookup = new std::unordered_map<std::string,
				FunctionCaller::FunctionPointer>();
		}
		catch (...)
		{
			lookup = 0;
			return false;
		}

		// Built-in operators
		Install("add", Functions::add);
		Install("subtract", Functions::subtract);
		Install("multiply", Functions::multiply);
		Install("divide", Functions::divide);
		Install("power", Functions::power);
		Install("dice", Functions::dice);

		// Math library functions
		Install("abs", Functions::abs);
		Install("acos", Functions::acos);
		Install("asin", Functions::asin);
		Install("atan", Functions::atan);
		Install("ceil", Functions::ceil);
		Install("cos", Functions::cos);
		Install("cosh", Functions::cosh);
		Install("exp", Functions::exp);
		Install("floor", Functions::floor);
		Install("log", Functions::log);
		Install("log10", Functions::log10);
		Install("sin", Functions::sin);
		Install("sinh", Functions::sinh);
		Install("sqrt", Functions::sqrt);
		Install("tan", Functions::tan);
		Install("tanh", Functions::tanh);

		return true;
	}

	void FunctionCaller::TearDown()
	{
		delete lookup;
		lookup = 0;
	}

	void FunctionCaller::Install(const std::string &Name, FunctionPointer Function)
	{
		(*lookup)[Name] = Function;
	}

	Value FunctionCaller::Call(const std::string &Name, std::vector<Value> Args)
	try
	{
		return lookup->at(Name)(Args);
	}
	catch (const std::out_of_range &)
	{
		return ValueStatus::BAD_FUNCTION;
	}
}
