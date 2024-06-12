#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <fstream>
#include <regex>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI, DRUGI, TRECI, CETVRTI };
const char* ispisRazred[] = { "PRVI","DRUGI","TRECI","CETVRTI" };
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = obj._trenutno;
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < obj._trenutno; i++) {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
    }
    Kolekcija& operator=(const Kolekcija& obj)
    {
        if (this != &obj) {
            delete[]_elementi1;
            delete[]_elementi2;
            _trenutno = obj._trenutno;
            _omoguciDupliranje = obj._omoguciDupliranje;
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];
            for (int i = 0; i < obj._trenutno; i++) {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        return *this;
    }
    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    void AddElement(T1 e1, T2 e2) {
        if (!_omoguciDupliranje)
            for (int i = 0; i < _trenutno; i++)
                if (_elementi1[i] == e1 && _elementi2[i] == e2)
                    throw exception("Dupliranje onemoguceno");
        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;
        _elementi1 = new T1[_trenutno + 1];
        _elementi2 = new T2[_trenutno + 1];
        for (int i = 0; i < _trenutno; i++) {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }
        _elementi1[_trenutno] = e1;
        _elementi2[_trenutno] = e2;
        _trenutno++;
    }
    Kolekcija<T1, T2>operator()(int from, int to) {
        Kolekcija<T1, T2>novi;
        if (from < 1 || to > 10)
            throw exception("Mozemo dodati samo 10 elemenata");
        for (int i = from; i <= to; i++) {
            novi.AddElement(_elementi1[i], _elementi2[i]);
        }
        return novi;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (int i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj)
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum& operator=(const Datum& obj)
    {
        if (this != &obj) {
            *_dan = *obj._dan;
            *_mjesec = *obj._mjesec;
            *_godina = *obj._godina;
        }
        return *this;
    }
    bool operator==(const Datum& obj) {
        return *_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan == *obj._dan;
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    int toDays() const{
        return *_godina * 365 + *_mjesec * 30 + *_dan;
    }
    bool operator>(const Datum& obj) {
        return this->toDays() > obj.toDays();
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
};

class Predmet {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<int, Datum> _ocjene;
public:
    Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
        _naziv = GetNizKaraktera(naziv);
        if (ocjena > 0)
            _ocjene.AddElement(ocjena, datum);
    }
    Predmet(const Predmet& obj)
    {
        _naziv = GetNizKaraktera(obj._naziv);
        _ocjene = obj._ocjene;
    }
    Predmet& operator=(const Predmet& obj)
    {
        if (this != &obj) {
            delete[] _naziv;
            _naziv = GetNizKaraktera(obj._naziv);
            _ocjene = obj._ocjene;
        }
        return *this;
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    bool operator==(const Predmet& obj) {
        return strcmp(_naziv, obj._naziv) == 0;
    }
    void AddOcjena(int ocjena, Datum datum) {
        _ocjene.AddElement(ocjena, datum);
    }
    float prosjekPredmeta() {
        float prosjek = 0.0f;
        if (_ocjene.getTrenutno() == 0)
            return 0;
        for (int i = 0; i < _ocjene.getTrenutno(); i++) {
            prosjek += _ocjene.getElement1(i);
        }
        return prosjek / _ocjene.getTrenutno();
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
    friend ostream& operator<<(ostream& COUT, Predmet& obj) {
        COUT << "Naziv predmeta-> " << obj._naziv << endl;
        COUT << " : : : OCJENE & DATUM : : : " << endl;
        for (int i = 0; i < obj._ocjene.getTrenutno(); i++) {
            COUT << "Ocjena-> (" << obj._ocjene.getElement1(i) << ")\t" << "Datum-> " << obj._ocjene.getElement2(i) << endl;
        }
        COUT << "Prosjek je-> " << obj.prosjekPredmeta() << endl;
        COUT << endl;
        return COUT;
    }
};
/*
   email adresa mora biti u formatu text.text (sa ili bez tacke),
   dok domena moze biti fit.ba ili edu.fit.ba
   nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

   za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
   validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
   postaviti je na defaultnu adresu: notSet@edu.fit.ba
   */
bool ValidirajEmail(string mail) {
    return regex_match(mail, regex("[a-z]{4,10}\.?[a-z]{3,4}@(fit|edu.fit)\.(ba|org|com)"));
}

class Uspjeh {
    eRazred _razred;
    //string se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>();
    }
    Uspjeh(const Uspjeh& obj)
    {
        _razred = obj._razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>(*obj._polozeniPredmeti);
    }
    Uspjeh& operator=(const Uspjeh& obj)
    {
        if (this != &obj) {
            delete _polozeniPredmeti;
            _razred = obj._razred;
            _polozeniPredmeti = new Kolekcija<Predmet, string>(*obj._polozeniPredmeti);
        }
        return *this;
    }
    ~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
    float prosjekRazreda() {
        float prosjek = 0.0f;
        if (_polozeniPredmeti->getTrenutno() == 0)
            return 0;
        for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++) {
            prosjek += _polozeniPredmeti->getElement1(i).prosjekPredmeta();
        }
        return prosjek / _polozeniPredmeti->getTrenutno();
    }
    Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << endl;
        COUT << "Razred: " << ispisRazred[obj._razred] << "\t" << *obj._polozeniPredmeti << endl;
        return COUT;
    }
};
mutex muteks;
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    void saljiMail(eRazred razred, Predmet predmet) {
        muteks.lock();
        cout << "FROM:info@fit.ba" << endl;
        cout << "TO: " << GetEmail() << endl;
        cout << "Postovani " << GetImePrezime() << ", evidentirali ste uspjeh za " << ispisRazred[razred] << " razred.";
        cout << "Dosadasnji uspjeh na nivou " << ispisRazred[razred] << " iznosi ";
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred)
                cout << _uspjeh[i].prosjekRazreda();
        }
        cout << ", a ukupni uspjeh u toku skolovanja iznosi " << prosjekUkupno() << ".\n" << "Pozdrav.\n" << "FIT Team." << endl;
        cout << endl;
        muteks.unlock();
    }

    void saljiSMS(eRazred razred) {
        muteks.lock();
        cout << endl;
        cout << "Svaka cast za uspjeh ";
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred)
                cout << _uspjeh[i].prosjekRazreda();
        }
        cout << " u " << ispisRazred[razred] << " razredu" << endl;
        cout << endl;
        muteks.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _brojTelefona = brojTelefona;
    }
    Kandidat(const Kandidat& obj)
    {
        _imePrezime = GetNizKaraktera(obj._imePrezime);
        _emailAdresa = obj._emailAdresa;
        _brojTelefona = obj._brojTelefona;
        _uspjeh = obj._uspjeh;
    }
    Kandidat& operator=(const Kandidat& obj)
    {
        if (this != &obj) {
            delete[] _imePrezime;
            _uspjeh.clear();
            _imePrezime = GetNizKaraktera(obj._imePrezime);
            _emailAdresa = obj._emailAdresa;
            _brojTelefona = obj._brojTelefona;
            _uspjeh = obj._uspjeh;
        }
        return *this;
    }
    ~Kandidat() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    float prosjekUkupno() {
        float prosjek = 0.0f;
        if (_uspjeh.size() == 0)
            return 0;
        for (int i = 0; i < _uspjeh.size(); i++) {
            prosjek += _uspjeh[i].prosjekRazreda();
        }
        return prosjek / _uspjeh.size();
    }
    bool AddPredmet(eRazred razred, Predmet predmet, string napomena) {
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred)
            {
                if (_uspjeh[i].GetPredmeti()->getTrenutno() > 5)
                    return false;
                if (predmet.prosjekPredmeta() < 2.5)
                    return false;
                for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                    if (predmet == _uspjeh[i].GetPredmeti()->getElement1(j))
                        return false;
                }
                _uspjeh[i].GetPredmeti()->AddElement(predmet, napomena);
                thread mail(&Kandidat::saljiMail, this,razred, predmet);
                mail.join();
                if (_uspjeh[i].prosjekRazreda() > 4.5) {
                    thread SMS(&Kandidat::saljiSMS, this, razred);
                    SMS.join();
                }
                return true;
            }
        }
        Uspjeh usp(razred);
        usp.GetPredmeti()->AddElement(predmet, napomena);
        _uspjeh.push_back(usp);
        thread mail(&Kandidat::saljiMail, this,razred, predmet);
        mail.join();
        return true;
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT<< "Ime i prezime: " << obj._imePrezime << "\n" << "Email: " << obj._emailAdresa << "\n"<< "Broj tel.: " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    /*vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float>*/
    Kolekcija<Predmet, float>operator()(Datum prvi, Datum zadnji) {
        Kolekcija<Predmet, float> novi;
        float prosjekDani = 0.0f;;
        for (int i = 0; i < _uspjeh.size(); i++) {
            for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++) {
                    if (_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k) > prvi && zadnji > _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k)) {
                        if (_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno() >= 1) {
                            prosjekDani++;
                            if (prosjekDani > 0) {
                                prosjekDani /= _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno();
                            }
                            novi.AddElement(_uspjeh[i].GetPredmeti()->getElement1(j), prosjekDani);
                        }
                        prosjekDani = 0.0f;
                    }
                }
            }
        }
        return novi;
    }
    Uspjeh* operator[](eRazred razred) {
        Uspjeh* usp = nullptr;
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred)
                usp = &_uspjeh[i];
        }
        return usp;
    }

};
const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Na sta se odnosi pojam reprezentacije tipa?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

    cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum temp,
        datum19062019(19, 6, 2019),
        datum20062019(20, 6, 2019),
        datum30062019(30, 6, 2019),
        datum05072019(5, 7, 2019);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
    ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
    Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
    cout << kolekcija2 << crt;
    try {
        //primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
        Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
        Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

   /* parametri: nazivPredmeta, prva ocjena, datum*/
    Predmet Matematika("Matematika", 5, datum19062019),
        Fizika("Fizika", 5, datum20062019),
        Hemija("Hemija", 2, datum30062019),
        Engleski("Engleski", 5, datum05072019);

    Matematika.AddOcjena(3, datum05072019);
    Matematika.AddOcjena(5, datum05072019);

     /*ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
     ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0*/
    cout << Matematika << endl;

    if (ValidirajEmail("text.text@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.org"))
        cout << "Email validan" << crt;

    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */

    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
    Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout  << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    /*vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float>*/
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
    cout << jasminUspjeh << crt;

    Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
    if (uspjeh_Irazred != nullptr)
        cout << *uspjeh_Irazred << crt;

    cin.get();
    system("pause>0");
}