#include <iostream>
#include "resource.h"
#include "resourceManager.h"
#include "resourceExtractor.h"
#pragma comment(lib, "./Cryptopp/Lib/cryptlib.lib")

#include "cryptEngine.h"

/**	@brief	Example usage using the main function for the application. */
int main()
{
	res::manager rm;
	cryptEngine::defAes::m_crypt.SetParameters(192);

	std::cout << "Please insert flyff client folder" << std::endl;
	std::getline(std::cin, res::extract::folderHehe);

	const std::string temp = res::extract::folderHehe;
	rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wdf");
	rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wd1");
	rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wd2");

	if (const auto [fst, snd] = rm.findFile("addons\\configure"); snd)
	{
		rm.extractSingleFile(fst->first, "addons\\configure");
	}

	//rm.loadProjectForsaken();
	std::cout << "press any key to end" << std::endl;
	std::cin.get();
	return 0;
}
