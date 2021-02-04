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

	void VirtualFileExplorer::Initialize(const std::filesystem::path& rootPath)
	{
		KRK_INF("Assigning root path: " + root.string());
		
		if (!klib::CheckDirectoryExists(rootPath))
			KRK_FATAL("Root path does not exist: " + rootPath.string());
		
		if (klib::CheckFileExists(rootPath))
			KRK_FATAL(klib::ToString("Root path given is a file: {0}", rootPath));

		root = std::move(rootPath);
	}

	void VirtualFileExplorer::Mount(const std::filesystem::path& physicalPath, const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Mounting path \"{0}\" -> \"{1}\"", physicalPath, vtlPath));

		if (!klib::CheckDirectoryExists(physicalPath))
			KRK_FATAL("Physical path does not exist: " + physicalPath.string());
		
		if (klib::CheckFileExists(physicalPath))
			KRK_FATAL("Path is a file: " + physicalPath.string());
		
		redirectMap[vtlPath].emplace_back(physicalPath);
	}

	void VirtualFileExplorer::Dismount(const std::string& vtlPath)
	{
		KRK_INF(klib::ToString("Dismounting virtual path \"{0}\"", vtlPath));

		const auto iter = redirectMap.find(vtlPath);
		if (iter == redirectMap.end())
			return;
		redirectMap.erase(iter);
	}

	bool VirtualFileExplorer::PhysicalPathExists(const std::filesystem::path& physicalPath)
	{
		return klib::CheckDirectoryExists(physicalPath);
	}

	bool VirtualFileExplorer::PathExists(const std::filesystem::path& vtlPath)
	{
		return redirectMap.find(vtlPath.string()) != redirectMap.end();
	}

	bool VirtualFileExplorer::PhysicalFileExists(const std::filesystem::path& physicalPath)
	{
		return klib::CheckFileExists(physicalPath);
	}

	bool VirtualFileExplorer::FileExists(const std::filesystem::path& vtlPath)
	{
		auto paths = ResolveVirtualPath(vtlPath);
		const auto filename = vtlPath.filename();
		for (auto& path : paths)
		{
			if (klib::CheckFileExists(path / filename))
				return true;
		}
		return false;
	}

	klib::FileLines<char> VirtualFileExplorer::ReadFile(const std::filesystem::path& vtlPath)
	{
		auto paths = ResolveVirtualPath(vtlPath);
		const auto file = vtlPath.filename();

		fs::path filePath;
		for (auto& path : paths)
		{
			filePath = path / file;

			if (!PhysicalFileExists(filePath))
			{
				filePath.clear();
			}
		}
		
		return klib::ReadFile(filePath.string());
	}

	klib::PathList VirtualFileExplorer::ResolveVirtualPath(const std::filesystem::path& vtlPath)
	{
		if (PathExists(vtlPath))
		
		const auto parentPath = vtlPath.has_filename() ? vtlPath.parent_path() : vtlPath;

		const auto iter = redirectMap.find(vtlPath.string());

		if (iter == redirectMap.end())
		{
			KRK_WRN("Virtual F.E. redirect path does not exist: " + vtlPath.string());
			return {};
		}

		return iter->second;
	}

	std::filesystem::path VirtualFileExplorer::GetRoot()
	{
		return root;
	}
}

