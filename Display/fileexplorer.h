#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QtGui/QFileSystemModel>

class FileExplorer : public QFileSystemModel {

private:
public:
 
	FileExplorer(); 
	virtual ~FileExplorer();

	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
            
};
#endif // FILEEXPLORER_H