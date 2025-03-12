/* Functions.h: Built-in functions
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

#if !defined SILIKEGO_FUNCTIONS_H
#define SILIKEGO_FUNCTIONS_H

#include <vector>

#include <Silikego/Value.h>

namespace Silikego
{
	namespace Functions
	{
		Silikego::Value add(std::vector<Silikego::Value> Args);
		Silikego::Value subtract(std::vector<Silikego::Value> Args);
		Silikego::Value multiply(std::vector<Silikego::Value> Args);
		Silikego::Value divide(std::vector<Silikego::Value> Args);
		Silikego::Value power(std::vector<Silikego::Value> Args);
		Silikego::Value dice(std::vector<Silikego::Value> Args);
		Silikego::Value abs(std::vector<Silikego::Value> Args);
		Silikego::Value acos(std::vector<Silikego::Value> Args);
		Silikego::Value asin(std::vector<Silikego::Value> Args);
		Silikego::Value atan(std::vector<Silikego::Value> Args);
		Silikego::Value ceil(std::vector<Silikego::Value> Args);
		Silikego::Value cos(std::vector<Silikego::Value> Args);
		Silikego::Value cosh(std::vector<Silikego::Value> Args);
		Silikego::Value exp(std::vector<Silikego::Value> Args);
		Silikego::Value floor(std::vector<Silikego::Value> Args);
		Silikego::Value log(std::vector<Silikego::Value> Args);
		Silikego::Value log10(std::vector<Silikego::Value> Args);
		Silikego::Value sin(std::vector<Silikego::Value> Args);
		Silikego::Value sinh(std::vector<Silikego::Value> Args);
		Silikego::Value sqrt(std::vector<Silikego::Value> Args);
		Silikego::Value tan(std::vector<Silikego::Value> Args);;
		Silikego::Value tanh(std::vector<Silikego::Value> Args);
	};
};
#endif // SILIKEGO_FUNCTIONS_H
