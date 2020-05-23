#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stack>
#include <cstdio>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::stack;
namespace fs = std::filesystem;

#define WORDINFOLEN 4
/*
(1)	字典词库至少有10个不同的英语单词，每个单词有拼写、词性（n，v，adj，adv等）、中文释义和典型例句；        创建时自带10个
(2)	添加 – 可向字典词库添加一个单词，单词拼写（必填）、词性（必填）、中文释义（必填）、典型例句（可选）；  类中函数
(3)	删除 – 可删除字典词库中的一个单词；                                                                    根据序号或搜索单词得到序号删除
(4)	更新 – 可以更新字典词库中一个单词的基本信息	；                                                      搜索得出序号更新
(5)	查询 – 输入单词拼写，可以查询单词相关的基本信息；                                                      搜索得出序号查询
(6)	浏览 – 可以按照逆序或者升序浏览字典词库中的单词；                                                      顺序或逆序打印单词表
(7)	保存 – 字典词库信息更改后能够保存，下次能够读取新的字典词库信息                                        app方式输出文件
*/

class WordBank {
public:
	int wordNum = 10;//实际记录单词总数
	int indexCount = 0;//单词已使用序号计数
	stack<int> indexStack;//字典空位置索引

	string tempBank = "temp_wordBank.txt";//临时字典文件
	string Bank = "wordBank.txt";//字典储存文件名
	string count = "wordNum.txt";//字典单词总数文件名
	string index = "wordIndex.txt";//字典空位置索引文件名


	WordBank() {//默认构造函数，包含10个单词
		ifstream file;
		string strdata;
		int intdata;

		//读取字典文件，没有就创建一个
		file.open(Bank, std::ios::in);


		if (!file)//创建
		{
			fs::path p{ tempBank };
			ofstream output{ p };
			output << "序号：1 " << endl << "拼写：fever" << endl << "释义：n.发烧; 发热; 热(病); 激动不安; 兴奋紧张 v.使发高烧; 使狂热; 煽动" << endl << "例句：He has a high fever." << endl;
			output << "序号：2 " << endl << "拼写：grand" << endl << "释义：adj.壮丽的;堂皇的;重大的;(用于大建筑物等的名称)大;宏大的;宏伟的;有气派的 n.1 000元;1 000英镑" << endl << "例句：It's not a very grand house." << endl;
			output << "序号：3 " << endl << "拼写：ounce" << endl << "释义：n.盎司(重量单位，¹⁄₁₆磅，等于28.35克);少许;少量;一点点;丝毫" << endl << "例句：There's not an ounce of truth in her story." << endl;
			output << "序号：4 " << endl << "拼写：ordinary" << endl << "释义：adj. 普通的;平常的;一般的;平凡的;平庸的;平淡无奇的" << endl << "例句：an ordinary sort of day" << endl;
			output << "序号：5 " << endl << "拼写：rural" << endl << "释义：" << endl << "例句：" << endl;
			output << "序号：6 " << endl << "拼写：bow" << endl << "释义：" << endl << "例句：" << endl;
			output << "序号：7 " << endl << "拼写：plug" << endl << "释义：" << endl << "例句：" << endl;
			output << "序号：8 " << endl << "拼写：nut" << endl << "释义：" << endl << "例句：" << endl;
			output << "序号：9 " << endl << "拼写：duck" << endl << "释义：" << endl << "例句：" << endl;
			output << "序号：10 " << endl << "拼写：dull" << endl << "释义：" << endl << "例句：" << endl;
			
			file.close();
		}
		else if(file) //提取到临时文件中去
		{
			ifstream file;
			ofstream out;
			char lineData[1024] = { 0 };
			string strFileData = "";

			
			file.open(Bank);					//原文件
			out.open(tempBank, std::ios::out);	//目标文件
			while (file.getline(lineData, sizeof(lineData)))
			{
				strFileData += CharToStr(lineData);
				strFileData += "\n";
			}
			out.flush();
			out << strFileData;
			out.close();
			file.close();
		}
		
		//读取字典单词总个数
		file.open(count, std::ios::in);
		if (!file)
		{
			fs::path p{ count };
			ofstream output{ p };
			output << "10" << endl;

			wordNum = 10;
			file.close();
		}
		else 
		{
			file >> strdata;
			cout << strdata;
			wordNum = std::stoi(strdata);
		}

		//读取空位置索引
		indexStack.push(wordNum + 1);
		file.open(index, std::ios::in);
		if (!file)
		{
			fs::path p{ index };
			ofstream output{ p };
			output << "";

			file.close();
		}
		else
		{
			int line = 1;
			char lineData[10] = { 0 };
			while (file.getline(lineData, sizeof(lineData)))
			{
				intdata = std::stoi(CharToStr(lineData));
				indexStack.push(intdata);
				indexCount++;
			}
		}
		indexCount += wordNum;

	}


