/**
* @file File.h
* @brief ファイルの読み書きなど
* @author shiihara_kiyosumi
* @date 2022_08_13
*/

// ヘッダーファイルのインクルード
#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

// using宣言
using std::ifstream;
using std::ofstream;
using std::endl; using std::fixed; using std::setprecision;

/**
* @class File
* @brief ファイル読み書き関連の静的クラス
*/
class File
{
public:
	struct FileData
	{
		FileData()
		{
			posX = 0.0f;
			posY = 0.0f;
			scaleX = 0.0f;
			scaleY = 0.0f;
			rotateZ = 0.0f;
		}

		FileData(float posX,float posY,float scaleX,float scaleY,float rotateZ)
		{
			this->posX = posX;
			this->posY = posY;
			this->scaleX = scaleX;
			this->scaleY = scaleY;
			this->rotateZ = rotateZ;
		}

		float posX;
		float posY;
		float scaleX;
		float scaleY;
		float rotateZ;
	};

	static const FileData LoadFile(const char* fileName)
	{
		FileData data;
		std::ifstream in;
		in.open(fileName);
		data.posX = CutLine(&in, ':');
		data.posY = CutLine(&in, ':');
		data.scaleX = CutLine(&in, ':');
		data.scaleY = CutLine(&in, ':');
		data.rotateZ = CutLine(&in, ':');
		in.close();
		return data;
	}

	static void OutputFile(const char* fileName, const FileData& data)
	{
		ofstream out;
		out.open(fileName);
		out << fixed << setprecision(2);
		out << "posX:" << data.posX << endl;
		out << "posY:" << data.posY << endl;
		out << "scaleX:" << data.scaleX << endl;
		out << "scaleY:" << data.scaleY << endl;
		out << "rotateZ:" << data.rotateZ << endl;
		out.close();
	}



private:
	static float CutLine(ifstream* in, char id)
	{
		std::string line;
		std::getline(*in, line);
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == id)
			{
				return std::stof(line.substr(i + 1));
			}
		}
	}
};
