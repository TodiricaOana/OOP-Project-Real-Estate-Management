#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <iterator>

using namespace std;

class locuinta
{
    string nume_client;
    double suprafata;
    int discount;
    double pret;

public:

    virtual void citire (istream & in) { in>>nume_client>>suprafata>>discount>>pret; }
    virtual void afisare (ostream &out)const { out<<"nume client: "<<nume_client<<"\nsuprafata: "<<suprafata<<"\ndiscount: "<<discount<<"\npret pe metru patrat:"<<pret; }

    friend istream & operator >> (istream & in, locuinta &a) { a.citire(in); return in; }
    friend ostream & operator << (ostream & out, const locuinta &a) { a.afisare(out); return out; }

    locuinta():nume_client("no name"),suprafata(0.0),discount(0), pret(0.0) {}
    locuinta(string nume, double S, int d, double p): nume_client(nume), suprafata(S), discount(d), pret(p) {}

    locuinta (const locuinta &a)
    {
        nume_client=a.nume_client;
        suprafata=a.suprafata;
        discount=a.discount;
        pret=a.pret;
    }

    locuinta &operator = (const locuinta &a)
    {
        if(this==&a)return *this;

        nume_client=a.nume_client;
        suprafata=a.suprafata;
        discount=a.discount;
        pret=a.pret;

        return *this;
    }

    double get_pret () { return pret; }
    double get_suprafata () {return suprafata;}
    int get_discount () {return discount; }
    string get_nume () {return nume_client;}

    virtual double calcul_chirie () = 0;

    virtual ~locuinta () {}

};


class apartament : public locuinta
{
    int etaj;

public:
    apartament():locuinta(){ etaj=0;}
    apartament(string nume, double S, int d, double p,int e):locuinta(nume,S,d,p) { etaj=e; }
    apartament(const apartament & a):locuinta(a) {etaj=a.etaj;}
    apartament &operator = (const apartament &a) { locuinta::operator=(a); etaj=a.etaj; return *this;}

    void citire (istream & in) { locuinta::citire(in); in>>etaj; }
    void afisare (ostream &out)const { locuinta::afisare(out); out<<"\netaj: "<<etaj<<'\n'; }

    friend istream & operator >> (istream & in,  apartament &a) { a.apartament::citire(in); return in; }
    friend ostream & operator << (ostream & out, const apartament &a) { a.apartament::afisare(out); return out; }

    double calcul_chirie()
    {
        double s,d;
        s=this->get_pret()*this->get_suprafata();
        d=s*this->get_discount()/100;
        s=s-d;
        return s;
    }

    ~apartament () {}

};

class casa: public  locuinta
{
    map<int,double> suprafata_pe_etaj;   //retine etajul si suprafata
    double suprafata_curte;
    double pret_curte;

public:
    casa(): locuinta() {suprafata_curte=0.0; pret_curte=0.0;}
    casa(string nume,double S, int d, double p, map<int,double>m, double sup_c, double p_c):locuinta(nume,S,d,p)
    {
        suprafata_pe_etaj=m;
        suprafata_curte=sup_c;
        pret_curte=p_c;
    }

    casa( const casa &a):locuinta(a)
    {
        suprafata_pe_etaj=a.suprafata_pe_etaj;
        suprafata_curte=a.suprafata_curte;
        pret_curte=a.pret_curte;
    }

    casa & operator = ( const casa & a)
    {
        locuinta::operator=(a);
        suprafata_pe_etaj=a.suprafata_pe_etaj;
        suprafata_curte=a.suprafata_curte;
        pret_curte=a.pret_curte;
        return *this;

    }
     void citire (istream & in)
     {
        locuinta::citire(in);
         int etaj,n;
         double sup;
         cin>>n;
         for(int i=0;i<n;i++)
         {
             in>>etaj>>sup;
            suprafata_pe_etaj.insert(pair<int,double>(etaj,sup));
         }
         in>>suprafata_curte;
         in>>pret_curte;

    }
    void afisare (ostream &out)const
    {
        locuinta::afisare(out);

        for(auto i=suprafata_pe_etaj.begin();i!=suprafata_pe_etaj.end();i++)
        {
             out<<"\netaj: "<<i->first<<"  , suprafata: "<<i->second;

        }

         out<<"\nsuprafata curte: "<<suprafata_curte<<"\npret curte: "<<pret_curte<<'\n';
    }

