/* Functions.cc: Built-in functions
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

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "Functions.h"

namespace Silikego
{
	Value Functions::add(std::vector<Value> Args)
	{
		if (!Args.size())
			return 0;

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.Status())
			{
			case ValueStatus::INTEGER:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Integer() + i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Integer() + i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			case ValueStatus::FLOAT:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Float() + i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Float() + i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			default:
				return ValueStatus::BAD_ARGUMENTS;
			}
		return rVal;
	}

	Value Functions::subtract(std::vector<Value> Args)
	{
		if (!Args.size())
			return 0;

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.Status())
			{
			case ValueStatus::INTEGER:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Integer() - i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Integer() - i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			case ValueStatus::FLOAT:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Float() - i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Float() - i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			default:
				return ValueStatus::BAD_ARGUMENTS;
			}
		return rVal;
	}

	Value Functions::multiply(std::vector<Value> Args)
	{
		if (!Args.size())
			return 0;

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.Status())
			{
			case ValueStatus::INTEGER:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Integer() * i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Integer() * i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			case ValueStatus::FLOAT:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Float() * i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Float() * i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			default:
				return ValueStatus::BAD_ARGUMENTS;
			}
		return rVal;	}

	Value Functions::divide(std::vector<Value> Args)
	{
		if (Args.size() < 2)
			return ValueStatus::BAD_ARGUMENTS;

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
		{
			/* Division-by-Zero Error */
			if ((i->Status() == ValueStatus::FLOAT && i->Float() == 0.0)
				|| (i->Status() == ValueStatus::INTEGER && i->Integer() == 0))
			{
				return ValueStatus::ZERO_DIV_ERR;
			}

			switch (rVal.Status())
			{
			case ValueStatus::INTEGER:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					if (rVal.Integer() % i->Integer() == 0)
						rVal = rVal.Integer() / i->Integer();
					else
						rVal = static_cast<double>(rVal.Integer())
							/ static_cast<double>(i->Integer());
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Integer() / i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			case ValueStatus::FLOAT:
				switch(i->Status())
				{
				case ValueStatus::INTEGER:
					rVal = rVal.Float() / i->Integer();
					break;
				case ValueStatus::FLOAT:
					rVal = rVal.Float() / i->Float();
					break;
				default:
					return ValueStatus::BAD_ARGUMENTS;
				}
				break;
			default:
				return ValueStatus::BAD_ARGUMENTS;
			}
		}
		return rVal;
	}

	Value Functions::power(std::vector<Value> Args)
	{
		double runningValue = Args[0].Float();
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			runningValue = std::pow(runningValue, i->Float());
		return runningValue;
	}


	Value Functions::dice(std::vector<Value> Args)
	{
		static int hasSeeded = 0;

		if(Args.size() != 2)
			return ValueStatus::BAD_ARGUMENTS;

		if (!hasSeeded)
		{
			hasSeeded = 1;
			std::srand((unsigned int)std::time(NULL));
		}

		long long int runningTotal = 0;
		for (int i = 1; i <= Args[0].Integer(); i++)
			runningTotal += (std::rand() % Args[1].Integer()) + 1;

		return runningTotal;
	}

	Value Functions::abs(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		switch (Args[0].Status())
		{
		case ValueStatus::FLOAT:
			return std::abs(Args[0].Float());
		case ValueStatus::INTEGER:
			return std::abs(Args[0].Integer());
		default:
			return Args[0];
		}
	}

	Value Functions::acos(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		if (Args[0].Float() < -1 || Args[0].Float() > 1)
			return ValueStatus::DOMAIN_ERR;

		return std::acos(Args[0].Float());
	}

	Value Functions::asin(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		if (Args[0].Float() < -1 || Args[0].Float() > 1)
			return ValueStatus::DOMAIN_ERR;

		return std::asin(Args[0].Float());
	}

	Value Functions::atan(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::atan(Args[0].Float());
	}

	Value Functions::ceil(std::vector<Value> Args)
	{
		double result;

		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		result = std::ceil(Args[0].Float());

		if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
			return static_cast<long long int>(result);
		else
			return result;
	}

	Value Functions::cos(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::cos(Args[0].Float());
	}

	Value Functions::cosh(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::cosh(Args[0].Float());
	}

	Value Functions::exp(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::exp(Args[0].Float());
	}

	Value Functions::floor(std::vector<Value> Args)
	{
		double result;

		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		result = std::floor(Args[0].Float());

		if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
			return static_cast<long long int>(result);
		else
			return result;
	}

	Value Functions::log(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::log(Args[0].Float());
	}

	Value Functions::log10(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::log10(Args[0].Float());
	}

	Value Functions::sin(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::sin(Args[0].Float());
	}

	Value Functions::sinh(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::sinh(Args[0].Float());
	}

	Value Functions::sqrt(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		if (Args[0].Float() < 0.0)
			return ValueStatus::DOMAIN_ERR;

		return std::sqrt(Args[0].Float());
	}

	Value Functions::tan(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::tan(Args[0].Float());
	}

	Value Functions::tanh(std::vector<Value> Args)
	{
		if (Args.size() != 1)
			return ValueStatus::BAD_ARGUMENTS;

		return std::tanh(Args[0].Float());
	}
}
