#include "Precompile.hpp"
#include "VirtualFileExplorer.hpp"

#include "../Debug/Debug.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kToString.hpp>

namespace krakoa::filesystem
{
	namespace fs = std::filesystem;

	fs::path VirtualFileExplorer::root;
	VirtualFileExplorer::PathRedirectsMap VirtualFileExplorer::redirectMap;
	VirtualFileExplorer::DirectoryMap VirtualFileExplorer::folderMap;

	namespace
	{
		auto GetDirectoryImpl(VirtualFolder* vDrive, std::vector<std::string> hierarchy)
		{
			if (vDrive->name == hierarchy[1])
			{
				return vDrive;
			}
			else
			{
				hierarchy.erase(hierarchy.begin(), hierarchy.begin() + 1);

				auto vFolder = vDrive->folders;
				std::shared_ptr<VirtualFolder> current;

				auto iter = hierarchy.begin();
				while (iter != hierarchy.end())
				{
					size_t i = 0;

					for (; i < vFolder.size(); ++i)
					{
						current = vFolder[i];

						if (current->name == *iter)
						{
							break;
						}
					}

					++iter;
					vFolder = vFolder[i]->folders;
				}

				return current.get();
			}
		}

		klib::PathList GetFilesRecursiveImpl(const VirtualFolder& vDirectory)
		{
			klib::PathList items;

			const auto& folders = vDirectory.folders;

			if (folders.empty())
				items.insert(items.end(), vDirectory.files.begin(), vDirectory.files.end());
			else
			{
				for (auto& subFolders : folders)
				{
					const auto subItems = GetFilesRecursiveImpl(*subFolders);
					items.insert(items.end(), subItems.begin(), subItems.end());
				}
			}

			return items;
		}


		void MapDriveImpl(const fs::path& path, VirtualFolder& vFolder)
		{
			const auto end_iter = fs::directory_iterator();

			const auto pathStr = path.string();
			const auto lastPathSeperator = pathStr.find_last_of(klib::pathSeparator<char>);
			const auto currentFolder = pathStr.substr(lastPathSeperator + 1);

			vFolder.name = currentFolder;

			for (auto dir_iter = fs::directory_iterator(path); dir_iter != end_iter; ++dir_iter)
			{
				if (dir_iter->is_directory())
				{
					auto& subFolder = vFolder.folders.emplace_back(Make_Multi<VirtualFolder>());
					MapDriveImpl(dir_iter->path(), *subFolder);
				}
				else
				{
					vFolder.files.emplace_back(dir_iter->path());
				}
			}
		}
	}

	void VirtualFileExplorer::Initialize(std::filesystem::path path)
	{
		KRK_INF("Assigning root path: " + path.string());

		VerifyDirectory(path);

		root = fs::absolute(path);
	}

	void VirtualFileExplorer::MountAbs(const std::filesystem::path& absPath, const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Mounting path \"{0}\" -> \"{1}\"", absPath, vtlPath));

		VerifyDirectory(absPath);

		if (redirectMap.find(vtlPath) != redirectMap.end())
		{
			KRK_ERR(vtlPath + " already exists. Please dismount first if you want to change path");
			return;
		}

		const auto key = klib::ToLower(vtlPath);

		redirectMap[key] = absPath;
		MapVPath(key);
	}

	void VirtualFileExplorer::Mount(const std::filesystem::path& relativePath, const std::string& vtlPath)
	{
		MountAbs(root / relativePath, vtlPath);
	}

	void VirtualFileExplorer::Dismount(const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Dismounting virtual path \"{0}\"", vtlPath));

		const auto iter = redirectMap.find(vtlPath);
		if (iter == redirectMap.end())
			return;
		redirectMap.erase(iter);
		folderMap.erase(vtlPath);
	}

	void VirtualFileExplorer::DismountAll()
	{
		redirectMap.clear();
		folderMap.clear();
	}

	klib::PathList VirtualFileExplorer::GetFiles(const std::string& vtlPath, FileSearchMode searchMode)
	{
		auto hierarchy = klib::Split(vtlPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = folderMap.at(klib::ToLower(key));

		const auto folder = GetDirectoryImpl(&vDirectory, hierarchy);

		if (!folder)
		{
			KRK_WRN("Bad path given: " + vtlPath);
			return {};
		}

		const klib::PathList files =
			searchMode == FileSearchMode::RECURSIVE
			? GetFilesRecursiveImpl(*folder)
			: folder->files;

		return files;
	}

	klib::PathList VirtualFileExplorer::GetFiles(const PathRedirectsMap::key_type& vtlPath,
		const std::string_view& extension, FileSearchMode searchMode)
	{
		auto hierarchy = klib::Split(vtlPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = folderMap.at(klib::ToLower(key));

		const auto folder = GetDirectoryImpl(&vDirectory, hierarchy);

		if (!folder)
		{
			KRK_WRN("Bad path given: " + vtlPath);
			return {};
		}

		klib::PathList files =
			searchMode == FileSearchMode::RECURSIVE
			? GetFilesRecursiveImpl(*folder)
			: folder->files;

		std::erase_if(files, [&](const decltype(files)::value_type& file)
			{
				if (!file.has_extension())
				{
					return extension.empty();
				}
			
				const auto ext = file.extension();

				return klib::Contains(ext.string(), extension);
			});

		return files;
	}

	klib::PathList VirtualFileExplorer::GetDirectories(const std::string& vtlPath, FileSearchMode searchMode)
	{
		auto hierarchy = klib::Split(vtlPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = folderMap.at(klib::ToLower(key));

		const auto folder = GetDirectoryImpl(&vDirectory, hierarchy);

		if (!folder)
		{
			KRK_WRN("Bad path given: " + vtlPath);
			return {};
		}

		klib::PathList paths =
			searchMode == FileSearchMode::RECURSIVE
			? GetFilesRecursiveImpl(*folder)
			: folder->files;

		for (auto& path : paths)
		{
			path = path.parent_path();
		}
		
		return paths;
	}

	klib::Path VirtualFileExplorer::GetRealPath(const PathRedirectsMap::key_type& vtlPath)
	{
		const auto key = klib::ToLower(vtlPath);

		const auto iter = redirectMap.find(key);

		if (iter == redirectMap.end())
			return {};

		return iter->second;
	}

	void VirtualFileExplorer::VerifyDirectory(const std::filesystem::path& path)
	{
		if (!klib::CheckDirectoryExists(path))
			KRK_FATAL("Path does not exist: " + path.string());

		if (klib::CheckFileExists(path))
			KRK_FATAL(klib::ToString("Path given is a file: {0}", path));
	}

	void VirtualFileExplorer::MapVPath(const PathRedirectsMap::key_type& key)
	{
		const auto& path = GetRealPath(key);
		auto& vFolder = folderMap.emplace(key, VirtualFolder()).first->second;
		MapDriveImpl(path, vFolder);
	}

	std::filesystem::path VirtualFileExplorer::GetRoot()
	{
		return root;
	}
}