	//将字符串数组转为string
	string CharToStr(char* contentChar, int start = 0, int end = 2048)
	{
		string tempStr;
		for (int i = start; contentChar[i] != '\0' && i<end; i++)
		{
			tempStr += contentChar[i];
		}
		return tempStr;
	}


	//根据单词获取某一行行号
	int getnum(string word)
	{
		ifstream in(tempBank);
		char lineData[1024] = { 0 };
		int line = 1;

		while (in.getline(lineData, 1024))
		{
			if (lineData == word)
			{
				return line;
			}
			line++;
		}

		return 0;
	}


	//根据正确的行号获取单词序号id
	int getid(int lineNum)
	{
		ifstream in(tempBank);
		char lineData[1024] = { 0 };
		int line = 1;

		while (in.getline(lineData, 1024))
		{
			if (line == lineNum)
			{
				return std::stoi(CharToStr(lineData, 7));
			}
			
			line++;
		}
	}

	//删除某n行
	void deleteLine(int lineNum,int n)
	{
		ifstream in;
		string fileName = tempBank;
		in.open(fileName);

		string strFileData = "";
		int line = 1, i = 0;
		char lineData[1024] = { 0 };
		while (in.getline(lineData, sizeof(lineData)))
		{
			if (line == lineNum || i>0)
			{
				i++;

				if (i >= n)
				{
					i = 0;
				}//删完了
			}
			else
			{
				strFileData += CharToStr(lineData);
				strFileData += "\n";
			}
			line++;
		}
		in.close();

		//写入文件
		ofstream out;
		out.open(fileName);
		out.flush();
		out << strFileData;
		out.close();
		
	}


	//修改某一行
	void changeLine(int lineNum, string info)
	{
		ifstream in;
		string fileName = tempBank;
		in.open(fileName);

		string strFileData = "";
		int line = 1;
		char tmpLineData[1024] = { 0 };


		while (in.getline(tmpLineData, sizeof(tmpLineData)))
		{
			if (line == lineNum)
			{
				strFileData += info;
				strFileData += "\n";
			}
			else
			{
				strFileData += CharToStr(tmpLineData);
				strFileData += "\n";
			}
			line++;
		}
		in.close();

		//写入文件
		ofstream out;
		out.open(fileName);
		out.flush();
		out << strFileData;
		out.close();
	}


	//打印某n行
	void printLines(int lineNum, int n)
	{
		ifstream in;
		string fileName = tempBank;
		char data[1024] = { 0 };
		in.open(fileName);

		int line = 1, i = 0;
		while (in.getline(data, 1024))
		{
			if (lineNum == line || i > 0)
			{
				if (i >= n)
				{
					cout << endl;
					break;
				}
				cout << CharToStr(data) << endl;
				i++;

			}
			line++;
		}
	}


	bool add() //添加
	{
		ofstream file{ tempBank,std::ios::app };
		string tool;
		string spell, mean, sentence;

		cout << "你将添加到序号" << indexStack.top() << "位的单词" << endl;
		
		cout << "拼写:"; cin >> spell;
		if (spell == "")
		{
			cout << "拼写不能为空！" << endl;
			return false;
		}
		if (getnum("拼写：" + spell) != 0)
		{
			cout << "该单词已存在";
			return false;
		}
		
		cout << endl << "释义:"; cin >> mean;
		if (mean == "")
		{
			cout << "释义不能为空！" << endl;
			return false;
		}
		
		cout << endl << "例句："; cin >> sentence;

		//两个记录性质变量的更改
		wordNum++;
		indexStack.pop();
		indexStack.push(wordNum);


		file << "序号：" << wordNum << endl << "拼写：" << spell << endl << "释义：" << mean << endl << "例句：" << sentence << endl;
		return true;
	}


	bool dele() //删除函数
	{
		string word;
		int lineNum;

		cout << "你想删除的单词:";
		cin >> word;

		lineNum = getnum("拼写：" + word);
		if (lineNum == 0)
		{
			cout << "单词不存在!";
			return false;
		}
		else
		{
			lineNum--;
			indexStack.push(lineNum);//空缺索引,压栈
			deleteLine( lineNum,WORDINFOLEN);
		}

		wordNum--;

		return true;
	}


