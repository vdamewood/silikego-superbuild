/* StringSource.h: Class to input data from a string
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

#if !defined SILIKEGO_STRING_SOURCE_H
#define SILIKEGO_STRING_SOURCE_H

#include <string>

#include <Silikego/Api.h>
#include <Silikego/DataSource.h>

namespace Silikego
{
	class SILIKEGO_EXPORT StringSource : public DataSource
	{
	public:
		StringSource(const char *);
		StringSource(const std::string &);
		virtual ~StringSource();

		virtual bool Advance();
		virtual char GetCurrent();

	private:
		StringSource(const StringSource&) = delete;
		const StringSource& operator=(const StringSource&) = delete;
		class State;
		State* S;
	};
};

#endif // SILIKEGO_STRING_SOURCE_H
