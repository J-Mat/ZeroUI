#pragma once
#include <crossguid/guid.hpp>
#include "Math/ZMath.h"
#include "stdio.h"

namespace ZeroUI
{

#ifdef _WIN32
#include <direct.h>		//for mkdir rmdir
#include <io.h>			//for access
#elif __linux__
#include <unistd.h>		//for mkdir rmdir
#include <sys/stat.h>	//for access
#include <dirent.h>		//for DIR remove
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))
#elif __linux__
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#define RMDIR(a) rmdir((a))
#endif

	namespace Utils
	{
		using namespace xg;

		static inline std::string WString2String(const std::wstring& Input)
		{
			_bstr_t t = Input.c_str();
			char* pchar = (char*)t;
			std::string result = pchar;
			return result;

		}

		static inline std::wstring String2WString(const std::string& Input)
		{
			_bstr_t t = Input.c_str();
			wchar_t* pwchar = (wchar_t*)t;
			std::wstring result = pwchar;
			return result;
		}

		static LPCWSTR StringToLPCWSTR(const std::string& Input)
		{
			size_t origsize = Input.length() + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (Input.length() - 1));
			mbstowcs_s(&convertedChars, wcstring, origsize, Input.c_str(), _TRUNCATE);

			return wcstring;
		}
		
		static std::string GetShaderPath(const std::string& FileName)
		{
			return  FileName;
		}

		static bool IsParentFolder(const std::filesystem::path& Root, const std::filesystem::path& Child) 
		{
			if (Child.empty() || Child.root_path() == Child)
				return false;
			else 
			{
				const auto Parent = Child.parent_path();
				if (Parent == Root)
					return true;
				return IsParentFolder(Root, Parent);
			}
		}
		

		static void NameShrink(std::string& Name, size_t MaxLength) {
			if (Name.size() <= MaxLength)
				return;
			Name.resize(MaxLength);
			Name[MaxLength - 2] = '.';
			Name[MaxLength - 1] = '.';
		}

		static bool RemoveDir(const std::string& path)
		{
			std::string strPath = path;
#ifdef _WIN32
			struct _finddata_t fb;   //������ͬ�����ļ��Ĵ洢�ṹ��
			//������������·��
			if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
				strPath.append("\\");
			std::string findPath = strPath + "*";
			intptr_t handle;//��long���ͻᱨ��
			handle = _findfirst(findPath.c_str(), &fb);
			//�ҵ���һ��ƥ����ļ�
			if (handle != -1L)
			{
				std::string pathTemp;
				do//ѭ���ҵ����ļ� 
				{
					//ϵͳ�и�ϵͳ�ļ�����Ϊ��..���͡�.��,������������  
					if (strcmp(fb.name, "..") != 0 && strcmp(fb.name, ".") != 0)//��ϵͳ�����ļ��Ĵ������
					{
						//��������·��
						pathTemp.clear();
						pathTemp = strPath + std::string(fb.name);
						//����ֵΪ16����˵�����ļ��У�����  
						if (fb.attrib == _A_SUBDIR)//_A_SUBDIR=16
						{
							RemoveDir(pathTemp.c_str());
						}
						//���ļ��е��ļ���ֱ��ɾ�������ļ�����ֵ�����û����ϸ���飬���ܻ������������  
						else
						{
							remove(pathTemp.c_str());
						}
					}
				} while (0 == _findnext(handle, &fb));//�жϷ�ǰ���ʧȥ��һ�������Ľ��
				//�ر��ļ��У�ֻ�йر��˲���ɾ����������������˺ܾã���׼c���õ���closedir  
				//������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ�����  
				_findclose(handle);
			}
			//�Ƴ��ļ���  
			return RMDIR(strPath.c_str()) == 0 ? true : false;

#elif __linux__
			if (strPath.at(strPath.length() - 1) != '\\' || strPath.at(strPath.length() - 1) != '/')
				strPath.append("/");
			DIR* d = opendir(strPath.c_str());//�����Ŀ¼
			if (d != NULL)
			{
				struct dirent* dt = NULL;
				while (dt = readdir(d))//�����ȡĿ¼�е��ļ���dt
				{
					//ϵͳ�и�ϵͳ�ļ�����Ϊ��..���͡�.��,������������
					if (strcmp(dt->d_name, "..") != 0 && strcmp(dt->d_name, ".") != 0)//�ж��Ƿ�Ϊϵͳ�����ļ�
					{
						struct stat st;//�ļ�����Ϣ
						std::string fileName;//�ļ����е��ļ���
						fileName = strPath + std::string(dt->d_name);
						stat(fileName.c_str(), &st);
						if (S_ISDIR(st.st_mode))
						{
							RmDir(fileName);
						}
						else
						{
							remove(fileName.c_str());
						}
					}
				}
				closedir(d);
			}
			return rmdir(strPath.c_str()) == 0 ? true : false;
#endif
		}


		static bool RemoveFilesInDir(const std::string& path)
		{
			RemoveDir(path);
			return MKDIR(path.c_str()) == 0  ? true : false;
		}

		static void CreateFolerIfNotExits(const std::filesystem::path& Folder)
		{
			if (!std::filesystem::exists(Folder))
			{
				MKDIR(Folder.string().c_str());
			}
		}

		static void RemoveOtherFilesInDir(const std::filesystem::path& Folder, std::set<std::filesystem::path>& RemainFiles)
		{
			for (auto& Child : std::filesystem::directory_iterator(Folder))
			{
				if (std::filesystem::is_directory(Child))
				{
					RemoveOtherFilesInDir(Child.path(), RemainFiles);
				}
				else if (!RemainFiles.contains(Child))
				{
					remove(Child.path().filename().string().c_str());
				}
			}
		}
	};
}