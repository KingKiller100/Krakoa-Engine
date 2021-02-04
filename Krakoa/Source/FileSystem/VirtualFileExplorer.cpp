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

	namespace
	{
		template<typename InsertionFunc>
		klib::PathList GetImpl(const fs::path& path, InsertionFunc insertionClause)
		{
			klib::PathList items;

			const auto end_iter = fs::directory_iterator();

			for (auto dir_iter = fs::directory_iterator(path); dir_iter != end_iter; ++dir_iter)
			{
				if (insertionClause(dir_iter))
					items.emplace_back(dir_iter->path());
			}

			return items;
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
		
		redirectMap[vtlPath] = absPath;
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
	}

	klib::Path VirtualFileExplorer::GetRealPath(const PathRedirectsMap::key_type& vtlPath)
	{
		const auto iter = redirectMap.find(vtlPath);

		if (iter == redirectMap.end())
			return {};

		return iter->second;
	}

	klib::PathList VirtualFileExplorer::GetFiles(const PathRedirectsMap::key_type& vtlPath)
	{
		const auto path = GetRealPath(vtlPath);

		if (path.empty())
			return {};

		return GetImpl(path, [](const fs::directory_iterator& dir_iter)
			{
				return dir_iter->is_regular_file();
			});
	}

	klib::PathList VirtualFileExplorer::GetFiles(const PathRedirectsMap::key_type& vtlPath,
		const std::string_view& extension)
	{
		const auto path = GetRealPath(vtlPath);

		if (path.empty())
			return {};

		return GetImpl(path, [&](const fs::directory_iterator& dir_iter)
			{
				const auto& currentPath = dir_iter->path();
			
				if (!currentPath.has_extension())
					return false;

				const auto ext = currentPath.extension();
			
				return dir_iter->is_regular_file() && ext == extension;
			});
	}

	klib::PathList VirtualFileExplorer::GetDirectories(const PathRedirectsMap::key_type& vtlPath)
	{
		const auto path = GetRealPath(vtlPath);

		if (path.empty())
			return {};

		return GetImpl(path, [](const fs::directory_iterator& dir_iter)
			{
				return dir_iter->is_directory();
			});
	}

	void VirtualFileExplorer::VerifyDirectory(const std::filesystem::path& path)
	{
		if (!klib::CheckDirectoryExists(path))
			KRK_FATAL("Path does not exist: " + path.string());

		if (klib::CheckFileExists(path))
			KRK_FATAL(klib::ToString("Path given is a file: {0}", path));
	}

	std::filesystem::path VirtualFileExplorer::GetRoot()
	{
		return root;
	}
}

