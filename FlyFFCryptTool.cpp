#include <iostream>
#include "resource.h"
#include "resourceManager.h"
#include "resourceExtractor.h"
#include "resourceSettings.h"
#pragma comment(lib, "./Cryptopp/Lib/cryptlib.lib")

#include "cryptEngine.h"


res::manager rm;
/**	@brief	Example usage using the main function for the application. */
int main()
{	
	cryptEngine::defAes::m_crypt.SetParameters(192);
//	std::cout << tempStr << std::endl;
	std::cout << "Please insert Insanity flyff client folder" << std::endl;
	std::getline(std::cin, res::extract::folderHehe);

	const std::string temp = res::extract::folderHehe;
	//rm.loadPackedClient<res::vp_Insanity>(temp, ".res");
	rm.loadPackedFile<res::vp_Forsaken>(temp);
	//check1

	const std::string ch = cryptEngine::getSha256("define.h", "");
	if (const auto [fst, snd] = rm.testEntropia(ch); snd)
	{
		rm.extractSingleFile(fst->first, "define.h");
		//std::cout << " found : " << ch << " " << check1 << std::endl;
		//std::ofstream ofs("hash.txt", std::ios::binary | std::ios::out);
		//ofs.write(ch.data(), ch.size());
		//ofs.close();
		//return 0;
	}


	if (const auto [fst, snd] = rm.findFile("data/spec_item.txt"); snd)
	{
		std::cout << "spec item found" << std::endl;
	}
	if (const auto [fst, snd] = rm.findFile("Data\\mdlObj.inc"); snd)
	{
		std::cout << "mdl item found" << std::endl;
	}
	//rm.loadPackedClient<res::vp_Forsaken>(temp, ".fast");
	//rm.loadProject();

	//rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wd1");
	//rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wd2");
	//


	std::cout << "press any key to end" << std::endl;
	std::cin.get();
	return 0;
}
