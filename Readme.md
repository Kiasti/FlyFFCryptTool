# FlyFF Crypt Tool

A tool made to unpack the client data from the mmorpg FlyFF and to unpack other variations being used on private servers of the mmorpg. This includes released methods in the private server scene, like the AES implementation from the release that was labeled GoW and the one from the Azure source.

Other methods were obtained from reverse engineering clients.

This does contain zlib  and crypto++ as eventually It will offer using zlib's compression, and currently it implements sha256 from the crypto++ library.

if you have any questions or concerns
	Kia#1411 - Discord
	

# Usages
As of current, there is no particular usage for the application. To use it, one must edit the main function in the **FlyFFCrypttool.cpp** file to use the implementations - or import the code in your own project.

```cpp
	// manager is a class to handle all the Resources.
	res::manager rm; 

	// the function loadPackedClient<T> will accept a string of the clients location and the extention string in which to unpack. 
	// T is expected to be of one of the types listed in fs_Variant.
	rm.loadPackedClient<T>(folderString, extentionString);
	
	// Another example to load the structure from  https://ff.163.com
	rm.loadPackedClient<res::vp_NewFeiFei>(temp, ".wdf");

	// You can load a specific file with
	loadPackedFile<T>(fileName);
	
	// rm.findFile returns a pair. Fst being an iterator to the resource that contains the file and snd being a boolean to determine whether or not it was found.
		if (const auto [fst, snd] = rm.findFile("addons\\configure"); snd)
	{
		// extracts a file from the ResourceFile and the Expected file
		rm.extractSingleFile(fst->first, "addons\\configure");
	}	
	
	// Some servers, like Forsaken FlyFF and the Chinese release  of FlyFF (newFeiFei), will hash the filename strings. 
	// Bruteforcing may be an option but this function will load all the .fast files from Forskane Flyff.
	// It will extract known files to create a list of files to extract. (using their spec_item/mdlobj/etc to extract)
	rm.loadProjectForsaken();

```

# Todo

- Skim through already existing files when using Project Forskan and folders. Supply default resource list as well to extract when extracting.
- Other servers.
	

# Coffee
	¿Can I has coffee?
