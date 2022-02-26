#pragma once
//#include "data.h"
#include <intsafe.h>
#include <shared_mutex>
#include <map>
namespace ffParser
{
	constexpr unsigned long NULL_ID = 0xffffffff;
	namespace text
	{
		constexpr unsigned long MAX_TOKENSTR = 2048;

		enum Toke {
			NONE_, ARG, VAR, IF, ELSE, FOR, DO, WHILE, BREAK, SWITCH, ANSWER, SELECT, YESNO,
			CASE, DEFAULT, GOTO, RETURN, EOL, DEFINE, INCLUDE, ENUM, FINISHED, END
		};
		enum TokenType { TEMP, DELIMITER, IDENTIFIER, NUMBER, HEX, KEYWORD, STRING, BLOCK };
		enum { INTERPRITER, COMPILER };

		class CScanner
		{
			protected:
				bool m_bComma;
				unsigned char m_bMemFlag;
				unsigned long m_dwDef;
				std::string fileInMem;

			public:
				char* m_lpMark; // marked
				char* m_pProg; // current
				char* m_pBuf; // start
				std::string Token;
				char* token;
				
				char m_mszToken[MAX_TOKENSTR];


				int tokenType;
				int tok;
				ULONG_PTR dwValue;
				std::string m_strFileName;

				CScanner();
				explicit CScanner(bool bComma);
				explicit CScanner(void* p, bool bComma = false);
				virtual ~CScanner();

				CScanner(CScanner const&) = delete;
				CScanner& operator =(CScanner const&) = delete;
				CScanner(CScanner&&) = delete;
				CScanner& operator=(CScanner&&) = delete;

				char* getFileMem() { return &fileInMem[0]; }
				[[nodiscard]] unsigned long long getFileMemLength() const { return fileInMem.length(); }

				virtual void SetMark();
				virtual void GoMark();
				virtual int GetLineNum(char* lpProg = nullptr);
				virtual void PutBack();
				virtual	int GetToken(bool bComma = false);
				virtual void Free();
				virtual void GetLastFull();

				//bool Load(const char* lpszFileName, bool bMultiByte = true);
				virtual bool Load(std::string&& inString);
				void SetProg(void* pProg);
				int GetNumber(bool bComma = false);
				long long GetInt64(bool bComma = false);
				float GetFloat(bool bComma = false);
				unsigned long GetHex(bool bComma = false);
				long long GetExpInteger(bool bComma = false);
		};

		class CScript final : public CScanner
		{
			bool process = false;

			protected:
				bool m_bMarkInDefine;
				char* m_pOldProg;

			public:
				static std::map<std::string, unsigned long> m_defines;
				static std::map<std::string, std::string> m_mapString;
				static std::shared_mutex defineListMutex;
				static std::shared_mutex stringListMutex;

				explicit CScript(bool bComma = false);
				~CScript() override;

				CScript(CScript const&) = delete;
				CScript& operator =(CScript const&) = delete;
				CScript(CScript&&) = delete;
				CScript& operator=(CScript&&) = delete;


				void scanDefineFile();
				void scanStringFiles();

				int GetTkn(bool bComma = false);
				int LookUp(const char* s) const;

				static bool LookupDefine(const char* lpszString, unsigned long& rValue);
				static unsigned long GetDefineNum(const char* lpStr);

				void Free() override;
				void SetMark() override;
				void GoMark() override;
				int GetLineNum(char*) override;
				//bool Load(const char* lpszFileName, bool bMultiByte = true, int nProcess = INTERPRITER);
				bool Load(std::string&& inString) override;

				[[nodiscard]] bool IsInterpriterMode() const { return process == INTERPRITER; }
				int GetToken(bool bComma = false) override;
				int GetToken_NoDef(bool bComma = false);
				void GetLastFull() override;
		};
	}
}
namespace ffTextParser = ffParser::text;

