//FONDAMENTI DI INFORMATICA, elaborato

//----------------------------librerie

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>  //libreria che contiene algoritmi standard già pronti
                      //per essere usati(funzioni già pronte)
                      //permette random shuffle
#include <ctime> //per generare numeri casuali
                 //mi fa usare  srand(time(0));
#include <windows.h> //librerie utilizzate per la modifica della stampa

using namespace std;

//variabili globali
const size_t  manoGiocatoreLength = 8;   //numero di tessere che può avere in mano un giocatore( 8, costante)
bool fine = false; //variabile globale che mi indica la fine del gioco


//--------------classe che gestisce i punteggi
class Punt{  //Punt-->...Punteggio
private:

    int PunteggioTOT;   //Punteggio totale del gioc, ottenuto a parola inserita completamente(Non viene mai azzerato)

    int Punteggio;   //Punteggio dopo ogni inserimento di lettera,(parola corrente)

    vector<int> MoltPar; //contenitore dei moltiplicatori parola

    int NumLettere;  //Lettere utilizzate

    string parolaCorrente; //contenitore della parola in formazione


public:

    //costruttori

    Punt() : PunteggioTOT(0), Punteggio(0), MoltPar() , NumLettere(0) , parolaCorrente("") {} //specificando i valori di default

    Punt (Punt const& P): PunteggioTOT(P.PunteggioTOT), Punteggio(P.Punteggio), MoltPar(P.MoltPar),
                                    NumLettere(P.NumLettere) , parolaCorrente(P.parolaCorrente) {} //costruttore per copia

    // getter

    int GetPuntTot() const {
        return PunteggioTOT;
    }

    string GetParolaCorr() const {
        return parolaCorrente;
    }

    int GetPunt() const {
        return Punteggio;
    }

    // setter

    void azzera(){  //funzione usata solo alla fine del turno
        Punteggio = 0; //azzera il punteggio attuale, dell'ultima parola immessa
        parolaCorrente = ""; //e azzera la parola immessa,per future immissioni
        return;
    }

    //funzioni

    void NoJolly(){
        PunteggioTOT+=10;  //se non si usa jolly punteggio di +10
        return;
    }
                    //let è la lettera in input
    Punt& FunzPunt (char let, int CordC, int CordR, int TabellaMolt[][17], int ControlloFine) {

        if(CordR>16 || CordC>16 || CordR<0 || CordC<0)
        {return *this;}  //se sbaglio immissione coordinate ritorna questa classe punteggio senza modifiche

        if(ControlloFine==0)      //controllo se si è inserita tutta la parola(il carattere 0 di terminazione)
        {

            if(NumLettere<4)      //controllo che la parola non sia più lunga di tre lettere
            {                           //se sì,non da'  punti
                parolaCorrente = "";
                Punteggio=0;
                NumLettere = 0; //riazzero parolaCorrente e NumLettere ovvero variabili che formano la parola corrente(da inserimento)
            }

            if(MoltPar.size()!=0)   // bonus moltiplicatori (alla parola totale qua)
            {
                for(unsigned int temp = 0; temp<MoltPar.size(); temp++) //finche sono presenti moltiplicatori parola...
                {
                    Punteggio*=MoltPar[temp];  //...moltiplico il punteggio della parola appena formata
                    MoltPar.erase(MoltPar.begin()+temp);  //tolgo moltiplicatore appena utilizzato
                }                   //begin() da dove parte...+temp è l'offset
            }



            //controllo lunghezza parola per i bonus ( se numLettere >= 6 --> bonus ulteriori )
            if (NumLettere==6)
            {
                PunteggioTOT+= 10;
            }
            else
            {
                if (NumLettere==7)
                {
                    PunteggioTOT+= 30;
                }
                else
                {
                    if (NumLettere==8)
                    {
                        PunteggioTOT+= 50;
                    }
                }
            }

            if(parolaCorrente == "scarabeo" || parolaCorrente == "scarabei")
            {
                PunteggioTOT += 100;
            }

            NumLettere = 0;
            parolaCorrente = "";  //azzero tutto perchè il punteggio sulla parola attuale è stato contato
            Punteggio=0;
            return *this;  // ritorno l'oggetto punteggio  modificato
        }
        else  //se il controllo fine è falso ( != 0 )   --> è un altra lettera
        {
            parolaCorrente += let; //aggiungo la lettera alla parola in formazione

            if(TabellaMolt[CordR][CordC]== 2 || TabellaMolt[CordR][CordC]== 3)  //controllo moltiplicatori parola
            {                                                                    // 2 e 3 indicano un moltiplicatore parola
                                                                                  // 4 e 5 indicano un moltiplicatore lettera

                MoltPar.push_back( TabellaMolt[CordR][CordC] ); //metti nel vet dei  moltiplicatori il moltiplicatore
                TabellaMolt[CordR][CordC] = 0;  //bonus già preso quindi azzero             // il vet lo usi per il i moltiplicatori parola
            }

            if(TabellaMolt[CordR][CordC]== 4 || TabellaMolt[CordR][CordC]== 5) //controllo moltiplicatori lettera
            {                                                                   //qui moltiplica la lettera singola
                if(TabellaMolt[CordR][CordC] == 4)
                {
                    SommaPunt(let,2);  //aggiorno punteggio corrente
                }                       //sommaPunt è per la lettera singola
                else
                {
                    SommaPunt(let,3);  //aggiorno punteggio corrente
                }
                TabellaMolt[CordR][CordC] = 0;
            }
            else
            {
                SommaPunt(let,1);  //aggiorno punteggio corrente
            }

            NumLettere+=1;  //nuova lettera inserita, parola attuale ha +1 numLettere
        }

        return *this;

    }


    void SommaPunt (char let, int i){   //Aggiorno il punteggio in base al valore della lettera e ai moltiplicatori

        if(let=='a' || let=='c' || let=='e' || let=='i' ||

                let=='o' || let=='r' || let=='s' || let=='t' )
        {
            Punteggio += 1*i;  //Lettere che danno punteggio 1, per(x) il moltiplicatore trovato
        }

        if(let=='n' || let=='m' || let=='l' )  //lettere che danno punteggio 2 ...
        {
            Punteggio += 2*i;
        }

        if(let=='p')
        {
            Punteggio += 3*i;
        }

        if(let=='b' || let=='d' || let=='f' ||
                let=='g' || let=='u' || let=='v' )
        {
            Punteggio += 4*i;
        }

        if(let=='h' || let=='z')
        {
            Punteggio += 8*i;
        }

        if(let=='q')
        {
            Punteggio += 10*i;
        }
        return;
    }

};

//----------------------------------- classe che gestisce il giocatore

class Giocatore{

private:
    int NUM;         //variabile che associa al giocatore un turno(turno è un intero)
    vector<char> G;  //vettore lettere a disposizione del giocatore
    Punt P;          //utilizza la classe punteggio per la gestione di esso(ogni giocatore ha un punteggio)
    int PunteggioMano; //usta a fine gioco
public:

    //costruttori

    Giocatore(): NUM(), G() , P() , PunteggioMano() {}   //costruttore di default

    Giocatore(int const num): NUM(num), G(), P(), PunteggioMano(){}  //costruttore con parametri

    Giocatore(Giocatore const& g): NUM(g.NUM), G(g.G), P(g.P), PunteggioMano(g.PunteggioMano){}  //costruttore per copia
            //bisogna passare un oggetto per riferimento nel costruttore per copia
    
    
    //getter
    int GetTurno() const { return NUM ; }

    char getMano(int const i) const { return G[i]; }  //get di una lettera specifica

    vector<char> getLetereDisp() const {return G;}    //get vettore lettere disponibili(mano del giocatore)

    size_t GetSize() const { return G.size(); }          //get numero di lettere disponibili

    int GetPunt() const { return P.GetPuntTot(); }    //get punteggio totale(del giocatore)è

    int GetPuntAttuale() const { return P.GetPunt(); } //get punteggio della parola in formazione

    string GetParolaCorr() const { return P.GetParolaCorr(); }  //get parola in formazione

    int GetPunteggioMano () const { return PunteggioMano; }

    //funzioni

    bool empty() {
        return G.empty();    //funzione che ritorna true se la mano del giocatore è vuota(vettore G)
    }

    void punteggioMano ()   {  //funzione che calcola il punteggio di lettere singole(senza bonus) in mano a un giocatore
            //verrà usata alla fine del gioco, l 'ultimo a comporre una parola riceve unPunteggio somma delle lettere in mano a ciascun giocatore
        P.azzera();   //azzera punteggio attuale precedente(dell ultima parola immessa ,nel turno prima)
        if(G.empty() == false) //se il vet giocatore non è vuoto
        {
            for(unsigned int i = 0; i<G.size(); i++)
            {
                P.SommaPunt(G[i],1);  //calcola punteggio intera mano senza moltiplicatori( int i =1)
            }
            PunteggioMano = P.GetPunt();  //setta punteggio mano = al punteggio calcolato
        }
        else
        {
            PunteggioMano = 0;
        }
        P.azzera();//azzera punteggio ottenuto in questo turno tanto è gia stato salvato
        return ;
    }

    Giocatore& riempiMano(vector<char>& lettere) { //char lettere per & perchè deve venir modificato

        while( G.size() <  manoGiocatoreLength )  //finchè la mano del giocatore attuale è minore di 8(valore costante di manoGiocatoreLength)
        {
            if( !lettere.empty() )   //e se le tessere non sono finite(vector delle tessere non è vuoto)
            {
                G.push_back( lettere.back() ); //mette nel vector mano del giocatore attuale, una lettera del vector tessere(con .back si prende l'ultima,quella in coda)
                lettere.pop_back();  //visto che la tessera è stata pescata,non è più nel vector lettere, quindi viene tolta (pop back toglie l'ultimo elemento del vector)
            }
            else
                break; //Se le tessere rimanenti(lettere rimanenti) sono finite ,rompe il ciclo e esce
        }
        return *this; //modifica l oggetto corrente
    }


    Giocatore CreaManoProva (){

        Giocatore Gprova(*this); //costruzione per copia di un giocatore temporaneo = a quello attuale
        return Gprova;  //ritorno la copia del gioc
    }

    bool ControlloTraLeTessere (char lettere ){  //controllo di presenza di una lettera nella mano del gioc e la tolgo perchè sarà usata

        bool a=false;  //variabile da restituire
        for( unsigned int L=0;L<G.size();++L){  //controllo ogni tessera...
            if (lettere== G[L]){                //...se la lettera è presente nella mia mano
                G.erase(G.begin()+L);           // la tolgo dalle mie tessere,, begin + offset
                a=true;                         //imposto che la lettera è stata trovata
                break;                          //esco dal ciclo perchè la lettera è stata trovata
            }

        }
        return a;
    }


