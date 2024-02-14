#include <exi2xml/XmlWriter.h>

extern "C" {
#include <EXIParser.h>
}

#include <string>
#include <iterator>
#include <exception>
#include <stack>
#include <sstream>
#include <map>


using namespace std;

struct appData
{
	appData()
		: expectAttributeData(false)
	{}

	bool expectAttributeData;
	std::stack<std::string> tagName;
	std::stringstream strm;
};


// Content Handler API
static errorCode sample_fatalError(const errorCode code, const char* msg, void* app_data);
static errorCode sample_startDocument(void* app_data);
static errorCode sample_endDocument(void* app_data);
static errorCode sample_startElement(QName qname, void* app_data);
static errorCode sample_endElement(void* app_data);
static errorCode sample_attribute(QName qname, void* app_data);
static errorCode sample_stringData(const String value, void* app_data);
static errorCode sample_qnameData(const QName qname, void* app_data);

namespace
{
	errorCode decodeImpl(char* buffer, size_t len, appData* parsingData)
	{
		Parser exiParser;
		errorCode tmp_err_code = EXIP_UNEXPECTED_ERROR;
		BinaryBuffer binBuf;
		EXIOptions opts;
		makeDefaultOpts(&opts);

		binBuf.buf = buffer;
		binBuf.bufLen = len;
		binBuf.bufContent = len;
		binBuf.ioStrm.readWriteToStream = NULL;
		binBuf.ioStrm.stream = NULL;

		TRY(parse.initParser(&exiParser, binBuf, parsingData));

		exiParser.strm.header.opts = opts;
		TRY(parse.parseHeader(&exiParser, TRUE));

		exiParser.handler.fatalError = sample_fatalError;
		exiParser.handler.error = sample_fatalError;
		exiParser.handler.startDocument = sample_startDocument;
		exiParser.handler.endDocument = sample_endDocument;
		exiParser.handler.startElement = sample_startElement;
		exiParser.handler.attribute = sample_attribute;
		exiParser.handler.stringData = sample_stringData;
		exiParser.handler.endElement = sample_endElement;
		exiParser.handler.qnameData = sample_qnameData;

		TRY(parse.setSchema(&exiParser, NULL));

		parsingData->strm << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;

		while (tmp_err_code == EXIP_OK)
		{
			tmp_err_code = parse.parseNext(&exiParser);
		}

		parse.destroyParser(&exiParser);

		if (tmp_err_code == EXIP_PARSING_COMPLETE)
			return EXIP_OK;
		else
			return tmp_err_code;
	}

	std::string ToString(const String* str)
	{
		std::string newStr;

		std::copy(str->str, str->str + str->length, std::back_inserter(newStr));

		return newStr;
	}

	std::map<std::string, std::string> nsMap;
	int nsCount = 1;
}

namespace ThetaStream
{
	XmlWriter::XmlWriter(std::ostream& file)
		:_ofile(file)
	{
	}

	XmlWriter::~XmlWriter()
	{
	}

	int XmlWriter::decode(char* buffer, unsigned int len)
	{
		struct appData parsingData;
		errorCode ret = decodeImpl(buffer, len, &parsingData);

		if (ret == EXIP_OK)
		{
			_ofile << parsingData.strm.str();
		}
		else
		{
			std::string msg("\nFailed to parse EXI file.  Error Code = ");
			msg += GET_ERR_STRING(ret);
			_ofile << msg;
			return 1;
		}

		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////

errorCode sample_fatalError(const errorCode code, const char* msg, void* app_data)
{
	return EXIP_OK;
}

errorCode sample_startDocument(void* app_data)
{
	struct appData* appD = (struct appData*)app_data;
	return EXIP_OK;
}

errorCode sample_endDocument(void* app_data)
{
	nsMap.clear();
	nsCount = 1;
	return EXIP_OK;
}

errorCode sample_startElement(QName qname, void* app_data)
{
	struct appData* appD = (struct appData*)app_data;
	std::string schema = ToString(qname.uri);
	std::string nsLabel("");

	if (appD->tagName.size() == 0)
	{
		std::pair < std::string, std::string> ns;
		ns.first = schema;
		ns.second = "ns" + std::to_string(nsCount++);
		nsMap.insert(ns);
		nsLabel = " xmlns:" + ns.second + "=\"" + schema + "\"";
	}

	std::string elementName = ToString(qname.localName);
	auto it = nsMap.find(schema);
	if (it != nsMap.end())
	{
		elementName = it->second + ":" + elementName;
	}
	appD->tagName.push(elementName);

	string s = appD->strm.str();
	if (s[s.length() - 2] != '>')
	{
		appD->strm << ">" << endl;
	}

	string indent;
	for (int i = 0; i < appD->tagName.size() - 1; i++)
	{
		indent += "\t";
	}

	appD->strm << indent <<  "<" << appD->tagName.top() << nsLabel;
	return EXIP_OK;
}

errorCode sample_endElement(void* app_data)
{
	struct appData* appD = (struct appData*)app_data;

	string strm = appD->strm.str();
	if (strm[strm.length() - 2] == '>')
	{
		string indent;
		for (int i = 0; i < appD->tagName.size() - 1; i++)
		{
			indent += "\t";
		}
		appD->strm << indent;
	}

	appD->strm << "</" << appD->tagName.top() << ">" << endl;

	appD->tagName.pop();
	return EXIP_OK;
}

errorCode sample_attribute(QName qname, void* app_data)
{
	struct appData* appD = (struct appData*)app_data;
	appD->tagName.push(ToString(qname.localName));
	appD->expectAttributeData = true;
	return EXIP_OK;
}

errorCode sample_stringData(const String value, void* app_data)
{
	struct appData* appD = (struct appData*)app_data;

	if (appD->expectAttributeData)
	{
		appD->strm << " " << appD->tagName.top() << "=\"" << ToString(&value) << "\"";
		appD->tagName.pop();
		appD->expectAttributeData = false;
	}
	else
	{
		appD->strm << ">" << ToString(&value);
	}

	return EXIP_OK;
}

errorCode sample_qnameData(const QName qname, void* app_data)
{
	return EXIP_OK;
}