	bool change() //更新函数
	{
		string word, info;
		int lineNum = 0;
		int part;

		cout << "你想修改的单词:";
		cin >> word;
		cin.tie(&cout);

		lineNum = getnum( "拼写：" + word);
		if (lineNum == 0)
		{
			cout << "单词不存在!";
			return false;
		}
		else
		{
			cout << "你想修改哪一个部分？(0.单词拼写 1.中文释义 2.典型例句)";
			cin >> part;
			switch (part) {
			case 0:
				cout << "修改后的内容";
				cin >> info;
				if (info == "")
				{
					cout << "拼写不能为空！" << endl;
					return false;
				}
				changeLine( lineNum, "拼写：" + info);
				break;
			case 1:
				cout << "修改后的内容";
				cin >> info;
				if (info == "")
				{
					cout << "释义不能为空！" << endl;
					return false;
				}
				changeLine( lineNum+1, "释义：" + info);
				break;
			case 2:
				cout << "修改后的内容";
				cin >> info;
				changeLine( lineNum+2, "例句：" + info);
				break;
			default:
				cout << "选项错误";
				return false;
			}

		}
		return true;
	}


	bool query() //查询函数
	{
		string word;
		int lineNum = 0;

		cout << "你想查询的单词:";
		cin >> word;

		lineNum = getnum( "拼写：" + word);
		if (lineNum == 0)
		{
			cout << "单词不存在!";
			return false;
		}
		else
		{
			lineNum--;
			printLines( lineNum, WORDINFOLEN);
		}

		return true;
	}


	void browse() //浏览函数，打印单词表
	{
		int choose, maxLine;
		ifstream normal, reverse;

		char data[1024] = { 0 };

		cout << "正序(0)倒序(1)浏览？";
		cin >> choose;

		switch (choose) {
		case 0:
			normal.open(tempBank);
			while (normal.getline(data, 1024))
			{
				cout << CharToStr(data) << endl;
			}
			normal.close();
			break;
		case 1:
			for (maxLine = WORDINFOLEN * wordNum + 1; maxLine >= 0; maxLine -= WORDINFOLEN)
			{
				printLines( maxLine, WORDINFOLEN);
			}
			break;
		default:
			cout << "错误!";
			break;
		}

	}


	void save()// 保存并退出  // 主要保存数目和索引
	{
		ofstream numf;
		ofstream indexf;
		int save_index;

		//总数目文件
		numf.open(count, std::ios::out);
		numf << std::to_string(wordNum);
		numf.close();

		//索引文件
		indexf.open(index, std::ios::out);
		while (!indexStack.empty())
		{
			save_index = indexStack.top();
			indexf << std::to_string(save_index) << endl;
			indexStack.pop();
		}
		indexf.close();

		//临时字典文件固定化
		ifstream in;
		ofstream out;
		char lineData[1024] = { 0 };
		string strFileData = "";

		
		in.open(tempBank);					//原文件
		out.open(Bank, std::ios::out);	//目标文件
		while (in.getline(lineData, sizeof(lineData)))
		{
			strFileData += CharToStr(lineData);
			strFileData += "\n";
		}
		out.flush();
		out << strFileData;
		out.close();
		in.close();

		std::remove(tempBank.c_str());

		cout << "保存成功";

	}

};


void showfunction() {//打印功能表
	cout << "(1) 字典词库至少有10个不同的英语单词，每个单词有拼写、中文释义（n，v，adj，adv等）、和典型例句；(自带10个单词)" << endl
		<< "(2)	添加 – 可向字典词库添加一个单词，单词拼写（必填）、中文释义（必填）、典型例句（一行，可选）；              " << endl
		<< "(3)	删除 – 可删除字典词库中的一个单词；                                                                   " << endl
		<< "(4)	更新 – 可以更新字典词库中一个单词的基本信息	；                                                         " << endl
		<< "(5)	查询 – 输入单词拼写，可以查询单词相关的基本信息；                                                       " << endl
		<< "(6)	浏览 – 可以按照逆序或者升序浏览字典词库中的单词；                                                       " << endl
		<< "(7)	保存 – 字典词库信息更改后能够保存，下次能够读取新的字典词库信息                                           " << endl
		<< "   ====================! ! ! ! ! ！务 必 使 用 保 存 功 能 退 出 程 序！！！！！！！====================== ";


}


int main() {
	WordBank  wb;

	while (1) {//主循环
		showfunction();
		int choose{ 0 };

		cout << "测试验证"<< endl;

		while (choose < 2 || choose>7) {
			cout << "请选择你想使用的功能（输入数字）" << endl;
			cin >> choose;
		}
		switch (choose) {

		case 2://添加功能
			if (wb.add()) cout << "添加成功" << endl;
			else cout << "添加失败" << endl;
			break;
		case 3://删除功能
			if (!wb.dele()) cout << "删除失败" << endl;
			break;
		case 4://更新功能
			if (wb.change()) cout << "更新成功" << endl;
			else cout << "更新失败" << endl;
			break;
		case 5://查询功能
			if (!wb.query()) cout << "查询失败" << endl;
			break;
		case 6://浏览功能
			wb.browse();
			break;
		case 7://退出程序
			wb.save();
			return 0;
			break;

		}

	}

	return 0;
}