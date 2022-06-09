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
	cryptEngine::defAes::m_crypt.StartDecryption(tmmo_key);

	std::cout << "Please insert Gods Of War FlyFF folder" << std::endl;
	std::getline(std::cin, res::extract::folderHehe);

	const std::string temp = res::extract::folderHehe;// level flyff .lvf
	rm.loadPackedClient<res::vp_AesGow>(temp, tmmo_ext);
	rm.extractAllArchives();

	//if (const auto [fst, snd] = rm.findFile("mdlobj.inc"); snd)
	//{
	//	rm.extractSingleFile("mdlobj.inc");
	//}	
	//if (const auto [fst, snd] = rm.findFile("mdldyna.inc"); snd)
	//{
	//	rm.extractSingleFile("mdldyna.inc");
	//}
	//rm.loadProject();


	std::cout << "press any key to end" << std::endl;
	std::cin.get();
	return 0;
}
