//
// Created by Sonullx Liu on 2020-12-18.
//

#include <iostream>

#include <sonullx/args.h>

int main()
{
	const int argc = 18;
	const char * argv[argc] = {"main", "-a", "-b", "b.x", "--best", "b.xx", "fox", "foxtrot", "-c10K", "--duration", "1s", "--eof", "-b", "b.xx", "--best", "b.xxx", "-c1M", "--eof"};

	{
		using namespace sonullx::args;

		Arg a{"", 'a', FollowingBy::Zero};
		Arg b{"best", 'b', FollowingBy::One};
		Arg c{"", 'c', FollowingBy::One};
		Arg d{"duration", '\0', FollowingBy::One};
		Arg e{"eof", '\0', FollowingBy::Zero};
		Arg f{"", '\0', FollowingBy::One};

		parse(argc, argv, {a, b, c, d, e, f});

		auto print = [](const std::vector<std::string> & v)
		{
			std::cout << v.size() << std::endl;
			for (const auto & s : v)
				std::cout << "[" << s  << "], ";

			std::cout << std::endl << std::endl;
		};

		print(a.values);
		print(b.values);
		print(c.values);
		print(d.values);
		print(e.values);
		print(f.values);
	}



	return 0;
}
