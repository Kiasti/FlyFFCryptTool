#include "ffParser.h"
//#include <comdef.h>
//#include "vutil.h"
//#include "xutil.h"
#include <fstream>
#include <iostream>

#include "helper.h"


unsigned short g_codePage = 0;
inline bool iswhite(const wchar_t c){
	return c > 0 && c <= 0x20;
}

inline bool isdelim(const wchar_t c){
	return (strchr(" !:;,+-<>'/*%^=()&|\"{}", c) || c == 9 || c == '\r' || c == 0 || c == '\n');
}


inline bool isdigit2(const char c)
{
	return c >= '0' && c <= '9';
}

ffTextParser::CScanner::CScanner() : m_bComma(false), m_bMemFlag(true), m_dwDef(0), m_lpMark(nullptr), m_pProg(nullptr), m_pBuf(nullptr), token(m_mszToken), 
	m_mszToken{}, tokenType(0), tok(0),  dwValue(0)
{	
}

ffTextParser::CScanner::CScanner(const bool bComma) : m_bComma(bComma), m_bMemFlag(true), m_dwDef(0), m_lpMark(nullptr), m_pProg(nullptr), m_pBuf(nullptr), 
	token(m_mszToken), m_mszToken{}, tokenType(0), tok(0), dwValue(0)
{
}
ffTextParser::CScanner::CScanner(void* p, const bool bComma) : m_bComma(bComma), m_bMemFlag(true), m_dwDef(0), m_lpMark(nullptr), m_pProg(static_cast<char*>(p)), 
	m_pBuf(static_cast<char*>(p)), token(m_mszToken), m_mszToken{}, tokenType(0), tok(0), dwValue(0)
{
}

ffTextParser::CScanner::~CScanner()
{
	CScanner::Free();
}

void ffTextParser::CScanner::Free()
{
	m_pProg = m_pBuf = nullptr;
}


std::mutex tempMutex;
//bool ffTextParser::CScanner::Load(const char* lpszFileName, bool bMultiByte)
bool ffTextParser::CScanner::Load(std::string&& inString)
{
	//fileInMem = origin::arcLoader::openOf(std::move(origin::arcLoader::getHeaderData(lpszFileName)));
	fileInMem = inString;
	//if (fileInMem.empty())
	//{
	//	{
	//		std::scoped_lock<std::mutex> lck(tempMutex);
	//		std::ofstream oof("ThreadErrorScanner.txt", std::ios::app);
	//		oof << "file not found: " << lpszFileName << "\n";
	//		oof.close();
	//	}
	//	return false;
	//}

	/*if (fileInMem.size() >= 2)
	{
		if (fileInMem[0] == -1 && fileInMem[1] == -2)
		{
			std::string temp;
			temp.reserve(fileInMem.size());

			for (int i = 2; i < fileInMem.size(); ++i)
			{
				if (i % 2 == 0)
					temp += fileInMem[i];
			}

			fileInMem = temp;
			temp.clear();
		}
	}*/

	if (fileInMem.size() >= 2)
	{
		const unsigned int nSize = static_cast<unsigned int>(fileInMem.size() + 2);
		if (static_cast<unsigned char>(fileInMem[0]) == 0xff && static_cast<unsigned char>(fileInMem[1]) == 0xfe)
		{
			char* lpMultiByte = new char[nSize];
			if (const int nResult = helper::WideCharToMultiByteEx(g_codePage, 0, reinterpret_cast<const wchar_t*>(&fileInMem[0] + 2), -1, lpMultiByte,
				static_cast<int>(nSize), nullptr, nullptr); nResult > 0)
			{
				lpMultiByte[nResult - 1] = 0;
				fileInMem.resize(nResult);
				memcpy(&fileInMem[0], lpMultiByte, static_cast<size_t>(nResult));
			}
			delete[](lpMultiByte);
		}
	}

	m_pProg = m_pBuf = fileInMem.data();

	return true;
}

void ffTextParser::CScanner::SetProg(void* pProg)
{
	m_bMemFlag = true;
	m_pProg = m_pBuf = static_cast<char*>(pProg);
}

void ffTextParser::CScanner::SetMark()
{
	m_lpMark = m_pProg;
}

void ffTextParser::CScanner::GoMark()
{
	if (m_lpMark) 
		m_pProg = m_lpMark;
}

