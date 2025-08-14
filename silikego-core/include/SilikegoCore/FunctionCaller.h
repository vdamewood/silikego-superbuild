/* FunctionCaller.h: Function handling
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

#if !defined SILIKEGO_FUNCTION_CALLER_H
#define SILIKEGO_FUNCTION_CALLER_H

#include <string>
#include <vector>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Value.h>


namespace Silikego
{
	class SILIKEGOCORE_EXPORT FunctionCaller
	{
	public:
		typedef Value (*FunctionPointer)(std::vector<Value>);

		FunctionCaller();
		~FunctionCaller();

		Value Call(const std::string &Name, std::vector<Value> Args);
		void Install(const std::string &Name, FunctionPointer Function);
		void InstallOperators();
		void InstallFunctions();

	private:
		class State;
		State *S;	
	};
};
#endif // SILIKEGO_FUNCTION_CALLER_H