        friend istream & operator >> (istream & in,  casa &a) { a.casa::citire(in); return in; }
        friend ostream & operator << (ostream & out, const casa &a) { a.casa::afisare(out); return out; }

        double calcul_chirie()
        {
        double s,d;
        s=this->get_pret()*this->get_suprafata();
        d=s*this->get_discount()/100;
        s=s-d + suprafata_curte*pret_curte;
        return s;
        }

        ~ casa () {}

};


template <class T>
class gestiune
{
    vector<locuinta*>v;
    vector<double>chirii;
    static int nr;

public:
    gestiune () {};
    gestiune (vector <locuinta*>w,vector <double>c):v(w),chirii(c){}
    gestiune (const gestiune &a)
    {
        v=a.v;
        chirii=a.chirii;
    }
    gestiune<T> &operator = (const gestiune<T> &a)
    {
        if(this==&a)return *this;
        v=a.v;
        chirii=a.chirii;
        return *this;
    }
   void operator += (T x)
    {
        v.push_back(x);
        chirii.push_back(x->calcul_chirie());
        nr++;

    }

    template<class U> friend ostream &operator << (ostream &out,const gestiune<U> &x);

    ~gestiune () {}


};
template<class T> ostream &operator << (ostream &out,const gestiune<T> &x)
    {
          out<<"Numar total locuinte: "<<x.nr<<'\n'<<'\n';
        for(int i=0;i<x.nr;i++)
            out<<*(x.v[i])<<'\n';

        out<<"Pret chirii:\n";
        for(int i=0;i<x.nr;i++)
            out<<x.chirii[i]<<'\n';
        return out;
    }


template<class T>
int gestiune<T>::nr=0;


template <>
class gestiune<string>
{
    vector<string>nume;

public:
    gestiune () {}
    gestiune (vector <string>n): nume(n) {}
    gestiune (const gestiune &a)
    {
        nume=a.nume;
    }
    gestiune<string> &operator = (const gestiune<string> &a)
    {
        if(this==&a)return *this;
        nume=a.nume;
        return *this;

    }
    void operator +=(string n)
    {
        nume.push_back(n);

    }
    friend ostream &operator << <> (ostream &out,const gestiune<string> &x);

    ~gestiune (){}
};
template<> ostream& operator<< (ostream &out,const gestiune <string> &x)
{
    out<<'\n';
    out<<"Nume persoane:\n";
    for(unsigned i=0;i<x.nume.size();i++)
        out<<x.nume[i]<<'\n';

    return out;
}

int main()
{
    /*
    //aici am testat daca merg mai multi constructori:

    locuinta *a=new apartament("Maria",35, 6, 3, 2);
    apartament *b=static_cast<apartament*>(a);
    apartament c(*b);
    cout<<*a;
    cout<<a->calcul_chirie();
    cout<<'\n'<< *b;
    cout<<c;

    map<int, double>m;
    m[1]=12;
    m[2]=3;
    casa y("Costel",32,4,3,m,43,5);
    cout<<y;
    casa x(y);
     cout<<x;
    cout<<x.calcul_chirie();
*/

    gestiune <locuinta *>G ;
    gestiune <string>S;
    int n;
    n=4;

    locuinta *a[10];

    a[0]=new apartament("Maria",35, 6, 3, 2);

    map<int, double>m;
    m[1]=12;
    m[2]=3;
    locuinta *b=new casa("Costel",32,4,3,m,43,5);

    a[1]=b ;

    a[2]=new apartament("Mihai",21,3 , 5 ,2);

    a[3]=new casa("Daniel",26,5,6,m,31,8);

    for(int i=0;i<n;i++)
    {
        G+=a[i];
        S+=a[i]->get_nume();
    }
    cout<<G<<S;

    return 0;
}
