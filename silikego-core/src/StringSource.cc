/* StringSource.cc: Class to input data from a string
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


#include <string>

#include <Silikego/StringSource.h>

namespace Silikego
{
	class StringSource::State
	{
	public:
		State(const char* NewInput) : Input(NewInput) { }
		State(const std::string &NewInput) : Input(NewInput) { }

		std::string Input;
		std::string::iterator Index = Input.begin();
	};

	StringSource::StringSource(const char *NewSource)
		: S(new State(NewSource))
	{
	}

	StringSource::StringSource(const std::string &NewSource)
		: S(new State(NewSource))
	{
	}

	StringSource::~StringSource()
	{
		delete S;
	}

	bool StringSource::Advance()
	{
		if (S->Index != S->Input.end())
		{
			S->Index++;
			return true;
		}
		else
		{
			return false;
		}
	}

	char StringSource::GetCurrent()
	{
		if (S->Index != S->Input.end())
			return *S->Index;
		else
			return '\0';
	}
}
