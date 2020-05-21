
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
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
	int wordnum = 10;//单词总数
	string bank = "wordbank.txt";//字典文件名
	WordBank() {//默认构造函数，包含10个单词
		fstream isfile;
		isfile.open(bank, std::ios::in);
		if (!isfile)
		{
			fs::path p{ bank };
			ofstream output{ p };
			output << "序号：1 " << endl << "拼写：fever" << endl << "  释义：n.发烧; 发热; 热(病); 激动不安; 兴奋紧张 v.使发高烧; 使狂热; 煽动" << endl << "典型例句：He has a high fever." << endl;
			output << "序号：2 " << endl << "拼写：grand" << endl << " 释义：adj.壮丽的;堂皇的;重大的;(用于大建筑物等的名称)大;宏大的;宏伟的;有气派的 n.1 000元;1 000英镑" << endl << "典型例句：It's not a very grand house." << endl;
			output << "序号：3 " << endl << "拼写：ounce" << endl << " 释义：n.盎司(重量单位，¹⁄₁₆磅，等于28.35克);少许;少量;一点点;丝毫" << endl << "典型例句：There's not an ounce of truth in her story." << endl;
			output << "序号：4 " << endl << "拼写：ordinary" << endl << " 释义：adj. 普通的;平常的;一般的;平凡的;平庸的;平淡无奇的" << endl << "典型例句：an ordinary sort of day" << endl;
			output << "序号：5 " << endl << "拼写：rural" << endl << " 释义：" << endl << "典型例句：" << endl;
			output << "序号：6 " << endl << "拼写：bow" << endl << " 释义：" << endl << "典型例句：" << endl;
			output << "序号：7 " << endl << "拼写：plug" << endl << " 释义：" << endl << "典型例句：" << endl;
			output << "序号：8 " << endl << "拼写：nut" << endl << " 释义：" << endl << "典型例句：" << endl;
			output << "序号：9 " << endl << "拼写：duck" << endl << " 释义：" << endl << "典型例句：" << endl;
			output << "序号：10 " << endl << "拼写：dull" << endl << " 释义：" << endl << "典型例句：" << endl;
		}
		wordnum = 10;

	}

	//根据单词获取某一行行号
	int getnum(WordBank wb, string word)
	{
		ifstream in(wb.bank);
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

		return line;
	}


	//将字符串数组转为string
	string CharToStr(char* contentChar)
	{
		string tempStr;
		for (int i = 0; contentChar[i] != '\0'; i++)
		{
			tempStr += contentChar[i];
		}
		return tempStr;
	}

	//删除某一行
	void deleteLine(WordBank wb, int lineNum)
	{
		ifstream in;
		string fileName = wb.bank;
		in.open(fileName);

		string strFileData = "";
		int line = 1;
		char lineData[1024] = { 0 };
		while (in.getline(lineData, sizeof(lineData)))
		{
			if (line == lineNum)
			{
				strFileData += "\n";
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
		wb.wordnum--;
	}

	//修改某一行
	void changeLine(WordBank wb, int lineNum, string info)
	{
		ifstream in;
		string fileName = wb.bank;
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
				strFileData += tmpLineData;
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
	void printLines(WordBank wb, int lineNum, int n)
	{
		ifstream in;
		string fileName = wb.bank;
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


	bool add(WordBank wb) //添加
	{
		ofstream file{ wb.bank,std::ios::app };
		string tool;
		string spell, mean, sentence;

		cout << "你将添加第" << wb.wordnum + 1 << "个单词";
		cout << "序号：" << wb.wordnum + 1 << endl << "拼写:"; cin >> spell;
		if (spell == "")
		{
			cout << "拼写不能为空！" << endl;
			return false;
		}
		cout << endl << "释义:"; cin >> mean;
		if (mean == "")
		{
			cout << "释义不能为空！" << endl;
			return false;
		}
		cout << endl << "例句："; cin >> sentence;

		wb.wordnum++;
		file << "序号：" << wb.wordnum << endl << "拼写：" << spell << endl << " 释义：" << mean << endl << "典型例句：" << sentence << endl;
		return true;
	}

	bool dele(WordBank wb) //删除函数
	{
		string word;
		int lineNum = 0;

		cout << "你想删除的单词:";
		cin >> word;

		lineNum = getnum(wb, "拼写：" + word);
		if (lineNum == 0)
		{
			cout << "单词不存在!";
			return false;
		}
		else
		{
			lineNum--;
			for (int i = 0; i < WORDINFOLEN; i++)
			{
				deleteLine(wb, lineNum);
			}
		}

		return true;
	}

	bool change(WordBank wb) //更新函数
	{
		string word, info;
		int lineNum = 0;
		int part;

		cout << "你想修改的单词:";
		cin >> word;
		cin.tie(&cout);

		lineNum = getnum(wb, "拼写：" + word);
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
				changeLine(wb, lineNum, "拼写：" + info);
				break;
			case 1:
				cout << "修改后的内容";
				cin >> info;
				if (info == "")
				{
					cout << "释义不能为空！" << endl;
					return false;
				}
				changeLine(wb, lineNum, "释义：" + info);
				break;
			case 2:
				cout << "修改后的内容";
				cin >> info;
				changeLine(wb, lineNum, "典型例句：" + info);
				break;
			default:
				cout << "选项错误";
				return false;
			}

		}
		return true;
	}

	bool query(WordBank wb) //查询函数
	{
		string word;
		int lineNum = 0;

		cout << "你想查询的单词:";
		cin >> word;

		lineNum = getnum(wb, "拼写：" + word);
		if (lineNum == 0)
		{
			cout << "单词不存在!";
			return false;
		}
		else
		{
			lineNum--;
			printLines(wb, lineNum, WORDINFOLEN);
		}

		return true;
	}

	void browse(WordBank wb) //浏览函数，打印单词表
	{
		int choose, maxLine;
		ifstream normal, reverse;

		char data[1024] = { 0 };

		cout << "正序(0)倒序(1)浏览？";
		cin >> choose;

		switch (choose) {
		case 0:
			normal.open(wb.bank);
			while (normal.getline(data, 1024))
			{
				cout << CharToStr(data) << endl;
			}
			normal.close();
			break;
		case 1:
			for (maxLine = WORDINFOLEN * wb.wordnum + 1; maxLine >= 0; maxLine -= WORDINFOLEN)
			{
				printLines(wb, maxLine, WORDINFOLEN);
			}
			break;
		default:
			cout << "错误!";
			break;
		}

	}

};

void showfunction() {//打印功能表
	cout << "(1) 字典词库至少有10个不同的英语单词，每个单词有拼写、中文释义（n，v，adj，adv等）、和典型例句；(创建时自带10个单词)" << endl
		<< "(2)	添加 – 可向字典词库添加一个单词，单词拼写（必填）、中文释义（必填）、典型例句（一行，可选）；  " << endl
		<< "(3)	删除 – 可删除字典词库中的一个单词；                                                                    " << endl
		<< "(4)	更新 – 可以更新字典词库中一个单词的基本信息	；                                                      " << endl
		<< "(5)	查询 – 输入单词拼写，可以查询单词相关的基本信息；                                                      " << endl
		<< "(6)	浏览 – 可以按照逆序或者升序浏览字典词库中的单词；                                                      " << endl
		<< "(7)	保存 – 字典词库信息更改后能够保存，下次能够读取新的字典词库信息                                        " << endl;


}

int main() {
	WordBank  wb;

	while (1) {//主循环
		showfunction();
		int choose{ 0 };


		cout << "git测试更改-zmc 21/51" << endl;
		cout << "git浏览器模拟他人更改测试-zmc 21/58" << endl;

		while (choose < 2 || choose>7) {
			cout << "请选择你想使用的功能（输入数字）" << endl;
			cin >> choose;
		}
		switch (choose) {

		case 2://添加功能
			if (wb.add(wb)) cout << "添加成功" << endl;
			else cout << "添加失败" << endl;
			break;
		case 3://删除功能
			if (!wb.dele(wb)) cout << "删除失败" << endl;
			break;
		case 4://更新功能
			if (wb.change(wb)) cout << "更新成功" << endl;
			else cout << "更新失败" << endl;
			break;
		case 5://查询功能
			if (!wb.query(wb)) cout << "查询失败" << endl;
			break;
		case 6://浏览功能
			wb.browse(wb);
			break;
		case 7://退出程序
			return 0;
			break;

		}

	}
	return 0;
}

