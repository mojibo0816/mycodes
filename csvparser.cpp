#include "csvparser.h"


csvparser::csvparser()
{
}


csvparser::~csvparser()
{
}

string csvparser::loadCsvFile(const string &filename)
{
	FILE *pFile = fopen(filename.c_str(),"rb");
	if (pFile == NULL)
	{
		return "";
	}
	fseek(pFile,0,SEEK_END);
	long len = ftell(pFile);
	char  *pBuffer = new char[len + 1];
	fseek(pFile,0,SEEK_SET);
	fread(pBuffer,1,len,pFile);
	fclose(pFile);
	pBuffer[len] = 0;
	string strRead(pBuffer,len);
	delete[]pBuffer;
	return strRead;

}

vector<string> csvparser::splitString(const string&str, const string &separator)
{
	vector<string> resVec;
	if (str == "")
	{
		return resVec;
	}
	string dataStr = str + separator;
	int pos = dataStr.find(separator);
	int size = dataStr.size();
	while (pos != string::npos)
	{
		string tempData = dataStr.substr(0,pos);
		resVec.push_back(tempData);
		dataStr = dataStr.substr(pos+1,size);
		pos = dataStr.find(separator);
	}

	return resVec;
}

vector<vector<string> > csvparser::parseCsvFile(const string &filename, const string &separator)
{
	m_Data.clear();
	string strAllData = csvparser::loadCsvFile(filename);
	if (strAllData.size() == 0)
	{
		return m_Data;
	}
	typedef enum stateType
	{
		NewFieldStart = 0,  //新字段开始
		NonQuotesField,     //非引号字段
		QuotesField,        //引号字段
		FieldSeparator,     //字段分隔
		QuoteInQuotesField, //引号字段中的引号
		RowSeparator,       //行分隔符（回车）
		Error               //语法错误
	}StateType;
	vector<string> vecRows = splitString(strAllData,"\n");
	for (int i = 0; i < vecRows.size(); ++i)
	{
		string strRowData = vecRows.at(i);//一行数据
		if (strRowData.size() == 0)
		{
			continue;
		}
		vector<string> vecFields;
		string strField;
		StateType state = NewFieldStart;
		for (int j = 0; j < strRowData.size(); ++j)
		{
			const char ch = strRowData.at(j);
			switch (state)
			{
			case NewFieldStart:
			{
								  if (ch == '"')
								  {
									  state = QuotesField;
								  }
								  else if (ch == separator.at(0))
								  {
									  vecFields.push_back("");
									  state = FieldSeparator;
								  }
								  else if (ch == '\r' || ch == '\n')
								  {
									  state = RowSeparator;
								  }
								  else
								  {
									  strField.push_back(ch);
									  state = NonQuotesField;
								  }
			}
				break;
			case NonQuotesField:
			{
								   if (ch == separator.at(0))
								   {
									   vecFields.push_back(strField);
									   strField.clear();
									   state = FieldSeparator;
								   }
								   else if (ch == '\r' || ch == '\n')
								   {
									   vecFields.push_back(strField);
									   state = RowSeparator;
								   }
								   else
								   {
									   strField.push_back(ch);
								   }
			}
				break;
			case QuotesField:
			{
								if (ch == '"')
								{
									state = QuoteInQuotesField;
								}
								else
								{
									strField.push_back(ch);
								}
			}
				break;
			case FieldSeparator:
			{
								   if (ch == separator.at(0))
								   {
									   vecFields.push_back("");
								   }
								   else if (ch == '"')
								   {
									   strField.clear();
									   state = QuotesField;
								   }
								   else if (ch == '\r' || ch == '\n')
								   {
									   vecFields.push_back("");
									   state = RowSeparator;
								   }
								   else
								   {
									   strField.push_back(ch);
									   state = NonQuotesField;
								   }
			}
				break;
			case QuoteInQuotesField:
			{
									   if (ch == separator.at(0))
									   {
										   vecFields.push_back(strField);
										   strField.clear();
										   state = FieldSeparator;
									   }
									   else if (ch == '"')
									   {
										   strField.push_back(ch);
										   state = QuotesField;
									   }
									   else
									   {
										   cout << "error" << endl;
									   }
			}
				break;
			case RowSeparator:
			{
								 m_Data.push_back(vecFields);
								 continue;
			}
				break;
			case Error:
			{
			
			}
				break;
			default:
				break;
			}
		}
		
		switch (state)
		{
		case NonQuotesField:
		{
							   vecFields.push_back(strField);
							   m_Data.push_back(vecFields);
		}
			break;
		case QuoteInQuotesField:
		{
								   vecFields.push_back(strField);
								   m_Data.push_back(vecFields);
		}
			break;
		case FieldSeparator:
		{
							   vecFields.push_back(strField);
							   m_Data.push_back(vecFields);
		}
			break;
		case RowSeparator:
		{
							 m_Data.push_back(vecFields);
		}
			break;
		default:
			break;
		}
	}
	return m_Data;
}
void csvparser::printData()
{
	cout << "解析内容如下：" << endl;
	for (int row = 0; row < m_Data.size(); ++row)
	{
		vector<string> rowData = m_Data.at(row);
		for (int col = 0; col < rowData.size(); ++col)
		{
			cout << rowData.at(col) << "\t";
		}
		cout << "\n" << endl;
	}
}