#include <efsw/FileSystem.hpp>
#include <efsw/platform/platformimpl.hpp>

#include <sys/stat.h>

namespace efsw {

bool FileSystem::isDirectory( const std::string& path )
{
	return Platform::FileSystem::isDirectory( path );
}

FileInfoMap FileSystem::filesInfoFromPath( std::string path ) {
	dirAddSlashAtEnd( path );

	return Platform::FileSystem::filesInfoFromPath( path );
}

char FileSystem::getOSSlash()
{
	return Platform::FileSystem::getOSSlash();
}

void FileSystem::dirAddSlashAtEnd( std::string& dir )
{
	if ( dir.size() && dir[ dir.size() - 1 ] != getOSSlash() )
	{
		dir.push_back( getOSSlash() );
	}
}

void FileSystem::dirRemoveSlashAtEnd( std::string& dir )
{
	if ( dir.size() && dir[ dir.size() - 1 ] == getOSSlash() )
	{
		dir.erase( dir.size() - 1 );
	}
}

std::string FileSystem::fileNameFromPath( const std::string& filepath )
{
	return filepath.substr( filepath.find_last_of( getOSSlash() ) + 1 );
}

std::string FileSystem::pathRemoveFileName( const std::string& filepath )
{
	return filepath.substr( 0, filepath.find_last_of( getOSSlash() ) + 1 );
}

std::string FileSystem::getLinkRealPath( std::string dir, std::string& curPath )
{
	FileSystem::dirRemoveSlashAtEnd( dir );
	FileInfo fi( dir, true );

	/// Check with lstat and see if it's a link
	if ( fi.isLink() )
	{
		/// get the real path of the link
		std::string link( fi.linksTo() );

		/// get the current path of the directory without the link dir path
		curPath = FileSystem::pathRemoveFileName( dir );

		/// ensure that ends with the os directory slash
		FileSystem::dirAddSlashAtEnd( link );

		return link;
	}

	/// if it's not a link return nothing
	return "";
}

}
