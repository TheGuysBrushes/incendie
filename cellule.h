#ifndef CELLULE_H
#define CELLULE_H

class Cellule
{
private:
	int etat;
	int pv;
	float coefficient;
public:

      // Constructors
      Cellule(int etat_init, int base_pv=0, float coef=0.0);
      Cellule(const Cellule& other);
      
      // Getters and setters
      int getEtat() const {return etat;};
			int getPv() const {return pv;};
			float getCoeff() const {return coefficient;};
      
      void setEtat(int x);
      void setPv(int y);
      void setCoeff(float z);
      
      // Other methods
      bool isOnFire() { return etat==2; };
	
      void enflammer() { etat= 2; };
      void blast() { etat= 3; };
	
// 	Cellule& operator=(Cellule cell);
	
};

#endif // CELLULE_H