    Giocatore& CalcolaPunteggio(char let, int CordC, int CordR, int TabellaMolt[][17], int ControlloFine) {  // aggiornamento punteggio
        P.FunzPunt( let,  CordC,  CordR, TabellaMolt, ControlloFine); //funzione che modifica il punteggio del giocatore(e compone anche
        return *this;  //ritorna Questo oggetto                                                                              la parola)
    }


    void NoJolly (){ //per l'assenza di jolly, da' più punti  (+10)
        P.NoJolly();  //va richiamata in questa classe(giocatore) per modificare l'attributo punteggio interno alla classe giocatore
        return;
    }

    Giocatore& SostituisciLettera() { //funzione utilizzata poi nella gestione dei jolly
        G.push_back('$');  //mette il jolly nella mano del giocatore(usato quando si tira su un jolly)
        return *this;
    }


};

void Print (Giocatore const& G) { //funzione di stampa della classe ( funzione non membro )
    cout<<" Turno del giocatore : "<<G.GetTurno()<<endl;  //NUM è la variabile che indica il turno (In getTurno)
    cout<<" Tessere disponibili : ";
    for( size_t t=0; t<G.GetSize(); ++t)
    {
        cout<<" "<<G.getMano(t)<<" , ";
    }
    cout<<endl;
    cout<<" Punteggio totale : "<<G.GetPunt()<<"     Punteggio attuale : "<<G.GetPuntAttuale(); //getPunt-punteggio totale del giocatore
    cout<<endl<<endl;
}                               //getPuntAttuale è il punteggio ottenuto durante il turno

//------------------Struct jolly

struct jolly {
    //valore dei jolly
    char val = '0';
    char val2 = '0';
    //coordinate dei jolly
    int cordC = -1;
    int cordR = -1;
    int cordC2 = -1;
    int cordR2 = -1;
    //controllo posizionamento
    bool posizionato = false ;   //per primo jolly, true se in campo
    bool posizionato2 = false ;  //per secondo jolly( uso un unica struct per i due)
};

jolly J ;   // variabile globale, visibile a tutti.

//-----------------Struct suggerimento

struct ParolaSugg {
    string parola = "";  //parola suggerita
    //coordinate di inizio
    int Criga=-1;
    int Ccolonna=-1;            //inizializzo le variabili della struct
    int punteggio = 0;
    char direzione = '0';
    int NumIntersezioni = 0; //intersezioni effettuate
    int numjolly = 0; //numero di jolly utilizzati nella parola
};
//-------------------------------------------------------------Prototipi funzioni-----------------------------------------------------------------
//-----Funzioni di inserimento normale
void funzioneInserimentoParola( const vector<string>& vocabolario,char scarabeo [][17],Giocatore &G,vector<char>& lettere,
                                    int griglia[][17],int& turnozero );
bool Inserimento (char direzione, Giocatore& G,int Criga, int Ccolonna,const vector<string>& vocabolario, char scarabeo [][17],
                    int griglia[][17], int& Numerointersezioni, vector<char> & lettere, int & Numjolly, int& NumLet);

//-----Funzioni per la gestione dei suggerimenti
void InserisciSuggerimento (ParolaSugg suggerimento, char scarabeo [][17], int griglia[][17], Giocatore& G, size_t contatore);
bool Suggerimento (Giocatore& G,const vector<string>& vocabolario, char scarabeo[][17],int griglia[][17],
                        ParolaSugg& Paroladef, bool turnozero);
bool PossibilitaSuggerimento (int const Criga, int const Ccolonna, char direzione, char scarabeo[][17], size_t size);
void SuggerimentoRicorsivo(char const direzione, int Criga, int Ccolonna, vector<char> lettereDisp, char scarabeo[][17],
                            int griglia[][17], const vector<string>& vocabolario, ParolaSugg& Paroladef,
                             int& NumIntersezioni, int& Letteremie, int& numjolly);
ParolaSugg ComponiParola (char direzione, int Criga, int Ccolonna, char scarabeo [][17], int griglia[][17]);
bool verificaIniziali(string iniz, const vector<string>& dizionario);
bool inizialiParola(string parolaTot, string parolaParz);

//----Funzioni per la gestione dei jolly
int Esistenzajolly ();
void Prendijolly ( Giocatore & G, char scarabeo[][17], int i);
void JOLLY( Giocatore & G, char scarabeo[][17]);
char Mettijolly ( int CoorC, int CoorR,bool Presenzasugg, char let);
void StampaJolly();

//----
void creaLettere( vector<char>& lettere );

//----Funzione di controllo
bool controlloIntersezioni (char scarabeo[][17], int coordinatariga , int coordinatacolonna,
                            const vector<string>&vocabolario,char parola);

//----Funzione di movimento
void CoordinateInizio(char DirezioneInserimento, int& CRiga, int& CColonna, char Scarabeo [][17]);

//----Funzioni di copia
void AnnullaTurnoPerErrori (char scarabeo [][17], char TabellaProva[][17], Giocatore& G,
                                    Giocatore& Gprova, int griglia[][17], int grigliaProva [][17]);
void CreaManoProva (char scarabeo [][17], char TabellaProva[][17],Giocatore& G, Giocatore& Gprova ,
                            int griglia[][17], int grigliaProva [][17]);
void CopiaGriglia (int Griglia[][17], int Grigliaprova [][17]);
void CopiaTavola (char Scarabeo[][17], char Scarabeoprova [][17]);
void CreaTavola (char scarabeo[][17]);

//----funzioni che utilizzano il dizionario
bool ControlloEsistenzaParola (const vector<string>&vocabolario, string ParolaCorrente );
bool ricercaBinariaDizionario(const vector<string>& vocabolario, string parola);
void FunzCreaDizionario( vector<string>& vocabolario);

//----Funzione stampa
void Stampa (char scarabeo [][17], Giocatore const& G, vector<char>& lettere,int griglia[][17]);
void StampaTitolo ();
void SetColor(short Color);
//-----
//-----
//-----
//-----
//-------------------------------- corpo del programma

void StampaTitolo (){
    cout<<"  _____    _____    _____    _____    _____    _____    _____    _____ "<<endl;
    cout<<" |   __|  |     |  |  _  |  | __  |  |  _  |  | __  |  |   __|  |     |"<<endl;
    cout<<" |__   |  |   --|  |     |  |    -|  |     |  | __ -|  |   __|  |  |  |"<<endl;
    cout<<" |_____|  |_____|  |__|__|  |__|__|  |__|__|  |_____|  |_____|  |_____|"<<endl;
    cout<<endl<<endl<<endl<<endl<<endl;
}

//--------------------------------
void CreaTavola (char scarabeo[][17]){
    for(unsigned int r = 0; r<17; r++)
    {
        for(unsigned int c = 0; c<17; c++)
        {
            scarabeo[r][c] = '0';  //tutte le caselle sono inizializzate a zero (che per noi vuol dire vuote)
        }
    }
}

//--------------------------------
int main(){

    StampaTitolo();   //stampa la schermata iniziale del gioco

    char controllo;    // chiedo se si vuole iniziare una partita
    do{
        cout<<" Vuoi iniziare una partita? (si: s  -  no: n): ";
        cin>>controllo;
    }while(controllo != 's' && controllo != 'n');
    if(controllo=='n'){return 0;}   //fine del gioco
    //se non è   n   --> si gioca
    char scarabeo[17][17];  //creo la tavola da gioco, in cui verranno inserite le lettere
    CreaTavola(scarabeo);

    //altra,ulteriore griglia per i moltiplicatori
    int griglia[17][17] {
        {3,0,0,0,4,0,0,0,3,0,0,0,4,0,0,0,3},             //zero indica l'assenza di moltiplicatori
        {0,2,0,0,0,0,5,0,0,0,5,0,0,0,0,2,0},             // 2 e 3 indicano un moltiplicatore parola
        {0,0,2,0,0,0,0,4,0,4,0,0,0,0,2,0,0},             // 4 e 5 indicano un moltiplicatore lettera
        {0,0,0,2,0,0,0,0,4,0,0,0,0,2,0,0,0},
        {4,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,4},
        {0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0},
        {0,5,0,0,0,0,5,0,0,0,5,0,0,0,0,5,0},
        {0,0,4,0,0,0,0,4,0,4,0,0,0,0,4,0,0},
        {3,0,0,4,0,0,0,0,0,0,0,0,0,4,0,0,3},
        {0,0,4,0,0,0,0,4,0,4,0,0,0,0,4,0,0},
        {0,5,0,0,0,0,5,0,0,0,5,0,0,0,0,5,0},
        {0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0},
        {4,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,4},
        {0,0,0,2,0,0,0,0,4,0,0,0,0,2,0,0,0},
        {0,0,2,0,0,0,0,4,0,4,0,0,0,0,2,0,0},
        {0,2,0,0,0,0,5,0,0,0,5,0,0,0,0,2,0},
        {3,0,0,0,4,0,0,0,3,0,0,0,4,0,0,0,3}
    };

    vector<string> vocabolario;
    FunzCreaDizionario(vocabolario);  //creo il dizionario contenente le parole esistenti
    vector<char> lettere;
    creaLettere(lettere); //creo il contenitore di tutte le lettere utilizzabili

    int i;
    do{
        cout<<" inserisci il numero di giocatori (almeno 2 e al massimo 4) : ";
        cin>>i;
        cin.clear(); //se cè un errore nel cin,viene attivato il flag di errore del cin, clear resetta il flag(bit) di errore (per input errato),esempio se metto lettera in una Int),pulisco questo bit così le future operazioni di I/O funzioneranno come al solito(bene)
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignora l'input errato (per far inserire solo numeri)(L'INPUT sbagliato precedente sta ancora nel buffer,lo leggerai ancora una volta e ti porterà a un nuovo flag d'errore, per evitare ciò,mettere questo che ti fa saltare(skip) un numero gradissimo di caratteri in input dal buffer(Numeric limit),ma ciò fino alla /n,ovvero nuova riga,da li in poi ok->nuovo input ok
        cout<<endl;
    }while(i<2 || i>4);

    int turnozero = 0; //variabile di controllo del primo turno

    vector<Giocatore> giocatori; //vettore contenente oggetti giocatore(più  giocatori)

    int k;
    for( k = 1; k < i+1; k++)  //creo un numero di giocatori pari a quello specificato
    {                               //faccio partire da 1, giocatore1...gioc2...
        Giocatore temp (k);   //costruttore con parametro-> il turno,(NUM ,turno associato al gioc)
        giocatori.push_back(temp);  //riempo un vettore contenente giocatori
    }

    int turno = 0;  //la var intera turno parte da 0(verrà poi incrementata per ogni turno),indica il turno attuale
    int conta = i-1;  //quindi alla variabile che conta i turni devi sottrargli 1(variabile che poi rimane uguale,indica il numero totale di turni)
    do{
        funzioneInserimentoParola(vocabolario,scarabeo,giocatori[turno],lettere,griglia,turnozero ); //funzione inserimento del giocatore : giocatori[turno]
        if (turno == conta) //se sono al turno dell'ultimo giocatore(tutti i turni sono stati fatti)
        {
            turno = 0; //torno al turno del primo                         //in questo dowhile si gestisce il turno
        }
        else
        {
            turno++; //altrimenti passo al giocatore successivo
        }
    }while(fine == false); //si giocano i turni finchè il gioco non finisce

    //se si arirva qua vuol dire che si è a fine del gioco
    int PunteggioConclusione = 0;
    for(k = 0; k< i; k++)    //calcolo il punteggio rimanente tra le tessere di ogni giocatore
    {
        Giocatore G = giocatori[k];   //per ogni giocatore
        G.punteggioMano(); //calcola il punteggio otteuto dalle tessere rimaste in mano al giocatore
        const int a = G.GetPunteggioMano(); //get il punteggio ottenuto da funz PunteggioMano per giocatore corrente
        PunteggioConclusione += a ;  //all'ultimo  giocatore ad aver composto una parola, verrà sommata anche la somma di tuttel le lettere dei giocatori perdenti
    }                                   //(ovvero il giocatore che finisce le tessere totali)

    int Vincitore = 0;
    int PuntVincitore = 0;
    cout<<endl<<endl;
    for(k = 0; k< i; k++) //per ogni turno
    {
        Giocatore G = giocatori[k];  //prendi il giocatore corrente
        const int turnoo = G.GetTurno();
        if(G.empty() == true) //se sono il giocatore che ha finito le lettere
        {
            cout<<"  Punteggio giocatore "<<turnoo<<" : "<<giocatori[k].GetPunt()<< " + "<<PunteggioConclusione<<" = "<<giocatori[k].GetPunt() + PunteggioConclusione<<endl;
            if(giocatori[k].GetPunt() > PuntVincitore)                 //getpunt ottiene il punteggio totale del giocatore
            {  //al primo indice del ciclo for entra di sicuro in questo if(per sommare puntConclusione al giocatore che ha finito le tessere,in altri cicli entra solo se maggiore
                Vincitore = k; //controllo se k è il vincitore(controllare in tutti i casi!)
                PuntVincitore = giocatori[k].GetPunt() + PunteggioConclusione;
            }
        }
        else //se non lo sono
        {                      //il gioco finisce quando un giocatore finisce le lettere, ma non è detto che quel giocatore sia anche quello col punteggio piu alto
            cout<<"  Punteggio giocatore "<<turnoo<<" : "<<giocatori[k].GetPunt()<<endl; //stmapare lo stesso punteggio di ogni giocatore
            if(giocatori[k].GetPunt() > PuntVincitore)
            {//controllo se k è il vincitore(controllare in tutti i casi!)
                Vincitore = k;          //setta questo giocatore come vincitore se il punteggio di questo giocatore è > di PuntVincitore
                PuntVincitore = giocatori[k].GetPunt();
            }
        }
    }

    if(fine == true)  //se gioco finito
    {
        cout<<endl<<endl;
        SetColor(12);  //colore del testo: rosso
        cout<<"  Il vincitore e' il giocatore "<<Vincitore+1<<" che realizza un punteggio di "<<PuntVincitore<<"."<<endl<<endl;
        cout<<"  GAME OVER"<<endl;
        SetColor(15); //bianco
    }
    return 0;
}

