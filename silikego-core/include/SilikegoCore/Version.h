/* Version.h: Version information for API use
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

#if !defined SILIKEGO_CORE_VERSION_H
#define SILIKEGO_CORE_VERSION_H

#include <SilikegoCore/Api.h>

namespace Silikego::Version
{
    SILIKEGOCORE_EXPORT extern const unsigned int Major;
    SILIKEGOCORE_EXPORT extern const unsigned int Minor;
    SILIKEGOCORE_EXPORT extern const unsigned int Patch;
    SILIKEGOCORE_EXPORT extern const char *PrereleaseSuffix;
    SILIKEGOCORE_EXPORT extern const char *PackageSuffix;
    SILIKEGOCORE_EXPORT extern const char *String;
    SILIKEGOCORE_EXPORT extern const char *StringWithPackage;
    SILIKEGOCORE_EXPORT extern const char *Copyright;
};

#endif /* SILIKEGO_CORE_VERSION_H */