int ffTextParser::CScanner::GetLineNum(char* lpProg)
{
	int nLine = 0;
	if (!lpProg) 
		lpProg = m_pProg;

	char* pCur = fileInMem.data();
	while (lpProg != pCur)
	{
		if (*pCur == '\r' || *pCur == '\n') 
			nLine++;
		pCur++;
	}
	return nLine;
}

void ffTextParser::CScanner::GetLastFull()
{
	while (iswhite(*m_pProg) && *m_pProg && (*m_pProg != '\r' && *m_pProg != '\n')) 
		++m_pProg;

	char* pCur = token;
	while (*m_pProg && (*m_pProg != '\r' && *m_pProg != '\n'))
	{
		*pCur = *m_pProg;
		//const size_t count = CopyChar(m_pProg, pCur);
		m_pProg += 1;
		pCur += 1;
	}
	if (pCur != token)
	{
		--pCur;
		while (iswhite(*pCur) && *pCur) 
			--pCur;
		++pCur;
	}
	*pCur = '\0';
	Token = token;
}

int ffTextParser::CScanner::GetToken(const bool bComma)
{
	char* pszCur = m_mszToken; 
	*pszCur = '\0';
	tokenType = TEMP; 
	tok = NONE_;

	bool bLoop;
	do
	{		
		bLoop = false;
		while (iswhite(*m_pProg) && *m_pProg) 
			++m_pProg; 

		while(*m_pProg == '/')
		{
			++m_pProg;
			if(*m_pProg == '/') 
			{ 
				++m_pProg;
				while ((*m_pProg != '\r' && *m_pProg != '\n') && *m_pProg != '\0') 
					++m_pProg;
				if (*m_pProg == '\r' || *m_pProg == '\n')
					++m_pProg;
			}
			else if(*m_pProg == '*') 
			{
				++m_pProg;
				do 
				{
					while (*m_pProg != '*' && *m_pProg != '\0') { ++m_pProg; }
					
					if (*m_pProg == '\0')
					{
						tok = FINISHED;
						tokenType = DELIMITER;
						return tokenType;
					}
					++m_pProg;
				} 
				while(*m_pProg != '/');
				
				++m_pProg;
				while (iswhite(*m_pProg) && *m_pProg) 
					++m_pProg; 
			}
			else 
			{
				--m_pProg;
				break;
			}
			bLoop = true;
		}
	} while (bLoop);

	if(*m_pProg=='\0') 
	{
		tok = FINISHED;
		tokenType = DELIMITER; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	if( bComma || m_bComma )
	{
		if(*m_pProg=='"') 
		{
			++m_pProg;
			while (*m_pProg != '"' && (*m_pProg != '\r' && *m_pProg != '\n') && *m_pProg != '\0')
				*pszCur++ = *m_pProg++; 

			++m_pProg; 
			tokenType = STRING; 
		}
		else
		{
			while (*m_pProg != ',' && (*m_pProg != '\r' && *m_pProg != '\n') && *m_pProg != '\0')
				*pszCur++ = *m_pProg++; 
		}
		if (*m_pProg == ',' || (*m_pProg == '\r' || *m_pProg == '\n')) 
			++m_pProg; 


		if( pszCur != token )
		{
			--pszCur;
			while (iswhite(*pszCur) && *pszCur)
				--pszCur; 
			++pszCur;
			
			if (isdigit2(token[0])) 
				tokenType = NUMBER; 
			if (token[0] == '0' && token[1] == 'x') 
				tokenType = HEX;
		}
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	switch(*m_pProg) 
	{
		case '=':
			if(*(m_pProg+1)=='=') 
			{
				*pszCur++ = *m_pProg++;
				*pszCur++ = *m_pProg++;
			}

			if(*token)
			{
				tokenType = DELIMITER;
				*pszCur = '\0';
				Token = token;
				return tokenType;
			}
			break;
		case '!':
		case '<':
		case '>':
			if(*(m_pProg+1)=='=') 
				*pszCur++ = *m_pProg++;
			*pszCur++ = *m_pProg++;
			
			if(*token)
			{
				tokenType = DELIMITER;
				*pszCur = '\0';
				Token = token;
				return tokenType;
			}
			break;
		default:
			break;
	}
		
	// delimiter
	if(*m_pProg == '&' && *(m_pProg+1) == '&') 
	{
		*pszCur = *(pszCur+1) = '&';
		pszCur+=2; 
		m_pProg+=2; 
		tokenType = DELIMITER; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}
	if(*m_pProg == '|' && *(m_pProg+1) == '|') 
	{
		*pszCur = *(pszCur+1) = '|';
		pszCur+=2; 
		m_pProg+=2; 
		tokenType = DELIMITER; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}
	if( strchr( "+-*^/%=;(),':{}.", *m_pProg ) )  
	{
		*pszCur = *m_pProg; 
		++m_pProg;
		++pszCur;	
		tokenType = DELIMITER; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	if(*m_pProg=='"') 
	{
		++m_pProg;
		while (*m_pProg != '"' && (*m_pProg != '\r' && *m_pProg != '\n') && *m_pProg != '\0' && (pszCur - token) < MAX_TOKENSTR)
		{
			*pszCur = *m_pProg;
			pszCur++;
			m_pProg++;
		}
		++m_pProg; 
		tokenType = STRING; 
		if( *(m_pProg-1) != '"' )
		{
			if( *(m_pProg-1) == '\0' )
				--m_pProg; 
		}
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	if( *m_pProg=='0' && *(m_pProg + 1)=='x' ) 
	{
		m_pProg+=2;
		while(!isdelim(*m_pProg)) 
			*pszCur++ = *m_pProg++;
		tokenType = HEX; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	if( isdigit2( *m_pProg ))
	{
		while( !isdelim( *m_pProg ) )
		{
			*pszCur = *m_pProg;
			m_pProg++;
			pszCur++;
		}
		tokenType = NUMBER; 
		*pszCur = '\0';
		Token = token;
		return tokenType;
	}

	if (iswalpha(*m_pProg) /* || IsMultiByte(m_pProg)*/ || *m_pProg == '#' || *m_pProg == '_' || *m_pProg == '@' || *m_pProg == '$' || *m_pProg == '?')
	{
		while (!isdelim(*m_pProg))
		{
			*pszCur = *m_pProg;
			m_pProg++;
			pszCur++;
		}
	}
	else
		*pszCur++ = *m_pProg++;

	tokenType = TEMP;

	*pszCur = '\0';
	Token = token;
	return tokenType;
}

int ffTextParser::CScanner::GetNumber(const bool bComma )
{
	m_dwDef = 1;
	if (GetToken(bComma) == HEX)
	{
		std::ranges::transform(Token, Token.begin(), [](const unsigned char c) { return std::tolower(c); });
		unsigned long long dwlNumber = 0;
		unsigned long dwMulCnt = 0;
		for (int i = static_cast<int>(Token.length()) - 1; i >= 0; i--)
		{
			char cVal = Token[i];
			if (cVal >= 'a')
				cVal = static_cast<char>(static_cast<int>(cVal) - 'a' + 10);
			else 
				cVal -= '0';

			dwlNumber |= static_cast<unsigned long long>(cVal) << dwMulCnt;
			dwMulCnt += 4;
		}
		m_dwDef = 0;
		return static_cast<int>(dwlNumber);
	}
	
	if (!Token.empty())
	{
		switch (Token[0])
		{
			case '=': 
				m_dwDef = 0;
				return static_cast<int>(NULL_ID);	
			case '-': 
				if (!bComma)
				{
					GetToken();	
					m_dwDef = 0;
					return -static_cast<int>(std::stoul(Token));
				}
				m_dwDef = 0;
				return static_cast<int>(std::stoul(Token));

			case '+':	
				if (!bComma)
					GetToken();
				break;
			default:
				break;
		}
		m_dwDef = 0;

		/** @brief	Using official scanner, they use atoi regardless where this would throw for some files. */
		try  { return static_cast<int>(std::stoul(Token)); }
		catch (const std::exception &e)
		{
			std::cout << Token << std::endl;
			return 0;
		}
	}
	m_dwDef = 0;
	return 0;
}

long long ffTextParser::CScanner::GetInt64(const bool bComma)
{
	m_dwDef = 1;
	if (GetToken(bComma) == HEX)
	{
		
	}
	else if (!Token.empty())
	{
		switch( Token[0] )
		{
			case '=': 
				m_dwDef = 0;
				return NULL_ID;
			case '-': 
				if (!bComma)
				{
					GetToken();	
					m_dwDef = 0;
					return -static_cast<long long>(std::stoull(Token));
				}
				else
				{
					m_dwDef = 0;
					return static_cast<long long>(std::stoull(Token));
				}
			case '+':	
				if(!bComma)
					GetToken();
				break;
			default:
				break;
		}
		m_dwDef = 0;
		return static_cast<long long>(std::stoull(Token));
	}
	m_dwDef = 0;
	return 0;
}

float ffTextParser::CScanner::GetFloat(const bool bComma)
{
	m_dwDef = 1;
	GetToken( bComma );
	if (!Token.empty())
	{
		switch( Token[0] )
		{
			case '=': 
				m_dwDef = 0;
				return -1;
			case '-': 
				GetToken(bComma);	
				m_dwDef = 0;
				return std::stof(Token);
			case '+':	
				GetToken(bComma);
				break;
			default:
				break;
		}
		m_dwDef = 0;
		return std::stof(Token);
	}
	m_dwDef = 0;
	return 0.0f;
}

unsigned long ffTextParser::CScanner::GetHex(const bool bComma)
{
	m_dwDef = 1;
	if (GetToken(bComma) == HEX)
	{
		std::ranges::transform(Token, Token.begin(), [](const unsigned char c) { return std::tolower(c); });
		unsigned long long dwlNumber = 0;
		unsigned long dwMulCnt = 0;
		for (int i = static_cast<int>(Token.length()) - 1; i >= 0; i--)
		{
			char cVal = Token[i];
			if (cVal >= 'a')
				cVal = static_cast<char>(static_cast<int>(cVal) - 'a' + 10);
			else
				cVal -= '0';

			dwlNumber |= static_cast<unsigned long long>(cVal) << dwMulCnt;
			dwMulCnt += 4;
		}
		m_dwDef = 0;
		return static_cast<int>(dwlNumber);
	}
	return 0;
}

long long ffTextParser::CScanner::GetExpInteger(const bool bComma)
{
	return GetInt64(bComma);
}

void ffTextParser::CScanner::PutBack()
{
	for (char *t = m_mszToken; *t; t++)
		m_pProg--; 
	if (tokenType == STRING) 
		m_pProg -= 2; 
}



std::map<std::string, unsigned long> ffTextParser::CScript::m_defines;
std::map<std::string, std::string> ffTextParser::CScript::m_mapString;
std::shared_mutex ffTextParser::CScript::defineListMutex;
std::shared_mutex ffTextParser::CScript::stringListMutex;

ffTextParser::CScript::CScript(const bool bComma) : CScanner(bComma), m_bMarkInDefine(false), m_pOldProg(nullptr)
{
}

ffTextParser::CScript::~CScript()
{
	Free();
}

int ffTextParser::CScript::GetLineNum(char*)
{
	if (!m_pOldProg)
		return CScanner::GetLineNum();
	return CScanner::GetLineNum(m_pOldProg);
}

void ffTextParser::CScript::GetLastFull()
{
	CScanner::GetLastFull();

	if (*token == 0 && m_pOldProg)
	{
		m_pProg = m_pOldProg;
		m_pOldProg = nullptr;
		GetLastFull();
	}
}

#pragma warning (disable : 4706)
int ffTextParser::CScript::GetTkn(const bool bComma)
{
	CScanner::GetToken(bComma);
	if (tokenType == DELIMITER)
	{
		if (*token == '{' || *token == '}')
			tokenType = BLOCK;
	}
	else if (tokenType == TEMP)
	{
		if ((tok = LookUp(token)))
			tokenType = KEYWORD;
		else
			tokenType = IDENTIFIER;
	}
	return tokenType;
}
#pragma warning (default : 4706)


int ffTextParser::CScript::GetToken(const bool bComma)
{
	GetTkn(bComma);
	if (tokenType == IDENTIFIER)
	{
		if (IsInterpriterMode())
		{
			std::map<std::string, std::string>::iterator i;
			bool isDone = false;
			{
				std::shared_lock<std::shared_mutex> lck(stringListMutex);
				i = m_mapString.find(m_mszToken);
				if (i != m_mapString.end())
					isDone = true;
			}
			if (isDone)
			{
				tokenType = STRING;
				strcpy_s(m_mszToken, i->second.c_str());
				Token = m_mszToken;
			}
			else
			{
				if (unsigned long nValue = 0; LookupDefine(token, nValue))
				{
					tokenType = NUMBER;
					sprintf_s(m_mszToken, "%d", nValue);
					Token = m_mszToken;
				}
			}
		}
	}
	else if (tok == FINISHED && m_pOldProg)
	{
		m_pProg = m_pOldProg;
		m_pOldProg = nullptr;
		return GetToken(bComma);
	}
	return tokenType;
}

int ffTextParser::CScript::GetToken_NoDef(const bool bComma)
{
	GetTkn(bComma);
	if (tok == FINISHED && m_pOldProg)
	{
		m_pProg = m_pOldProg;
		m_pOldProg = nullptr;
		return GetTkn(bComma);
	}
	return tokenType;
}

void ffTextParser::CScript::SetMark()
{
	CScanner::SetMark();
	if (m_pOldProg)
		m_bMarkInDefine = true;
	else
		m_bMarkInDefine = false;
}

void ffTextParser::CScript::GoMark()
{
	if (m_bMarkInDefine)
	{
		if (!m_pOldProg)
			m_pOldProg = m_pProg;
	}
	else
	{
		if (m_pOldProg)
			m_pOldProg = nullptr;
	}
	CScanner::GoMark();
}





void ffTextParser::CScript::scanDefineFile()
{
	std::map<std::string, int> tempDefineList;
	int brace = 0;

	char* p = m_pProg;
	GetTkn();
	if (!strcmp(token, "#"))
		process = COMPILER;
	else
	{
		process = INTERPRITER;
		m_pProg = p;
	}

	do
	{
		if (brace)
			return;

		GetTkn();
		if (tok == DEFINE)
		{
			CScanner::GetToken(false);
			std::string str = token;
			CScanner::GetToken(false);

			int n;
			if (tokenType == NUMBER)
			{
				n = atoi(token);
				tempDefineList.insert(make_pair(str, n));
			}
			else if (tokenType == HEX)
			{
				sscanf_s(token, "%x", &n);
				tempDefineList.insert(make_pair(str, n));
			}
			else if (tokenType == 0)
				PutBack();
		}
		else if (tok == ENUM)
		{
			int eNum = 0;
			GetTkn();					// {
			do
			{
				GetTkn();
				GetTkn();				//  , or =
				if (*token == '=')
				{
					GetTkn();			// num
					eNum = atoi(token);
					GetTkn();			// ,
				}
				tempDefineList.insert(std::make_pair(token, eNum));
				eNum++;
			} while (*token != '}');
			GetTkn(); // ;
		}
		else if (tok == INCLUDE)
			GetTkn();
		else if (tok == VAR)
		{
			PutBack();
			GetToken(); // get type 
			do
			{
				GetToken();
				GetToken();
			} while (*token == ',');
		}
		else if (tokenType == IDENTIFIER)
		{
			GetTkn();
			if (*token == '(')
			{
				while (*m_pProg != ')')
					m_pProg++;
				m_pProg++;
			}
			else
				PutBack();
		}
		else if (*token == '{')
			brace++;
	} while (tok != FINISHED);
	m_pProg = p;

	{
		const std::shared_lock lock(defineListMutex);
		m_defines.insert(tempDefineList.begin(), tempDefineList.end());
	}
}


void ffTextParser::CScript::scanStringFiles()
{
	std::map<std::string, std::string> temp;
	CScanner::GetToken();
	while (tok != FINISHED)
	{
		std::string str = Token;
		if (str.find("IDS", 0) != 0)
		{
			GetToken();
			continue;
		}
		GetLastFull();
		temp.insert(std::map<std::string, std::string>::value_type(str, Token));
		CScanner::GetToken();
	}

	{
		const std::shared_lock<std::shared_mutex> lock(stringListMutex);
		m_mapString.insert(temp.begin(), temp.end());
	}
}


int ffTextParser::CScript::LookUp(const char* s) const
{
	if (!process) {
		if (memcmp(s, "#define", 7) == 0)
			return DEFINE;
	}
	return 0;
}



void ffTextParser::CScript::Free()
{
	CScanner::Free();
	m_pOldProg = nullptr;
}

bool ffTextParser::CScript::Load(std::string&& inString)
{
	if (!CScanner::Load(std::move(inString)))
		return false;
	return true;
}


bool ffTextParser::CScript::LookupDefine(const char* lpszString, unsigned long& rValue)
{
	std::shared_lock lck(defineListMutex);
	if (const std::map<std::string, unsigned long >::iterator it = m_defines.find(lpszString); it != m_defines.end())
	{
		rValue = it->second;
		return true;
	}
	return false;
}

unsigned long ffTextParser::CScript::GetDefineNum(const char* lpStr)
{
	unsigned long nValue;
	if (!LookupDefine(lpStr, nValue))
		return 0;
	return nValue;
}
