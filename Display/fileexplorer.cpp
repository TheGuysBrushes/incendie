#include "fileexplorer.h"

FileExplorer::FileExplorer() : QFileSystemModel()
{	
}

FileExplorer::~FileExplorer()
{
}

QVariant FileExplorer::data(const QModelIndex& index, int role) const
{
	// On commence par appeler le fonction de base
            QVariant v = QFileSystemModel::data(index, role);
 
            // Si le QVariant est invalide, inutile d'allé plus loin ...
            if (!v.isValid()) {
                return v;
            }
 
            // Si cela concerne l'affichage d'un nom :
            if ( ( role == Qt::DisplayRole || role == Qt::EditRole ) && index.column() == 0 ) {
 
                // On réccupère les informations du fichier ou dossier en cour d'affichage
                QFileInfo fi = fileInfo(index);
 
                // Si il s'agit d'un dossier qui contient un fichier "plop.txt", BINGO !!!
                if (fi.isDir() && QFile::exists(QDir(fi.absoluteFilePath()).absoluteFilePath("plop.txt"))) {
                    // On retourne la valeur normal avec le smiley
                    return v.value<QString>() + "[  YEAH !!! ]";
 
                }
 
            }
 
            // Dans les autres cas, on retourne la valeur sans modification
            return v;
 
}
       