//---------------------------- funzione di inserimento

void funzioneInserimentoParola( const vector<string>& vocabolario,char scarabeo [][17],Giocatore &G,vector<char>& lettere, int griglia[][17],int& turnozero )
{
    system ("cls");
    if (G.GetSize() == 0 &&  lettere.size() == 0) //il controllo c'è anche a fine blocco. Questo serve perche se finisco
    {                                             //le lettere in mano e il turno dopo quelle rimanenti, probabilmente rientrerà in questa funzione
        fine = true; //fine gioco
        return;  //esco
    }

    char TabellaProva[17][17];
    int grigliaProva [17][17];
    Giocatore Gprova;
    jolly JProva = J;  //J (maiuscolo) è variabile globale jolly(struct) (JProva è variabile per salvare stato precedente del jolly,che poi si può riutilizzare se ci sono errori e devo tornare indietro)

    G.riempiMano(lettere);    // il giocatore pesca(riempie mano del giocatore fino ad 8)
    CreaManoProva(scarabeo,TabellaProva,G,Gprova, griglia,grigliaProva);    //crea una nuova griglia e un gruppo di tessere identiche a quelle gia esistenti per tornare in dietro in caso di errore
    //--                                                                    // e crea una copia (diProva) del giocatore corrente
    Stampa(scarabeo,G,lettere,griglia); //funzione di stampa
    JOLLY(G,scarabeo);     //funzione che gestisce i jolly... viene controllata a qualsiasi mano(ad esempio per tirar su un jolly)
    system("cls");
    //---
    char scelta;     // mi serve per chiedere al giocatore se giocare o passare.
    bool continua = true;  // variabile per controllare l'effettiva correttezza delle operazioni
    do{
        Stampa(scarabeo,G,lettere,griglia);

        do {
            cout<<"Inserisci se passare o giocare(g: giocare, p: passare): ";
            cin>>scelta;
        }while(scelta!='g' && scelta!='p');
        if(scelta == 'p')  // se si è scelto di passare il turno
        {
            system("cls");
            return; //esco dalla funzione,E torna al do while dove si gestisce la funzione
        }
        else
        {
            if(scelta == 'g') // se si è scelto di giocare
            {
                char direzione;
                cout<<"   v  : inserimento in verticale"<<endl;
                cout<<"   o  : inserimento in orizzontale"<<endl;
                cout<<"   -  : annullare operazione"<<endl;
                cout<<"   .  : utilizzare suggerimento : ";
                do{
                    cin>>direzione;
                }while(direzione!='v' && direzione!= 'o' && direzione!= '-' && direzione!= '.');
                if(direzione!='-' && direzione!= '.') //se non si vuole utilizzare il sugg. o annullare
                {
                    int Numintersezioni;
                    int numJ = 0;
                    int NumLet= 0;
                    if (turnozero != 0) //se non è il primo turno
                    {
                        int Criga;
                        cout<<"inserisci la coordinata della riga.-1 per annullare. (y) : ";
                        do{
                            cin>>Criga;
                        }while(Criga!=-1 &&(Criga < 0 || Criga > 16) );  //inserimento è da 0 a 16 ,se no errore
                        if(Criga!= -1) //se Non ho voluto annullare l inserimento riga
                        {
                            int Ccolonna;
                            cout<<"inserisci la coordinata della colonna.-1 per annullare. (x) : ";
                            do{
                                cin>>Ccolonna;
                            }while(Ccolonna!=-1 &&(Ccolonna<0 || Ccolonna>16));
                            if(Ccolonna!= -1)
                            {
                                CoordinateInizio(direzione,Criga,Ccolonna,scarabeo); //mi sposto sull'inizio della parola se sono gia presenti lettere
                                Numintersezioni=0;  //azzero variabile, che magari poteva essere diversa da 0 per precedenti inserimenti
                                NumLet = 0;
                                continua = Inserimento(direzione,G,Criga,Ccolonna,vocabolario,scarabeo,griglia,Numintersezioni,lettere,numJ,NumLet); //proseguo con l'inserimento,ritora true se va a buon fine
                            }                                                                                                                        // e se fa intersez
                            else
                            {
                                direzione = '-'; //annullo direzione
                            }
                        }
                        else
                        {
                            direzione = '-';
                        }
                    }
                    else //se è il primo turno,inserimento per forza dal centro
                    {
                        turnozero++;
                        Numintersezioni = 1;  //numero intersez lo imposto io a 1,giusto per far scrivere la parola, che se no,avrebbe 0 intersezioni
                        NumLet = 0;
                        continua = Inserimento(direzione,G,8,8,vocabolario,scarabeo,griglia,Numintersezioni,lettere,numJ,NumLet); //proseguo con l'inserimento(NB criga e ccolonna costanti,valgono 8)
                    }
                }
                 //se non si è scelto l'inserimento normale

                if(direzione == '.')  // se si è scelto il suggerimento(il puntino lo salvi sempre nella variabile per la direzione
                {
                    ParolaSugg Paroladef; //parola suggerita(variabile struct)
                    bool SUG = false;  //verrà settata true se possibile trovare un suggerimento
                    if(turnozero == 0)
                    {
                        SUG = Suggerimento(G,vocabolario,scarabeo,griglia,Paroladef,true);  //suggerimento sul primo turno(diverso)
                        turnozero++;
                    }
                    else
                    {
                        SUG = Suggerimento(G,vocabolario,scarabeo,griglia,Paroladef,false);  //suggerimento normale
                    }
                    if (SUG == true)  // se esiste un possibile suggerimento...
                    {
                        InserisciSuggerimento(Paroladef,scarabeo,griglia,G,0); //...inseriscilo
                        Paroladef.parola = "";   //riazzero i parametri della struct ParolaSugg
                        Paroladef.punteggio = 0;
                        continua = true;  //continua=true,il tutto è andato a buon fine
                        system("cls");  //cancella i comandi
                        Stampa(scarabeo,G,lettere,griglia);  //ristampa la griglia
                    }
                    else //se non è possibile nessun suggerimento
                    {
                        continua = false;
                    }
                }
                if(direzione == '-')  // se si è scelto di annullare
                {
                    continua= false; // se hai premuto -
                }

            }
        }
        if(continua==true){   //se sono arrivato qua, il tutto è andato a buon fine
            char temp;
            do{
                cout<<"ATTENZIONE : Confermare? (si: s - no: n) : ";
                cin>>temp;
            }while(temp != 's' && temp != 'n');
            if (temp== 'n'){  //se si vuole annullare tutto
                continua=false;  //setto continua a false per annullare
            }
            else //se si conferma
            { turnozero=-1;}//esco dal turno zero(il primo di tutto,dove si parte dal centro-->d'ora in poi non farlo più) se continua == true al primo giro
            system("cls");
        }
        if(continua==false){
            if(turnozero == 1){turnozero--;}  //annullo mossa-->turno zero(che era stato incrementato) ritorna uguale a prima
            system("cls");
            cout<<"OPERAZIONE NON EFFETTUABILE."<<endl;
            //annullo tutto ciò che è stato fatto in questo turno
            J=JProva;  // il jolly ritorna = a prima,come non averlo toccato
            AnnullaTurnoPerErrori(scarabeo,TabellaProva,G,Gprova, griglia,grigliaProva);
        }

    }while( continua == false);

    if (G.GetSize() == 0 &&  lettere.size() == 0)  //se lettere da pescare finite e il giocatore di qsto turno ha finito le proprie lettere-->fine del gioco
    {
        fine = true;
        return;
    }
    return;

}
//-------
bool Inserimento (char direzione, Giocatore& G,int Criga, int Ccolonna,const vector<string>& vocabolario,
                  char scarabeo [][17], int griglia[][17], int& Numerointersezioni, vector<char> & lettere, int& Numjolly, int& NumLet){

    int NumeroIntersezionitemp = Numerointersezioni;
    int NUmjollytemp = Numjolly;   //in main cè numJ->significa numero di jolly
    int NumLettemp = NumLet;

    system("cls");

    if((Criga < 0) || (Criga>16) || (Ccolonna < 0) || (Ccolonna > 16)) //se non posso piu inserire niente
    {
        if (Numerointersezioni == 0 || NumLet == 0 ) //se non ho messo lettere o se non ho fatto intersezioni
        {return false;} //ovvero  non puoi inserire      (NumLet è il numero di lettere attuale che compongono la parola)
        else {
            //controllo l'esistenza della parola
            if(ControlloEsistenzaParola(vocabolario,G.GetParolaCorr()) == false){return false;} //se la parola non esiste
            else //se la parola esiste
            {
                if(Numjolly == 0)
                {
                    if(G.GetParolaCorr().size()>3)  //se parola corrente ha più di 3 lettere,guardo se non usi jolly per ulteriori punti bonus
                    G.NoJolly();//visto che sono nell IF (numJolly=0) -->dare +10 punti nojolly
                }
                G.CalcolaPunteggio('0',0,0,griglia,0); //"0" usato per dire che la parola è finita. Passo delle coordinate sbagliate ma non influiscono
                return true;                                                                 //sul punteggio
            }                                                                                //anche perchè nessuna lettere nuova è stata inserita(0)
        }
    }

    bool corretto=true;//per vericare che non ci siano annnullamenti,errori...
    char let;  //variabile che contiene la lettera da inserire

    char TabellaProva[17][17];
    int grigliaProva [17][17];
    Giocatore Gprova;
    jolly Jprova = J;
    CreaManoProva(scarabeo,TabellaProva,G,Gprova, griglia,grigliaProva); //creo una mano per tornare indietro in caso di errori

    if(scarabeo[Criga][Ccolonna] != '0')  //se c'è gia una lettera giù sulla griglia (!=0)
    {                                       //e quella lettera voglio usarla per la mia parola nuova da mettere
        Numerointersezioni++;//...vuol dire che sto facendo una intersezione
        let = scarabeo[Criga][Ccolonna];  // la lettera attuale da inserire diventa questa(qsta trovata già in griglia)
        G.CalcolaPunteggio(let,Ccolonna,Criga,griglia,1);//calcolo il punteggio per quella lettera (let è != da zero ovvero diversa da terminatore parola-->ultimo argomento funz =1 ,che mi significa che la lettera non è l ultima della parola
        if (direzione == 'o')  //direzione è variabile in ingresso alla funz.
        {
            return Inserimento(direzione,G,Criga,Ccolonna+1,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
        }                           //inserisco la lettera in griglia(se lettara gia giù in griglia--->utilizzo di ricorsione
        if (direzione == 'v')
        {
            return Inserimento(direzione,G,Criga+1,Ccolonna,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
        }
    }
    else    //se non c'è già una lettera posso inserire
    {
        do{   //lettera da inserire-->ciclo(e poi dopo ricorsione) Il caso base della ricorsione è quando metti il terminatore (0) e da li fa tutte le funzioni in cascata con la ricorsione
            if(scarabeo[Criga][Ccolonna] == '0')   //casella vuota->inserisco lettera
            {
                Stampa(scarabeo,G,lettere,griglia);
                //inserisci lettera
                cout<< "inserisci lettera (0 per finire) : ";
                cin>>let;
                //non metto nessun controllo per l'esistenza della lettera perchè verrà effettuato dopo

                if(let != '0') // se non ho finito
                {
                    NumLet++;  //nuova lettera,aumento conto lettere
                    if(let != '$') //se non è un jolly
                    {
                        if (G.ControlloTraLeTessere(let) == true) //controllo se esiste la lettera tra quelle disponibili
                        {
                            scarabeo[Criga][Ccolonna] = let;   //metto lettera nella griglia
                            if (controlloIntersezioni(scarabeo,Criga,Ccolonna,vocabolario,direzione) == true) //se posso mettere quella lettera in quella posizione
                            {       //(riga dopo)calcolo punteggio di quella lettera
                                G.CalcolaPunteggio(let,Ccolonna,Criga,griglia, 1);  //il numero "1" viene utilizzato solo per dire che la parola non è finita
                                if (direzione == 'o')
                                {
                                    return Inserimento(direzione,G,Criga,Ccolonna+1,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
                                }
                                if (direzione == 'v')
                                {
                                    return Inserimento(direzione,G,Criga+1,Ccolonna,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
                                }

                            }
                            else { corretto = false;}
                        }
                        else {corretto = false;} //manca lettera tra le tessere giocatore-->setto corretto(bool) a false
                    }
                    else //se è un jolly
                    {
                        if (G.ControlloTraLeTessere('$') == true) //se possiedo una jolly
                        {
                            let= Mettijolly(Ccolonna,Criga,false,'0');// zero e false mi permettono di dire che sto inserendo un jolly ma non nel suggerimento
                            scarabeo[Criga][Ccolonna] = let;
                            if (controlloIntersezioni(scarabeo,Criga,Ccolonna,vocabolario,direzione) == true)
                            {
                                //...calcolo il punteggio della lettera...
                                G.CalcolaPunteggio(let,Ccolonna,Criga,griglia, 1);  //il numero "1" viene utilizzato solo per dire che la parola non è finita
                                //...poi proseguo con la lettera successiva
                                Numjolly++;   //è stato usato un jolly->incremento numero di joly
                                if (direzione == 'o')
                                {
                                    return Inserimento(direzione,G,Criga,Ccolonna+1,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
                                }
                                if (direzione == 'v')
                                {
                                    return Inserimento(direzione,G,Criga+1,Ccolonna,vocabolario,scarabeo,griglia,Numerointersezioni,lettere,Numjolly,NumLet);
                                }
                            }
                            else { corretto = false;}   //no interse-->dà errore
                        }
                        else { corretto = false;}  //non possiedo un jolly->dà errore
                    }
                }
                else //se let==0, se ho quindi finito di inserire...
                {
                    if(Numerointersezioni == 0 || NumLet == 0) //se non ho messo lettere o se non ho fatto intersezioni
                    {return false;}
                    else
                    {
                        if(G.GetParolaCorr() == ""){ return false;} //se la parola è vuota(solo lo 0)-->error
                        if(ControlloEsistenzaParola(vocabolario,G.GetParolaCorr()) == true)// se la parola esiste
                        {
                            if(Numjolly == 0)
                            {
                                if(G.GetParolaCorr().size()>3) //lunghezza 2 o 3 da zero punteggio
                                G.NoJolly();  //visto che sono nell IF (numJolly=0) -->dare +10 punti nojolly
                            }
                            G.CalcolaPunteggio('0',0,0,griglia,0); //trova punteggioParola ("0" usato per dire che la parola è finita)
                            system("cls");
                            Stampa(scarabeo,G,lettere,griglia);
                            return true;  //andato a buon fine
                        }
                        else {return false;}   //se la parola non esiste
                    }

                }
            }

            if ( corretto == false)
            {
                //se qualcosa è falso
                //annulla turno per errori
                system("cls");
                cout<<"Mossa non valida! Utilizza un'altra lettera."<<endl;
                //ricomincia ciclo
                J=Jprova;
                Numerointersezioni = NumeroIntersezionitemp;
                Numjolly = NUmjollytemp;  //rimetto tutto = a prima(con le varibili temporanee che avevo salvato apposta)
                NumLet = NumLettemp;
                AnnullaTurnoPerErrori(scarabeo,TabellaProva,G,Gprova, griglia,grigliaProva);
            }
        }while(corretto == false);  //se sbagliato qualche input-->richiede l inserimento( dowhile)
        return corretto;
    }
    return corretto;
}
//----------------------
//----------------------
//----------------------
void InserisciSuggerimento (ParolaSugg suggerimento, char scarabeo [][17], int griglia[][17], Giocatore& G, size_t contatore){
    int Criga = suggerimento.Criga; //prendo la riga della struct suggerimento
    int Ccolonna = suggerimento.Ccolonna; //prendo la colonna della struct suggerimento

    char let; //lettera che sto considerando
    string parolasuggerita = suggerimento.parola; //prendo la parola suggerita dalla struct suggerimento

    int Cinizio = Ccolonna;  //Ccolonna e Criga sono la colonna d'inizio e la riga di inizio di inserzione del suggerimento
    int Rinizio = Criga;        //sono queste le variabili che userò
    while (contatore < parolasuggerita.size())  //scorro(con un contatore) tutta la parola suggerita(...    .size)
    {
        if(Criga>16 || Ccolonna>16 || Criga<0 || Ccolonna<0)
        {break;}//controllo di non uscire dalla tavola da gioco

        if(scarabeo[Criga][Ccolonna] != '0') //se c'è già una lettera in quelle coordinate
        {
            let = scarabeo[Criga][Ccolonna]; //la uso
            G.CalcolaPunteggio(let,Ccolonna,Criga,griglia,1);  //calcolo il punteggio su quella lettera
            contatore++; //vado alla lettera successiva
        }
        else //altrimenti...
        {
            if(scarabeo[Criga][Ccolonna] == '0') //se non c'è nessuna lettera in quella posizione
            {

                let = parolasuggerita[contatore];//accedo al carattere della parola suggerita utilizzando il contatore che poi incrementerò
                if(G.ControlloTraLeTessere(let) == false) //se  non ho la lettera potrebbe essere un jolly che ho in mano
                {
                    if(G.ControlloTraLeTessere('$') == true && suggerimento.numjolly > 0) //se la lettera è in realta un jolly
                    {   //suggerimento.numjolly>0 è un ulteriore controllo per jolly
                        Mettijolly(Ccolonna,Criga,true,let);//Bisogna gestire il jolly! la funzione mettijolly capirà che stiamo facendo un suggerimento e imposerà il valore del jolly = alla lettera corrente della parola suggerita
                        suggerimento.numjolly--;// un jolly gestito! --> faccio -1 (tolgo)
                    }
                    else {return;}//se arriva qua c'è un errore grave
                }
                scarabeo[Criga][Ccolonna] = let;  //in griglia metto la lettera della parola suggerita
                G.CalcolaPunteggio(let,Ccolonna,Criga,griglia,1);
                contatore++; //vado alla lettera successiva, quando cont=lettSugg.size    ho finito
            }
            else { return; } //se arriva qua c'èun errore
        }
        if(suggerimento.direzione == 'o')
        {
            Ccolonna++;  //[per prossima iterazione],colonne
        }
        if(suggerimento.direzione == 'v')
        {
            Criga++; //[per prossima iterazione], si andrà alla riga dopo se inserisco in verticale
        }
    }
    G.CalcolaPunteggio('0',Cinizio,Rinizio,griglia,0); //calcolo il punteggio della parola conclusa(parola suggerita)
    return;
}
//----
bool Suggerimento (Giocatore& G,const vector<string>& vocabolario, char scarabeo[][17],int griglia[][17], ParolaSugg& Paroladef, bool turnozero) {

    //Creo dei temporanei da usare in caso di errore
    int grigliaProva[17][17];
    CopiaGriglia(griglia,grigliaProva);
    char scarabeoProva [17][17];
    CopiaTavola(scarabeo,scarabeoProva);

    // dichiarazione variabili locali
    int NumInt;
    int Letmie;
    int numjolly;
    int a = Esistenzajolly();
    vector<char> lettereDisp;
    char direzione1 = 'o';
    char direzione2 = 'v';

    for(size_t l = 0; l< G.GetSize();l++) //inserisco nel vettore(locale) le lettere che posso utilizzare(del giocatore attuale)
    {
        lettereDisp.push_back(G.getMano(l));
    }

    if(turnozero==false) //se non è il primo turno
    {
        for(unsigned int r = 0; r<17; r++)   //scorro tutte le caselle della griglia
        {
            for(unsigned int c = 0;c<17; c++) //e vedo e calcolo il suggerimento da tutte le casella che danno PossibiliàSugg==true
            {

                if(PossibilitaSuggerimento(r,c,direzione1,scarabeo,lettereDisp.size()) == true) //se posso formare un suggerimento in verticale
                {// r e c--> da quella casella

                    NumInt = 0;  //numInt e letMie azzerarle sempre prima di far partire SuggRiCORSIVO(Le calcola lui)
                    Letmie = 0;
                    numjolly = a;  //la parola del Suggerimento verrà rimpiazzata ogni qualvolta che se ne trova una con punteggio maggiore
                    SuggerimentoRicorsivo(direzione1,r,c,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumInt,Letmie,numjolly); //funzione che forma la parola da suggerire ricorsivamente
                }
                if(PossibilitaSuggerimento(r,c,direzione2,scarabeo,lettereDisp.size()) == true) //se posso formare un suggerimento in orizzontale
                {
                    NumInt = 0;
                    Letmie = 0;  //ogni qualvolta che se ne trova una con punteggio maggiore, avviene il rimpiazzo(sia che sia in orizz sia che sia in verticale)
                    numjolly = a;
                    SuggerimentoRicorsivo(direzione2,r,c,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumInt,Letmie,numjolly);
                }

            }
        }
    }
    else //se turnozero== true
    {
        NumInt = 1; //parto già con un numero di intersezioni diverso da zero(Lo imposto io così,va bene così se è turnozero)
        Letmie = 0;
        numjolly = Esistenzajolly();                    //8,8 perchè è il centro della griglia
        SuggerimentoRicorsivo(direzione1,8,8,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumInt,Letmie,numjolly);
        NumInt = 1;  //risettare num int  e letmie perchè SuggRciorsivo le cambia
        Letmie = 0;
        numjolly = Esistenzajolly();
        SuggerimentoRicorsivo(direzione2,8,8,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumInt,Letmie,numjolly);
    }//chimarlo 2 volte(una per sugg in orizz e una per sugg in verticale), ma verrà tenuto solo quella col punteggio migliore
    //viene fatto per trovare il suggerimento migliore

    //per sicurezza risistemo grilgia e tavola
    CopiaGriglia(grigliaProva,griglia);
    CopiaTavola(scarabeoProva,scarabeo);

    if(Paroladef.punteggio == 0 || Paroladef.parola == "" || Paroladef.NumIntersezioni == 0)
    {//Se SuggRicorsivo non ha dato punteggio o non ha riempito la parola o la parola non fa intersezioni
        return false; //non esiste un suggerimento
    }
    else
    {
        return true; //esiste un suggerimento
    }
}
//-----
bool PossibilitaSuggerimento (int const Criga, int const Ccolonna, char direzione, char scarabeo[][17], size_t size) {
    size_t i;
    if (direzione == 'o')                  //size(in ingresso) dipende da G.getSize(grandezza del vettore lettere a disposizione del giocatore)
    {
        for(i = 0; i<size; i++) //controllo fino a 8 posizioni da dove mi trovo(se 8 son le tessere che ho in mano-->size(se no,meno di 8)
        {
            if((Ccolonna + i) <17 && Criga <17) //controllo di non uscire da griglia(colonna + i perchè in orizz son le colonne ad incrementare)
            {
                if(scarabeo[Criga][Ccolonna+i] != '0') //se c'è una lettera entro 8 posizioni
                {
                    return true; //posso formare un suggerimento
                }
            }
            else{return false;} //non posso formare un suggerimento
        }
        return false; //non posso formare un suggerimento
    }
    if (direzione == 'v')
    {
        for(i = 0; i<size; i++)
        {
            if((Criga + i) <17 && Ccolonna <17)
            {
                if(scarabeo[Criga + i][Ccolonna] != '0')
                {
                    return true; //posso formare un suggerimento
                }
            }
            else{return false;} //non posso formare un suggerimento
        }
        return false; //non posso formare un suggerimento
    }
    return false; //non posso fomare un suggerimento
}
//----
void SuggerimentoRicorsivo(char const direzione, int Criga, int Ccolonna, vector<char> lettereDisp, char scarabeo[][17],
                                int griglia[][17],const vector<string>& vocabolario, ParolaSugg& Paroladef, int& NumIntersezioni, int& Letteremie, int& numjolly){

    if(lettereDisp.size() <= 0){return;}//controllo di non aver finito le lettere,se si esco
    if(Criga>16 || Ccolonna>16 || Criga<0 || Ccolonna<0)
    {return;}//controllo di non uscire dalla tavola da gioco

    //creazione temporanei
    char scarabeoProva[17][17];
    int grigliaProva[17][17];
    CopiaTavola(scarabeo,scarabeoProva);
    CopiaGriglia(griglia,grigliaProva);

    char let;
    ParolaSugg temp;

    if (scarabeo[Criga][Ccolonna] != '0')  //se è gia presente una lettera
    {
        let = scarabeo[Criga][Ccolonna] ; //assegno a let il valore presente sulla tavola
        NumIntersezioni++;//se cè gia un alettera sulla tavola vuol dire che è di un altra parola che c'era gia -->è una lettera di intersezione con la mia parola
        //compongo la parola
        temp = ComponiParola(direzione,Criga, Ccolonna,scarabeo,griglia);//si sposta a inizio parola,la compone e la mette in parolaSugg temp e ne calcola il punteggio
        //verifico iniziali
        if(verificaIniziali(temp.parola,vocabolario) == true) //verifico che posso formare una parola con quello che c'è giù
        {                                                           //(che le lettere già giù sono lettere che potrebbero formare una parola di senso compiuto
            //se vero controllo se esiste veramente tutta la parola (il controllo precedente serve per rendere piu veloce il tutto)
            if(ControlloEsistenzaParola(vocabolario,temp.parola) == true)
            { //Se NON entri in questo if, rimane la parola Sugg precedente!!
                //se la parola esiste davvero controllo se è la migliore  (parola def è il suggerimento trovato alla chiamata di funzione (ricorsiva) precedente
                if( (temp.punteggio > Paroladef.punteggio) && NumIntersezioni >0 && Letteremie>0)
                {
                    Paroladef.parola = temp.parola;  //se è la migliore(e se fa intersez e se ho usato lettere dalla mano del gioc(lettermie)-->cambio tutti i parametri con questa attuale(la migliore)
                    Paroladef.punteggio = temp.punteggio;
                    Paroladef.Criga = temp.Criga;
                    Paroladef.Ccolonna = temp.Ccolonna;
                    Paroladef.direzione = direzione;
                    Paroladef.NumIntersezioni = NumIntersezioni;
                    Paroladef.numjolly = numjolly;

                }
            }
            //setto griglia e richiamo funz
            CopiaGriglia(grigliaProva,griglia); //mi serve per ricalcolare il punteggio alla parola successiva senza togliere i moltiplicatori
            if(direzione == 'o')                                             //mi serve quindi per mantenerli momentaneamente,poi verranno tolti e verrà usata la griglia dove i moltiplicatori usati son stati tolti
            {
                //richiamo la funzione
                SuggerimentoRicorsivo(direzione,Criga,Ccolonna + 1,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                NumIntersezioni--;     //richiami la stessa funzione per andare alla prossima lettera(e la funzione rifarà le stesse cose,ovvero userà la lettera(nuova,  +1) se la trova giù
                return;                 //per comporre la parola,la cercherà nel vocabolario...ecc
            }
            if( direzione == 'v')
            {
                //richiamo la funzione
                SuggerimentoRicorsivo(direzione,Criga + 1,Ccolonna,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                NumIntersezioni--;//qsto viene fatto solo quando lettera gia in griglia,per gestire la ricorsione,caso in cui essa fallisce e bisogna fare decremento al numIntersez
                return;                 //se finisci in qsto if o in quello precedente--> per forza arriverai a un return
            }
        }
        NumIntersezioni--;  //cado qui se verificsIniziali da esito false, il ++ al numero di intersez che avevi fatto prima devi annullarlo(--)
        //non copio la tavola perche rimane inalterata(in caso di annullamento)
        //sistemo la griglia
        CopiaGriglia(grigliaProva,griglia);
        return;
    }

    //se non è presente nessuna lettera in questa posizione
    for(unsigned int i=0; i<lettereDisp.size(); i++) //scorro le letterea mia disposizione(provarle tutte per trovare il suggerimento migliore)
    {
        let = lettereDisp[i];

        if(scarabeo[Criga][Ccolonna] == '0') //se posso mettere una lettera(casella vuota
        {
            if(let != '$') //se la lettera non è un jolly
            {
                scarabeo[Criga][Ccolonna] = let; //metto giu la lettera(giù ipoteticamente,la griglia con la lettera nuova non è ancora stampata pero')
                if ( controlloIntersezioni(scarabeo,Criga,Ccolonna,vocabolario,direzione) == true) //se posso mettere la lettera in questa posizione
                {
                    //compongo la parola
                    temp = ComponiParola(direzione,Criga, Ccolonna,scarabeo,griglia);
                    //verifico iniziali
                    if(verificaIniziali(temp.parola,vocabolario) == true) //verifico che posso formare una parola con quello che c'è giù
                    {
                        Letteremie ++; //cotatore delle lettere usate da me
                        //se vero controllo se esise veramente
                        if(ControlloEsistenzaParola(vocabolario,temp.parola) == true)
                        {
                            if( (temp.punteggio > Paroladef.punteggio) && NumIntersezioni >0 && Letteremie>0 )
                            {
                                Paroladef.parola = temp.parola;
                                Paroladef.punteggio = temp.punteggio;
                                Paroladef.Criga = temp.Criga;
                                Paroladef.Ccolonna = temp.Ccolonna;
                                Paroladef.direzione = direzione;
                                Paroladef.NumIntersezioni = NumIntersezioni;
                                Paroladef.numjolly = numjolly;

                            }
                        }
                        //setto griglia e richiamo funz
                        CopiaGriglia(grigliaProva,griglia); //mi serve per ricalcolare il punteggio alla parola successiva,mi servirà quindi la vecchia griglia

                        lettereDisp.erase(lettereDisp.begin()+i); // tolgo la lettera appena usata
                        //incremento in base alla direzione di inserimento
                        if(direzione == 'o')
                        {
                            //richiamo la funzione
                            SuggerimentoRicorsivo(direzione,Criga,Ccolonna + 1,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                        }
                        if( direzione == 'v')
                        {
                            //richiamo la funzione
                            SuggerimentoRicorsivo(direzione,Criga + 1,Ccolonna,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                        }
                        lettereDisp.insert(lettereDisp.begin()+i,let); //inserisco di nuovo la mia lettera
                        Letteremie--;
                    }
                    //se la parola non va bene ricopio la tavola per togliere la lettera
                }
                //sistemo la tavola
                CopiaTavola(scarabeoProva,scarabeo);
                //sistemo la griglia
                CopiaGriglia(grigliaProva,griglia);
            }
            else // se la lettera è un jolly
            {
                if(numjolly<2 && numjolly>=0) //se posso usare un jolly
                {

                    for(let='a'; let<='z'; let++) //scorro tutte le lettere dalla a alla z(provarle tutte)(XCHè HO UN JOLLY (SUGGERIMENTO)!!!)
                    {
                        if(let!='j' && let!='k' && let!='w' && let!='x' && let!='y') //evitando quelle straniere
                        {
                            scarabeo[Criga][Ccolonna] = let;  //prendo la lettera corrente
                            if ( controlloIntersezioni(scarabeo,Criga,Ccolonna,vocabolario,direzione) == true) //deve dare true per poter inserire la lettera nella posizione data
                            {
                                //compongo la parola
                                temp = ComponiParola(direzione,Criga, Ccolonna,scarabeo,griglia);
                                //verifico iniziali
                                if(verificaIniziali(temp.parola,vocabolario) == true) //verifico che posso formare una parola con quello che c'è giù(se no,esco (return),esco prorpio dalla funz SuggRicorsivo
                                {
                                    numjolly++; //aumento il numero di jolly in gioco(perchè sono nell if del jolly)
                                    //se vero controllo se esiste veramente
                                    if(ControlloEsistenzaParola(vocabolario,temp.parola) == true)
                                    {
                                        if( (temp.punteggio > Paroladef.punteggio) && NumIntersezioni >0 && Letteremie>0 )
                                        {
                                            Paroladef.parola = temp.parola;
                                            Paroladef.punteggio = temp.punteggio;
                                            Paroladef.Criga = temp.Criga;
                                            Paroladef.Ccolonna = temp.Ccolonna;
                                            Paroladef.direzione = direzione;
                                            Paroladef.NumIntersezioni = NumIntersezioni;
                                            Paroladef.numjolly = numjolly;
                                        }
                                    }
                                    //setto griglia e richiamo funz
                                    CopiaGriglia(grigliaProva,griglia); //mi serve per ricalcolare il punteggio alla parola successiva

                                    lettereDisp.erase(lettereDisp.begin()+i); // tolgo la lettera appena usata
                                    //poi(sotto) , incremento in base alla direzione di inserimento
                                    if(direzione == 'o')
                                    {
                                        //richiamo la funzione
                                        SuggerimentoRicorsivo(direzione,Criga,Ccolonna + 1,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                                    }
                                    if( direzione == 'v')
                                    {
                                        //richiamo la funzione
                                        SuggerimentoRicorsivo(direzione,Criga + 1,Ccolonna,lettereDisp,scarabeo,griglia,vocabolario,Paroladef,NumIntersezioni,Letteremie,numjolly);
                                    }
                                    //Toglijollysug(Criga,Ccolonna); //metto jolly a false
                                    lettereDisp.insert(lettereDisp.begin()+i,'$'); //inserisco di nuovo la mia lettera
                                    numjolly--;
                                }
                                //se la parola non va bene ricopio la tavola per togliere la lettera
                            }
                            //sistemo la tavola (arriva qua se non puoi fare intersez
                            CopiaTavola(scarabeoProva,scarabeo);
                            //sistemo la griglia
                            CopiaGriglia(grigliaProva,griglia);

                        }
                    }
                }
                else   //se non ho jolly
                {return;} // se la funzione arriva qua c'è qualche errore
            }
        }
        else
        {return;} // se la funzione arriva qua c'è qualche errore
    }
    return; //fine
}
//----
ParolaSugg ComponiParola (char direzione, int Criga, int Ccolonna, char scarabeo [][17], int griglia[][17]){
    //comporre la parola data in suggerimento
    ParolaSugg word;

    if(Criga>16 || Ccolonna>16 || Criga<0 || Ccolonna<0)
    {return word;} //se entro qua c'è qualche errore. In ogni caso si ritorna una parola inizializzata per default(struct inizializzata con parametri di deafult)

    //dichiarazione variabili locali
    string parolaFormata;
    Punt temp;  //oggetto punteggio temporaneo per parola corrente
    char let;
    int i;  //per scorrere

    CoordinateInizio(direzione,Criga,Ccolonna,scarabeo); //mi sposto all'inizio della parola
    if(direzione == 'o') //se la direzione è orizzontale
    {
        int ColonnaIniziale = Ccolonna;  //Ccolonna contiene la coordinata colonna inizio
        i = 0;
        while((Ccolonna+i)<17 && scarabeo[Criga][Ccolonna+i] != '0' ) //se la casella non è vuota e Non sforo le colonne
        {
            let = scarabeo[Criga][Ccolonna+i];  //la lettera è la lettera gia giu in griglia
            temp.FunzPunt(let,Ccolonna+i,Criga,griglia,1); //calcolo punteggio su quella lettera e compone la parola(funzPunt)
            if((Ccolonna+i) == 16)
            {
                break;   //questa lettera era al limite della griglia->esco dal ciclo
            }
            i++; //prossima lettera
        }
        parolaFormata = temp.GetParolaCorr(); //formo la parola
        temp.FunzPunt('0',Ccolonna,Criga,griglia,0); //calcolo punteggio totale(ho tutte le lettere)

        //salvo le caratteristiche
        word.punteggio = temp.GetPuntTot();  //punteggio parola (tutta la parola)
        word.parola = parolaFormata;  //salvo la parola formata nel campo parola della struct  parolaSugg
        word.Criga = Criga;
        word.Ccolonna = ColonnaIniziale;// colonna inziale l avevi salvata prima(prima del ciclo  ...+i)

        return word;
    }
    if(direzione == 'v') //se la direzione è verticale  -->(uguale ma in verticale)
    {
        int Rigainiziale = Criga;
        i=0;
        while((Criga+i)<17 && scarabeo[Criga+i][Ccolonna] != '0')       //da sistemare
        {
            let = scarabeo[Criga+i][Ccolonna];
            temp.FunzPunt(let,Ccolonna,Criga+i,griglia,1);
            if((Criga+i) == 16)
            {
                break;
            }
            i++;
        }
        parolaFormata = temp.GetParolaCorr(); //formo la parola
        temp.FunzPunt('0',Ccolonna,Criga,griglia,0); //calcolo punteggio totale

        word.punteggio = temp.GetPuntTot();
        word.parola = parolaFormata;
        word.Criga = Rigainiziale;
        word.Ccolonna = Ccolonna;

        return word;
    }
    else return word;
}
//----
bool inizialiParola(string parolaTot, string parolaParz){//controlla se parola parziale è iniziale di una parola totale

    if(parolaParz.size()>parolaTot.size())     //IL CICLO per scorrere tutto il dizionario è nella funzione seguente a questa
        return false;
    for(size_t i=0; i<parolaParz.size(); ++i){
        if(parolaTot[i]!=parolaParz[i])
            return false; //se c'è almeno una lettera diversa ritornami falso
    }
    return true;  //altrimenti ritorni true,la parola parziale è sottoparola della parola totale
}

bool verificaIniziali(string iniz, const vector<string>& dizionario){ //solo inziali-->controllo più veloce
    int beg=0;
    int end=dizionario.size();
    while(beg<=end){//controllo se esiste almeno una parola del dizionario che abbia le iniziali passate per parametro
        int mid=(beg+end)/2;
        if(inizialiParola(dizionario[mid], iniz) == true)
        {//richiamo funzione di controllo
            return true;
        }
        else
        {
            if(iniz<dizionario[mid])
            {
                end=mid-1;  //guardo la prima metà(la fine diventa la metà-1
            }
            else
            {
                beg=mid+1;  //altrimento prendo in considerazion là metà dopo
            }                   //+1 e -1 perchè quell elemento va scartato dato che cadrebbe nel ramo true dell if
        }
    }
    return false;
}
//---------------
//---------------
//---------------
//funzioni jolly

int Esistenzajolly (){  //controlla se sono presenti jolly e ritorna quanti sono stati posizionati

    if(J.posizionato == false && J.posizionato2 ==false){
        return 0;
    }
    else if(J.posizionato==true && J.posizionato2==false){
        return 1;
    }
    else if(J.posizionato==false && J.posizionato2==true){
        return 1;
    }
    else{
        return 2;
    }
}

void Prendijolly ( Giocatore & G, char scarabeo[][17], int i){ //i variabile associata a funzione che viene dopo(piu in basso):serve per sapere a quale jolly(in qsta funzione) riferirsi(jolly 1 o jolly2)
    if(i==0)  //Prendijolly con i=0  viene chiamata da void JOLLY quando cè 1 jolly in campo
    {//ControlloTraLeTessere ha 2 compiti: 1)cercare se cè la tessera nel vettore del Gioc  ed  2)eliminarla
        if (G.ControlloTraLeTessere(J.val)==true) //se il giocatore ha una lettera(= al valore del jolly) in mano
        {
            G.SostituisciLettera(); //mette il simbolo $ (con un vector pushback) nella mano del giocatore corrente (G)
            scarabeo[J.cordR][J.cordC]=J.val;  //il valore della lettera in griglia rimane quello(rimane lo stesso)
            J.val = '0';
            J.cordC=-1;  //riazzero,riinizializzo i parametri del jolly,perchè il giocatore lo pesca e lo potrà usare come vuole
            J.cordR=-1;
            J.posizionato=false;
            return;  //esce da prendijolli (entra in uni solo degli if!!! perchè uno solo è il jolly che puoi prendere(o il primo o il secondo)!! (nel caso i=0)
        }
        if (G.ControlloTraLeTessere(J.val2)==true)  //stessa cosa accade per il 2^ jolly
        {
            G.SostituisciLettera();
            scarabeo[J.cordR2][J.cordC2]=J.val2;
            J.val2 = '0';
            J.cordC2=-1;
            J.cordR2=-1;
            J.posizionato2=false;
            return;
        }
        return;
    }
    if(i==1)
    {//in questo caso(prendijolly con i=1) puoi prendere solo il primo jolly
        if (G.ControlloTraLeTessere(J.val)==true)   //ma tanto nella funzione jolly, verrà chiamato predijolly con i=1 prima,e poi anche predijolly con i=2(nel caso in cui puoi prendere 2 jolly-->richiami la funzione 2 volte,una volta per ogni caso)
        {
            G.SostituisciLettera();
            scarabeo[J.cordR][J.cordC]=J.val;
            J.val = '0';
            J.cordC=-1;
            J.cordR=-1;
            J.posizionato=false;
            return;
        }
        cout<<"Il jolly 1 non può essere sostituito"<<endl; //se arrivo qui,vuol dire che controlloTraLeTessere ha dato esito false-->non puoi prender il jolly
        return;
    }
    if(i==2)  //in questo caso(con i=2 ) puoi prendere solo il secondo jolly
    {
        if (G.ControlloTraLeTessere(J.val2)==true)
        {
            G.SostituisciLettera();
            scarabeo[J.cordR2][J.cordC2]=J.val2;
            J.val2 = '0';
            J.cordC2=-1;
            J.cordR2=-1;
            J.posizionato2=false;
            return;
        }
        cout<<"Il jolly 2 non può essere sostituito"<<endl;
        return;
    }

}

void JOLLY( Giocatore & G, char scarabeo[][17]){  //serve per tirar su il jolly
    int i;
    i=Esistenzajolly();  //i può essere 0, 1 o 2
    bool vero= false;
    for( size_t temp = 0; temp<G.GetSize(); temp++)   //scorre tutte le lettere del gioc (getsize->numero di lettere che ha il giocatore)
    {
        if(G.getMano(temp) == J.val || G.getMano(temp) == J.val2 ) //controllo se nella mano del giocatore corrente cè il jolly1 o il jolly 2
        {
            vero = true;
            break;  //se trovato imposto vero a true e esco dal for
        }
    }

    if(i==0 || vero == false){ return; } //se nessun jolly è posizionato(i=0) o se non ho le lettere giuste(vero=false), non fare nulla,esce dalla funzione e basta
    else  //altrimenti( se cè un jolly in campo e in mano il giocatore ha una tessera con la stessa lettera del jolly...
    {
        if(i==1)  //se cè solo un jolly
        {
            char temp;
            do{
                cout<<"Desideri prendere il jolly?(si:s - no:n) ";
                cin>>temp;
            }while(temp != 's' && temp != 'n');
            if (temp== 'n'){
                return;  //esco ,non voglio prenderlo
            }
            else
            {
                Prendijolly(G,scarabeo,0);  //chiamo funz prendijolly e poi ,dopo,termino( int i=0 in prendijolly indicherà che cè un solo jolly in campo(int i ha significato diverso da questa funzione e la precedente!!!)
            }
            return;

        }
        if(i==2) // se i jolly in campo sono 2...
        {
            char temp;
            do{
                cout<<"Desideri prendere il jolly?(si:s - no:n) ";
                cin>>temp;
            }while(temp != 's' && temp != 'n');
            if(temp=='n')
            {
                return;
            }
            else
            {
                Prendijolly(G,scarabeo,1);  //chiami 2 volte la funzione prendijolly perchè i jolly sono 2(la 1 per il primo jolly,la 2 per il secondo jolly)
                Prendijolly(G,scarabeo,2);

            }//sarà la funzione prendijolly che effettivamente controllerà se il giocatore può prendere il jolly e che,nel caso,sarà sempre lei a effettuare lo scambio
            return;
        }
    }
}

char Mettijolly ( int CoorC, int CoorR,bool Presenzasugg, char let){
    char ValJ;
    if(J.posizionato==false) //se posso mettere giu il jolly(ovvero se ce l ho in mano,non è posizionato)
    {
        if(Presenzasugg == false) // se non sto facendo un suggerimento,è l utente a decidere il valore del jolly
        {
            do{
                cout<<"Inserisci il valore del jolly: ";
                cin>>ValJ;
            }while(ValJ < 'a' || ValJ > 'z'); //finchè non inserisce un valore corretto
        }
        else // se sto facendo un suggerimento
        {
            ValJ = let;  //la variabile ValJ è ora uguale al valore let passato a questa funzione
        }
        J.val=ValJ;  //valore del jolly = a valj
        J.posizionato=true;
        J.cordC=CoorC;  //imposto le coordinate del jolly = a quelle a cui gli erano state passate a questa funzione
        J.cordR=CoorR;
        return ValJ;  //ritorna il valore della lettera che assumerà il jolly
    }
    if(J.posizionato2==false)
    {
        if(Presenzasugg == false)
        {
            do{
                cout<<"Inserisci il valore del jolly: ";
                cin>>ValJ;
            }while(ValJ < 'a' || ValJ > 'z');
        }
        else
        {
            ValJ = let;
        }
        J.val2=ValJ;
        J.posizionato2=true;
        J.cordC2=CoorC;
        J.cordR2=CoorR;
        return ValJ;
    }
    return let;

}

void StampaJolly(){  //riquadrino che stampa lo stato dei jolly durante la partita

    cout<<" jolly 1 : ";
    if(J.posizionato==false)
    {
        cout<<"vuoto"<<endl;
    }
    else
    {
        cout<<"valore : "<<J.val<<"   ";  //se posizionato->indichi valore e coordinate
        cout<<"coordinate  R: "<<J.cordR<<"   C: "<<J.cordC<<endl;
    }

    cout<<" jolly 2 : ";  //stessa cosa per secondo jolly
    if(J.posizionato2==false)
    {
        cout<<"vuoto"<<endl;
    }
    else
    {
        cout<<"valore : "<<J.val2<<"   ";
        cout<<"cordinate  R: "<<J.cordR2<<"   C: "<<J.cordC2<<endl;
    }
    return;
}
//---------------------
//---------------------
//---------------------
//------------------------------------------------------ funzione di stampa
void SetColor(short Color)
{   //handle per la stampa
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); // oppure system("COLOR E9");    handle-->gestore, per gestire le proprietà dell output
    SetConsoleTextAttribute(hCon,Color); //funzioni della libreria windows.h                                      colore in questo caso
}

void Colora(int griglia[][17],int k,int t){   //colora caselle della griglia in base ai moltiplicatori

    if(griglia[k][t]== 2)
    {
        SetColor(10);  //verde chiaro(moltiplic 2 * la parola)
    }
    else
    {
        if(griglia[k][t]== 3)
        {
            SetColor(11); //azzurro( 3 * la parola)
        }
        else
        {
            if(griglia[k][t]== 4)
            {
                SetColor(12);  //rosso(moltiplicatore 2 * la lettera)
            }
            else
            {
                if(griglia[k][t]== 5)
                {
                    SetColor(14); //giallo( 3 per la lettera)
                }
                else
                {
                    SetColor(7); //tutte le altre caselle -->bianche
                }
            }
        }
    }
    return;
}

void Stampa (char scarabeo [][17], Giocatore const& G, vector<char>& lettere, int griglia[][17]){
    SetColor(7);

    cout<<" Lettere rimanenti : "<<"["<<lettere.size()<<"]"<<endl<<endl;  //Stampa il numero di lettere ancora che puoi pescare
    //----------
    cout<<"    ";
    for(int s=0; s<17;s++){
        cout<<(s>=10?"  ":"   ")<<s;
    }
    cout<<endl;

    cout<<"      ";
    for(int u= 0; u<17; u++)
    {
        cout<<"__  ";  //parte inferiore della casella
    }
    cout<<endl;

    for(int k =0;k<17;k++){          //STAMPA LA TAVOLA DA GIOCO
        cout<<" "<<k<<"  ";// 2 spazi
        if(k<10)
        {
            cout<<" ";//1 spazio
        }

        for(int t =0; t<17;t++){
            Colora(griglia,k,t);
            cout<<"|";
            //-------------
            if(    scarabeo[k][t]=='0'    )  //se matrice scarabeo =0 ->ovvero,vuota
            {
                Colora(griglia,k,t);
                cout<<"__|";  //stampo il vuoto
                SetColor(7);
            }
            else //altrimenti, se non è vuota
            {
                if( (t==J.cordC && k==J.cordR)|| (t==J.cordC2 && k==J.cordR2)) //se quella casella e la casella con coordinate = a quelle di uno dei due jolly
                {
                    SetColor(15);
                    cout<<"$";  //stampo il carattere jolly
                    SetColor(7);   //il colore dell caselle(se magari era colorato) quando  prendi un bonus viene messo bianco
                    cout<<"_|";
                }
                else
                {
                    SetColor(15);
                    cout<<scarabeo[k][t];   //se non è jolly,stampo la ettera in quella casella
                    SetColor(7);
                    cout<<"_|";
                }

            }
        }//in righe diverse...(k)
        if( k == 3){cout<<"               Moltiplicatori(bonus punteggio)";}
        if( k == 4){cout<<"               "; SetColor(12); cout<<"rosso   : "; SetColor(7); cout<<" 2 * lettera";}
        if( k == 5){cout<<"               "; SetColor(14); cout<<"giallo  : "; SetColor(7); cout<<" 3 * lettera";}
        if( k == 6){cout<<"               "; SetColor(10); cout<<"verde   : "; SetColor(7); cout<<" 2 * parola";}
        if( k == 7){cout<<"               "; SetColor(11); cout<<"azzurro : "; SetColor(7); cout<<" 3 * parola";}
        cout<<endl;
    }
    //-----
    cout<<endl<<endl;
    StampaJolly(); //stampa i jolly coordinate e valore(se è in campo)
    cout<<endl;
    Print(G);

    return;
}

//------------------------------------------ funzioni per creare e controllare il dizionario

void FunzCreaDizionario( vector<string>& vocabolario){


    ifstream file("parole.txt");   //creo variabile ifstream, che legge da parole.txt (o parole.dic dipende dall'estensione)
                                   //il file dizionario deve stare nella stessa cartella di progetto QT del file main.cpp
    if(file.fail())                //se c'è un errore nel caricamento del file
        cout<<"ERRORE  NEL CARICAMENTO DEL FILE DIZIONARIO";
    else
    {
        string temp;        //variabile temporanea per la lettura parola per parola
        while(file>>temp)   //finché c'è una parola nel file(mette anche la parola presa dal file in temp)
        {
            vocabolario.push_back(temp);  //la aggiungo al vocabolario
        }
    }//ora nel vector vocabolario ho tutto il dizionario

}

bool ricercaBinariaDizionario(const vector<string>& vocabolario, string parola){ //controlla se la parola esiste nel dizionario
  //binarySearch--> 1)il vettore vocabolario è gia ordinato
    int beg=0; //variabile per inizio ,uguale a 0
    int end=vocabolario.size();   //variabile per fine(= lunghezza totale vettore vocabolario)
    int mid;
    while(beg<=end){
        mid=(beg+end)/2;  //variabile mid la impongo = all'indice di  metà del vettore
        if(parola<vocabolario[mid])  //restringo campo di ricerca alla metà di sinistra(restringo il vettore)
        {                            //sarà sicuramente nella metà di sx perchè il vettore è ordinato e perchè parola è minore di vocab[mid]
            end=mid-1; //-1 perchè la parola vocabolario[mid] è stata appena controllata e parola è < di essa(NON uguale,quindi la scarto)
        }
        else
        {
            if(parola>vocabolario[mid])
            {
                beg=mid+1;  //restringo campo di ricerca alla metà di destra
            }
            else {return true;} //se arrivo qui, non è nè minore nè maggiore(quindi è =) -->parola trovata
        }

    }
    return false; //se giungo qui,begin ha superato end-->Non cè la parola string parola

}
//---------------------------------------------------------------------------- controllo esistenza
bool ControlloEsistenzaParola (const vector<string>&vocabolario, string ParolaCorrente ){

    if( ParolaCorrente.size()<= 3)
    {
        return true; //se la parola ha meno o = di tre lettere, non controllo neanche se esiste--> va bene
    }
    else
    {
        bool c = ricercaBinariaDizionario(vocabolario, ParolaCorrente);  //la cerco nel dizionario
        return c;
    }
}
//--------------------------------------------------------------------------- copia e annulla
void CopiaTavola (char Scarabeo[][17], char Scarabeoprova [][17])  //gli passi la futura griglia di prova(scarabeoProva) e la griglia attuale(scarabeo)
{
    for(int k =0;k<17;k++){
        for(int t =0; t<17;t++){
            Scarabeoprova[k][t]=Scarabeo[k][t];
        }
    }
}

void CopiaGriglia (int Griglia[][17], int Grigliaprova [][17]) //copia la griglia in una nuova variabile chiamata Grigliaprova
{                                                               //così potrò usarla per tornare indietro in caso di aventuali errori(nell inseriemnto...)
    for(int k =0;k<17;k++){
        for(int t =0; t<17;t++){
            Grigliaprova[k][t]=Griglia[k][t];
        }
    }
}

void CreaManoProva (char scarabeo [][17], char TabellaProva[][17],Giocatore& G, Giocatore& Gprova , int griglia[][17], int grigliaProva [][17]){
//funzione per creare mano prova per giocatori diversi(infatti,gli passi l'oggetto giocatore,lo fai per ogni giocatore diverso)
    CopiaTavola(scarabeo,TabellaProva);      //la mano è di prova perchè se ci sono errori poi puoi tornare indietro
    CopiaGriglia(griglia,grigliaProva);

    Gprova = G.CreaManoProva();  //G.creamanoProva crea una copia del giocatore corrente e la ritorna(questa copia quindi andà dentro al left operand,ovvero Gprova)

}

void AnnullaTurnoPerErrori (char scarabeo [][17], char TabellaProva[][17], Giocatore& G, Giocatore& Gprova, int griglia[][17], int grigliaProva [][17]){

    CopiaTavola(TabellaProva,scarabeo);//tutto al contrario!  annullo!!
    CopiaGriglia(grigliaProva,griglia);  //per saper moltiplicatori gia usati
    G = Gprova; //Gprova te lo salvi prima di ogni operazione(o turno), e,qua, rimetti il giocatore G = a Gprova(ritorna = a prima) visto l'annullamento/errore
    return;
}

//------------------------------------------------- funzione per spostarsi alla prima lettera diversa da zero (vuota)
void CoordinateInizio(char DirezioneInserimento, int& CRiga, int& CColonna, char Scarabeo [][17]) { //per fare ciò,passo per parametro criga e ccolonna(che verranno modificati)
    if(CRiga>16 || CColonna>16 || CRiga<0 || CColonna<0)//se son fuori dalla griglia->esco dalla funzione
    {return;}
    bool fine = false;//per controllo se la parola è finita
    if ( DirezioneInserimento == 'o')
    {                                   //orizzontale
        while( fine == false)
        {  //partire da Ccolonna -1 perchè parola può anche essere lunga 2( è anche il minimo la lunghezza 2->quindi indietro di almeno una volta devo farlo per forza->lo scrivo subito->-1
            if(CColonna > 0 && CColonna< 17 && Scarabeo[CRiga][CColonna - 1] != '0')//se rimango dentro alla griglia e se nella griglia NON trovo 0(che vuol dire vuoto, ovvero parola finita!)
                CColonna -- ;  //decremento perchè devo andare a ritroso(indietro) per trovar l'inizio della parola
            else                //altrimenti(se cado qui)->vuol dire che la parola è finita(o ho finito la griglia(bordi)
                fine = true;// e il while Esce
        }
        return;
    }
    if (DirezioneInserimento == 'v') //stessa cosa ma per direzione verticale,quindi decrementerò la colonna
    {
        while( fine == false)
        {
            if(CRiga > 0 && CRiga < 17 && Scarabeo[CRiga-1][CColonna] != '0')
            {
                CRiga -- ;
            }
            else
                fine = true;
        }
        return;
    }
}
//--------------------------------------------------------------------------------------- controllo se posso mettere la lettera guardando le intersezioni

bool controlloIntersezioni (char scarabeo[][17], int coordinatariga , int coordinatacolonna,
                                const vector<string>&vocabolario,char parola){    //controllo se posso inserire la lettera nella posizione data

    if(coordinatariga>16 || coordinatacolonna>16 || coordinatariga<0 || coordinatacolonna<0)
    {return false;}  //sei fuori da limiti griglia
    int i = 0;
    string word = "";
    char let;
    if(parola == 'v')   //char parola(passata in ingresso) indica se la parola è verticale(v) o orizzontale(o)
    {//v (verticale) -->il controllo che una parola interseca sarà orizzontale(almeno una parola in orizzontale deve intersecare la parola che sto mettendo in verticale)
        CoordinateInizio('o',coordinatariga,coordinatacolonna,scarabeo);
        i = 0;  //coordinateInizio imposta le coordinate all'inizio della parola, perchè per ogni lettera della parola dovrò controllare se vi è una intersezione!Sarà poi compito del ciclo che viene dopo ,in questa funzione (ControlloIntersez), di spostarsi avanti e vedere se ci sono intersezioni lettera per lettera
        while((coordinatacolonna + i)<17 && scarabeo[coordinatariga][coordinatacolonna+i] != '0' ) //finchè la colonna dopo(+i,  che va a trovare le lettere nuove della parola orzzontale) è <17 e, sempre la colonna dopo,non è vuota,cè una lettera(!=0)
        {
            if((coordinatacolonna+i)>16 )//se cordColonna attuale +1,2...è > di 16(Fuori griglia) non possono esserci intersezioni
            {
                return false;  //controllo intersez ritorna false
            }
            let = scarabeo[coordinatariga][coordinatacolonna+i];  //in char let metto la lettera della parola orizzontale che deve intersecare(orizontale,quindi incremento le colonne)
            word += let; //in string word vado a formare la parola orizzontale che dovrebbe intersecare la mia parola verticale(poi bisogna controllare nel dizionario se esiste la parola che si forma)
            i++;  //incremento i per vedere le lettere piu avanti della parola orizzontale che deve intersecare la mia parola
        }
    }
    else
    {//stessa cosa ma per inserimenti orizzontali(o) -->il controllo che una parola intersecchi sarà verticale(almeno una parola in verticale deve intersecare la parola che sto inserendo, in orizzontale)
        if(parola == 'o')
        {
            CoordinateInizio('v',coordinatariga,coordinatacolonna,scarabeo);
            i = 0;
            while((coordinatariga + i)<17 && scarabeo[coordinatariga+i][coordinatacolonna] != '0')
            {
                if((coordinatariga+i)>16 )
                {
                    return false;
                }
                let = scarabeo[coordinatariga+i][coordinatacolonna];
                word += let;
                i++;
            }
        }
    }
    if(word.size() < 4)  //se la parola che si forma ha lunghezza minore o = a 3 ,va bene anche se NoN è di senso compiuto...
    {
        return true;  //... e quindi ritorno true
    }
    else
    {//se no,bisogna cercare se cè nel dizionario
        return ricercaBinariaDizionario(vocabolario,word); //ricBinDizionario ritornerà true se la trova ,false altrimenti
    }

}

//--------------------------------------------------------------------------- crea le lettere per il gioco

void creaLettere( vector<char>& lettere )
{

    srand(time(0));  //ogni volta che viene chiamata creaLettere viene settato il seme generatore di numeri casuali a un altro numero
    // (viene settato in base all'ora,in millisecondi). Usato dopo nella random_shuffle(overload della funzione random_shuffle)
    //per evitare di mischiare il vector partendo sempre dallo stesso elemento(uscirebbe sempre lo stesso risultato).


    int i = 0;
    for( i = 0; i < 12;i++)
    {
        lettere.push_back('a');
        lettere.push_back('e');
        lettere.push_back('i');
        lettere.push_back('o');
    }
    for( i = 0; i < 7;i++)
    {
        lettere.push_back('c');
        lettere.push_back('r');
        lettere.push_back('s');
        lettere.push_back('t');
    }
    for( i = 0; i < 6;i++)
    {
        lettere.push_back('l');
        lettere.push_back('m');
        lettere.push_back('n');
    }
    for( i = 0; i < 4;i++)
    {
        lettere.push_back('b');
        lettere.push_back('d');
        lettere.push_back('p');
        lettere.push_back('f');
        lettere.push_back('g');
        lettere.push_back('v');
        lettere.push_back('u');
    }
    for( i = 0; i < 2;i++)
    {
        lettere.push_back('h');
        lettere.push_back('q');
        lettere.push_back('z');
        lettere.push_back('$');  //caselle jolly (scarabeo)

    }


    random_shuffle( lettere.begin(), lettere.end());  //funzione già definita contenuta nella libreria algorithm,gli si forniscono come parametro l'inizio del vector e la fine,ma mischierà il vector sempre allo stesso modo
    // Qui viene usato l'overload della funzione random_shuffle che riconosce e usa un terzo paramentro,il generatore di numeri casuale.
    //Con il generatore di numeri casuali,la random_shuffle inizierà a mischiare il vector partendo ogni volta da una posizione diversa del vector
}
