class CPoint{
public:
	CPoint();
	~CPoint();
	
	void SetAbs(double value);
	void SetOrd(double value);
	void SetZord(double value);
	void SetFstDif(double value);
	void SetFstDifXZ(double value);
	void SetFstDifYZ(double value);
	void SetP(double value);
	void SetD(double value);
	void SetT(double value);
	
	double GetAbs();
	double GetOrd();
	double GetFstDif();
	double GetFstDifXZ();
	double GetFstDifYZ();
	double GetZord();
	double GetP();
	double GetD();
	double GetT();
private:
	double abs;
	double ord;
	double zord;
	double fstDif;
	double fstDifXZ;
	double fstDifYZ;
	double P;
	double D;
	double T;
};