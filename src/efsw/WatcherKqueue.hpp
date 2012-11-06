#ifndef EFSW_WATCHEROSX_HPP
#define EFSW_WATCHEROSX_HPP

#include <efsw/FileWatcherImpl.hpp>

#if EFSW_PLATFORM == EFSW_PLATFORM_KQUEUE

#include <map>
#include <vector>
#include <sys/event.h>
#include <sys/types.h>
#include <efsw/FileInfo.hpp>

namespace efsw
{

class FileWatcherKqueue;
class WatcherKqueue;

/// type for a map from WatchID to WatcherKqueue pointer
typedef std::map<WatchID, Watcher*> WatchMap;

typedef struct kevent KEvent;

class WatcherKqueue : public Watcher
{
	public:
		WatcherKqueue( WatchID watchid, const std::string& dirname, FileWatchListener* listener, bool recursive, FileWatcherKqueue * watcher, WatcherKqueue * parent = NULL );

		~WatcherKqueue();

		void addFile(  const std::string& name, bool emitEvents = true );

		void removeFile( const std::string& name, bool emitEvents = true );

		// called when the directory is actually changed
		// means a file has been added or removed
		// rescans the watched directory adding/removing files and sending notices
		void rescan();

		void handleAction(const std::string& filename, efsw::Action action);

		void handleFolderAction( std::string filename, efsw::Action action );

		void addAll();

		void removeAll();

		WatchID watchingDirectory( std::string dir );

		void watch();

		WatchID addWatch(const std::string& directory, FileWatchListener* watcher, bool recursive, WatcherKqueue * parent);

		void removeWatch (WatchID watchid );

		bool initOK();
	protected:
		/// The descriptor for the kqueue
		int					mKqueue;
		int					mDescriptor;
		WatchMap			mWatches;
		int					mLastWatchID;
		FileInfoMap			mFiles;

		// index 0 is always the directory
		KEvent				mKevent;

		FileWatcherKqueue *	mWatcher;

		WatcherKqueue *		mParent;

		std::vector<WatchID>	mErased;

		bool pathInWatches( const std::string& path );

		bool pathInParent( const std::string& path );
};

}

#endif

#endif
