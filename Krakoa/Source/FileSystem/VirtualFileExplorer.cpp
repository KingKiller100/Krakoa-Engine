﻿#include "Precompile.hpp"
#include "VirtualFileExplorer.hpp"

#include "../Debug/Debug.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kToString.hpp>

namespace krakoa::filesystem
{
	namespace fs = std::filesystem;

	fs::path VirtualFileExplorer::rootDirectory_;
	VirtualFileExplorer::PathRedirectsMap VirtualFileExplorer::directoryRedirectMap_;
	VirtualFileExplorer::DirectoryMap VirtualFileExplorer::directoryMap_;

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

					vFolder = vFolder[i]->folders;
					++iter;
				}

				return current.get();
			}
		}

		klib::PathList GetFilesRecursiveImpl(const VirtualFolder& vDirectory)
		{
			const auto& files = vDirectory.files;
			const auto& folders = vDirectory.folders;

			klib::PathList items;

			if (folders.empty())
				items.insert(items.end(), vDirectory.files.begin(), vDirectory.files.end());
			else
			{
				for (auto& subFolders : folders)
				{
					const auto subItems = GetFilesRecursiveImpl(*subFolders);
					items.insert(items.end(), subItems.begin(), subItems.end());
					items.insert(items.end(), files.begin(), files.end());
				}
			}

			return items;
		}


		void MapDriveImpl(const fs::path& path, VirtualFolder& vFolder)
		{
			const auto end_iter = fs::directory_iterator();

			const auto pathStr = path.string();
			const auto lastPathSeperator = pathStr.find_last_of(klib::PathSeparator<char>);
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
		const auto absPath = fs::absolute(path);
		KRK_INF("Assigning root path: " + absPath.string());

		VerifyDirectory(path);

		rootDirectory_ = absPath;
	}

	void VirtualFileExplorer::MountAbs(const std::filesystem::path& absPath, const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Mounting path \"{0}\" -> \"{1}\"", absPath, vtlPath));

		VerifyDirectory(absPath);

		if (directoryRedirectMap_.find(vtlPath) != directoryRedirectMap_.end())
		{
			KRK_ERR(vtlPath + " already exists. Please dismount first if you want to change path");
			return;
		}

		const auto key = klib::ToLower(vtlPath);

		directoryRedirectMap_[key] = absPath;
		MapVPath(key);
	}

	void VirtualFileExplorer::Mount(const std::filesystem::path& relativePath, const std::string& vtlPath)
	{
		MountAbs(rootDirectory_ / relativePath, vtlPath);
	}

	void VirtualFileExplorer::Dismount(const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Dismounting virtual path \"{0}\"", vtlPath));

		const auto iter = directoryRedirectMap_.find(vtlPath);
		if (iter == directoryRedirectMap_.end())
			return;
		directoryRedirectMap_.erase(iter);
		directoryMap_.erase(vtlPath);
	}

	void VirtualFileExplorer::DismountAll()
	{
		directoryRedirectMap_.clear();
		directoryMap_.clear();
	}

	klib::PathList VirtualFileExplorer::GetFiles(const std::string& vtlPath, FileSearchMode searchMode)
	{
		const auto correctPath = CorrectPath(vtlPath);
		auto hierarchy = klib::Split(correctPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = directoryMap_.at(klib::ToLower(key));

		const auto folder = hierarchy.size() >= 2
			? GetDirectoryImpl(&vDirectory, hierarchy)
			: &vDirectory;

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
		std::string_view extension, FileSearchMode searchMode)
	{
		const auto correctPath = CorrectPath(vtlPath);
		auto hierarchy = klib::Split(correctPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = directoryMap_.at(klib::ToLower(key));
		
		const auto folder = hierarchy.size() >= 2
			? GetDirectoryImpl(&vDirectory, hierarchy)
			: &vDirectory;

		if (!folder)
		{
			KRK_WRN("Bad path given: " + vtlPath);
			return {};
		}

		klib::PathList files =
			searchMode == FileSearchMode::RECURSIVE
			? GetFilesRecursiveImpl(*folder)
			: folder->files;


		auto toRemove = std::remove_if( files.begin(), files.end(), [&](const decltype(files)::value_type& file)
			{
				if (!file.has_extension())
				{
					return extension.empty();
				}
			
				const auto ext = file.extension();

				return !klib::Contains(ext.string(), extension);
			});
		files.erase(toRemove, files.end());

		return files;
	}

	klib::PathList VirtualFileExplorer::GetDirectories(const std::string& vtlPath, FileSearchMode searchMode)
	{
		const auto correctPath = CorrectPath(vtlPath);
		auto hierarchy = klib::Split(correctPath, "\\");
		const auto& key = hierarchy.front();

		auto vDirectory = directoryMap_.at(klib::ToLower(key));

		const auto folder = hierarchy.size() >= 2
			? GetDirectoryImpl(&vDirectory, hierarchy)
			: &vDirectory;

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

		const auto iter = directoryRedirectMap_.find(key);

		if (iter == directoryRedirectMap_.end())
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

	std::string VirtualFileExplorer::CorrectPath(const std::string& path)
	{
		return klib::Replace(path, '/', '\\');
	}

	void VirtualFileExplorer::MapVPath(const PathRedirectsMap::key_type& key)
	{
		const auto& path = GetRealPath(key);
		auto& vFolder = directoryMap_.emplace(key, VirtualFolder()).first->second;
		MapDriveImpl(path, vFolder);
	}

	std::filesystem::path VirtualFileExplorer::GetRoot()
	{
		return rootDirectory_;
	}
}

