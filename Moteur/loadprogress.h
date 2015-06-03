#ifndef LOADPROGRESS_H
#define LOADPROGRESS_H

class LoadProgress
{

public:
	LoadProgress();
	
	/// Doit mettre à jour de la valeur de la progression
	virtual void setProgress(int pourcentage) = 0;
	/// Doit fermer ou terminer la progression
	virtual void closeProgress() = 0;
	
};

#endif // LOADPROGRESS_H
