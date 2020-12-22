//
// Created by Sonullx Liu on 2020-12-18.
//

#ifndef SONULLX_ARGS_H
#define SONULLX_ARGS_H

#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>

namespace sonullx
{
	namespace args
	{
		using LongFlag = std::string;
		using ShortFlag = char;

		enum class FollowingBy
		{
			Zero,
			One,
		};

		class Arg
		{
		public:
			explicit Arg(LongFlag l, ShortFlag s, FollowingBy f)
				: long_flag{std::move(l)}, short_flag{s}, following_by{f}, values{}
			{
			}

			LongFlag long_flag;
			ShortFlag short_flag;
			FollowingBy following_by;
			std::vector<std::string> values;
		};

		bool parse(int argc, const char * const * argv, const std::vector<std::reference_wrapper<Arg>> & args)
		{
			if (argc <= 1 || args.empty())
				return true;

			std::unordered_map<LongFlag, Arg *> long_map;
			std::unordered_map<ShortFlag, Arg *> short_map;

			Arg no_flag_arg{"", '\0', FollowingBy::One};
			Arg * no_flag = &no_flag_arg;

			for (auto arg : args)
			{
				Arg * p = &arg.get();

				if (p->long_flag.empty() && p->short_flag == '\0')
				{
					if (no_flag == &no_flag_arg)
						no_flag = p;
					else
						return false;
				}
				else
				{
					if (!p->long_flag.empty())
					{
						auto iter = long_map.find(p->long_flag);
						if (iter == long_map.end())
							long_map.insert({p->long_flag, p});
						else
							return false;
					}

					if (p->short_flag != '\0')
					{
						auto iter = short_map.find(p->short_flag);
						if (iter == short_map.end())
							short_map.insert({p->short_flag, p});
						else
							return false;
					}
				}
			}

			Arg * key = no_flag;

			enum
			{
				L, S, N
			} type;
			const char * long_flag = nullptr;
			char short_flag = '\0';
			const char * value = nullptr;

			for (int i = 1; i < argc; i++)
			{
				const char * s = argv[i];

				if (s[0] == '-')
				{
					if (s[1] == '-')
					{
						type = L;
						long_flag = s + 2;
					}
					else
					{
						type = S;
						short_flag = s[1];
						value = s[1] ? (s[2] ? s + 2 : nullptr) : nullptr;
					}
				}
				else
				{
					type = N;
					value = s;
				}

				switch (type)
				{
				case L:
					key = no_flag;
					if (*long_flag)
					{
						auto iter = long_map.find(long_flag);
						if (iter != long_map.end())
							key = iter->second;

						switch (key->following_by)
						{
						case FollowingBy::Zero:
							key->values.emplace_back("");
							key = no_flag;
							break;

						case FollowingBy::One:
							break;
						}
					}
					break;

				case S:
					key = no_flag;
					if (short_flag)
					{
						auto iter = short_map.find(short_flag);
						if (iter != short_map.end())
							key = iter->second;

						switch (key->following_by)
						{
						case FollowingBy::Zero:
							key->values.emplace_back("");
							key = no_flag;
							break;

						case FollowingBy::One:
							if (value)
							{
								key->values.emplace_back(value);
								key = no_flag;
							}
							break;
						}
					}
					break;

				case N:
					key->values.emplace_back(value);
					key = no_flag;
					break;
				}

			}

			return true;
		}
	}
}

#endif //SONULLX_ARGS_H
