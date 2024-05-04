#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

void cargarCadena(char *pal, int tam)
{
    int i;
    fflush (stdin); ///limpia el buffer de entrada para que la carga se haga sin caracteres que hayan quedado sin usar
    for(i=0; i<tam; i++)
    {
        pal[i]=cin.get();
        if(pal[i]=='\n')break;
    }
    pal[i]='\0';
    fflush(stdin); ///vuelve a limpiar el buffer para eliminar los caracteres sobrantes
}
void altaTarjeta();
void bajaLogicaTarjeta();
void modificarSaldoTarjeta();

class Fecha{
    private:
        int dia;
        int mes;
        int anio;
    public:
        Fecha(int d=1, int m=1, int a=1900){
            dia=d;
            mes=m;
            anio=a;
        }
        void setDia(int d){
            if(d>0 && d<32){
                dia=d;
            }
        }
        void setMes(int m){mes=m;}
        void setAnio(int a){anio=a;}
        void setearFecha(int d, int m, int a){
            dia=d;
            mes=m;
            anio=a;
        }
        int getDia(){return dia;}
        int getMes(){return mes;}
        int getAnio(){return anio;}
        void Cargar();
        void Mostrar(){
            cout<<dia<<"/"<<mes<<"/"<<anio;
        }
};

void Fecha::Cargar()
{
    cout<<"DIA: ";
    cin>>dia;
    cout<<"MES: ";
    cin>>mes;
    cout<<"ANIO: ";
    cin>>anio;
}
///ACA EMPIEZA LO RELACIONADO A TARJETA
class Tarjeta
{
    private:
        int NroTarjeta;
        Fecha FechaDeAlta;
        int Dni;
        float Saldo;
        bool Estado;
    public:
        Tarjeta(int NroTarjeta = 0, Fecha f = Fecha(), int Dni = 0, float Saldo = 0, bool Estado = true)
        {
            NroTarjeta = 0;
            FechaDeAlta = f;
            Dni = 0;
            Saldo = 0;
            Estado = true;
        }

    ///SETTERS
        void setNroTarjeta(int nroTarjeta){NroTarjeta = nroTarjeta;}
        void setFechaDeAlta(Fecha fechaDeAlta){FechaDeAlta = fechaDeAlta;}
        void setDni(int dni){Dni = dni;}
        void setSaldo(float saldo){Saldo = saldo;}
        void setEstado(bool estado){Estado = estado;}

    ///GETTERS
        int getNroTarjeta(){return NroTarjeta;}
        Fecha getFechaDeAlta(){return FechaDeAlta;}
        int getDni(){return Dni;}
        float getSaldo(){return Saldo;}
        bool getEstado(){return Estado;}

    ///METODOS
        void Cargar();
        void Mostrar();
};

void Tarjeta::Cargar()
{
    cout << "NUMERO DE TARJETA: ";
    cin >> NroTarjeta;
    if(NroTarjeta < 1)
    {
        return;
    }
    cout << "FECHA DE ALTA DE LA TARJETA: " << endl;
    FechaDeAlta.Cargar();
    cout << "INGRESE DNI DEL DUEÑO DE LA TARJETA: ";
    cin >> Dni;
    cout << "INGRESE SALDO DE LA TARJETA: ";
    cin >> Saldo;
}

void Tarjeta::Mostrar()
{
    if(Estado == true)
    {
        cout<<"NUMERO DE TARJETA: "<< NroTarjeta << endl;
        FechaDeAlta.Mostrar();
        cout << endl;
        cout<<"DNI DEL TITULAR: " << Dni << endl;
        cout<<"SALDO: " << Saldo << endl;
    }
}

class ArchivoTarjetas
{
    private:
        char nombre[30];
    public:
    ///CONSTRUCTOR: INICIALIZAR EL ARCHIVO CON EL NOMBRE QUE PASAMOS POR PARAMETRO
        ArchivoTarjetas(const char * n = "Tarjetas.dat"){strcpy(nombre, n);}
    ///METODOS DE LA CLASE (QUE ACCIONES VA A REALIZAR EL ARCHIVO QUE MANIPULA LAS TARJETAS)
        bool grabarRegistro(Tarjeta obj);
        Tarjeta leerRegistro(int pos);
        int buscarRegistro(int num);
        int contarRegistros();
        bool modificarRegistro(Tarjeta obj, int pos);
        bool listarRegistros();
};

bool ArchivoTarjetas::grabarRegistro(Tarjeta obj)
{
    FILE *p;
    p=fopen(nombre, "ab");
    if(p==NULL) return false;
    bool escribio=fwrite(&obj, sizeof (Tarjeta), 1, p);
    fclose(p);
    return escribio;
}


bool ArchivoTarjetas::listarRegistros()
{
    FILE *p;
    Tarjeta obj;
    p=fopen(nombre, "rb");
    if(p==NULL) return false;
    while(fread(&obj, sizeof obj, 1, p)==1)
    {
        obj.Mostrar();
        cout<<endl;
    }
    fclose(p);
    return true;
}

int ArchivoTarjetas::buscarRegistro(int num)
{
    FILE *p;
    Tarjeta obj;
    p=fopen(nombre, "rb");
    int pos=0;
    if(p==NULL) return -1;
    while(fread(&obj, sizeof obj, 1, p)==1)
    {
        if(obj.getNroTarjeta()==num)
        {
            fclose(p);
            return pos;
        }
        pos++;
    }
    fclose(p);
    return -2;
}

Tarjeta ArchivoTarjetas::leerRegistro(int pos)
{
    FILE *p;
    Tarjeta obj;
    p=fopen(nombre, "rb");
    if(p==NULL) return obj;
    fseek(p, pos*sizeof obj,0);///función que permite ubicarse dentro del archivo
    fread(&obj, sizeof obj, 1, p);
    fclose(p);
    return obj;
}

bool ArchivoTarjetas::modificarRegistro(Tarjeta obj, int pos)
{
    FILE *p;
    p=fopen(nombre, "rb+"); ///agrega al modo de apertura lo que le falta (modifica)
    if(p==NULL) return false;
    fseek(p, pos*sizeof obj,0);///función que permite ubicarse dentro del archivo
    bool escribio = fwrite(&obj, sizeof obj, 1, p);
    fclose(p);
    return escribio;
}

int ArchivoTarjetas::contarRegistros()
{
    FILE *p;
    p=fopen(nombre, "rb");
    if(p==NULL) return -1;
    fseek(p, 0,2);///función que permite ubicarse dentro del archivo, en este caso lo ubiqué al final EOF
    int tam=ftell(p);///me devuelve la cantidad de bytes que hay desde el principio del archivo a la posición actual del indicador de pFILE.
    fclose(p);
    return tam/sizeof(Tarjeta);
}

void altaTarjeta(){
    Tarjeta reg;
    ArchivoTarjetas archiTarjetas("Tarjetas.dat");
    reg.Cargar();
    archiTarjetas.grabarRegistro(reg);
}

void bajaLogicaTarjeta(){
    Tarjeta reg;
    ArchivoTarjetas archiTarjetas("Tarjetas.dat");
    cout<<"INGRESE EL NUMERO DE TARJETA A BORRAR ";
    int num;
    cin>>num;
    int pos = archiTarjetas.buscarRegistro(num);
    if(pos < 0){
        cout<<"NO SE ENCUENTRA EL REGISTRO"<<endl;
        return;
    }
    reg=archiTarjetas.leerRegistro(pos);
    reg.Mostrar();
    cout<<"ESTA SEGURO DE ELIMINAR EL REGISTRO ? (S/N) ";
    char respuesta;
    cin>>respuesta;
    if(respuesta=='S' || respuesta =='s'){
        reg.setEstado(false);
        if(archiTarjetas.modificarRegistro(reg,pos)){
            cout<<"REGISTRO ELIMINADO";
        }
        else{
            cout<<"NO SE PUDO ELIMINAR EL REGISTRO";
        }
        cout<<endl;
    }

}

void modificarSaldoTarjeta(){
    Tarjeta reg;
    ArchivoTarjetas archiTarjetas("Tarjetas.dat");
    int nroTarjeta, saldoParaAcreditar;
    cout << "INGRESE NUMERO DE TARJETA: " << endl;
    cin >> nroTarjeta;
    int pos = archiTarjetas.buscarRegistro(nroTarjeta);
    if(pos < 0){
        cout << "NO SE ENCUENTRA EL REGISTRO" << endl;
        return;
    }
    cout << "INGRESE SALDO A MODIFICAR: " << endl;
    cin >> saldoParaAcreditar;
    reg = archiTarjetas.leerRegistro(pos);
    reg.setSaldo(saldoParaAcreditar);
    archiTarjetas.modificarRegistro(reg, pos);
    cout << "SALDO ACTUALIZADO CORRECTAMENTE" << endl;
}


///ACA EMPIEZA LO RELACIONADO A VIAJES

class Viaje
{
    private:
        int NroViaje;
        int NroTarjeta;
        int MedioTransporte;
        Fecha FechaDelViaje;
        float ImporteDelViaje;
        bool Estado;
        int cuantosViajesPorTransporte[3] = {};
    public:
    ///CONSTRUCTOR
        Viaje(int NroViaje = 0, int NroTarjeta = 0, int MedioTransporte = 0, Fecha f = Fecha(), float ImporteDelViaje = 0, bool Estado = true)
        {
            NroViaje = 0;
            NroTarjeta = 0;
            MedioTransporte = 0;
            FechaDelViaje = f;
            ImporteDelViaje = 0;
            Estado = true;
        }
    ///SETTERS
        void setNroViaje(int nroViaje){NroViaje = nroViaje;}
        void setNroTarjeta(int nroTarjeta){NroTarjeta = nroTarjeta;}
        void setMedioTransporte(int medioTransporte){MedioTransporte = medioTransporte;}
        void setFechaDelViaje(Fecha fechaDelViaje){FechaDelViaje = fechaDelViaje;}
        void setImporteDelViaje(float importeDelViaje){ImporteDelViaje = importeDelViaje;}
        void setEstado(bool estado){Estado = estado;}

    ///GETTERS
        int getNroViaje(){return NroViaje;}
        int getNroTarjeta(){return NroTarjeta;}
        int getMedioTransporte(){return MedioTransporte;}
        Fecha getFechaDelViaje(){return FechaDelViaje;}
        float getImporteDelViaje(){return ImporteDelViaje;}
        bool getEstado(){return Estado;}

    ///METODOS
        void Cargar();
        void Mostrar();
        void CuantosViajesEnCadaTransporte(Viaje obj, int numero);
};

void Viaje::Cargar()
{
    cout << "NUMERO DE VIAJE: ";
    cin >> NroViaje;
    if(NroViaje < 1)
    {
        return;
    }
    cout << "NUMERO DE TARJETA: ";
    cin >> NroTarjeta;
    if(NroTarjeta < 1)
    {
        return;
    }
    cout << "MEDIO DE TRANSPORTE (1-COLECTIVO, 2-SUBTE, 3-TREN): ";
    cin >> MedioTransporte;

    cout << "FECHA DEL VIAJE: " << endl;
    FechaDelViaje.Cargar();

    if(FechaDelViaje.getAnio() == 2023)
    {
        cuantosViajesPorTransporte[MedioTransporte - 1]++;
    }
    if(MedioTransporte != 1 && MedioTransporte != 2 && MedioTransporte != 3)
    {
        return;
    }

    cout << "INGRESE IMPORTE DEL VIAJE: ";
    cin >> ImporteDelViaje;
    setEstado(true);
}

void Viaje::Mostrar()
{
    if(Estado == true){
    cout<<"NUMERO DE VIAJE: "<< NroViaje << endl;
    cout<<"NUMERO DE TARJETA: "<< NroTarjeta << endl;
    cout<<"MEDIO DE TRANSPORTE: "<< MedioTransporte << endl;
    FechaDelViaje.Mostrar();
    cout << endl;
    cout<<"IMPORTE DEL VIAJE: " << ImporteDelViaje << endl;
    }
}

void Viaje::CuantosViajesEnCadaTransporte(Viaje obj, int numero)
{
    if(obj.getNroTarjeta() != numero)
    {
        cout << "No se encontraron viajes para el número de tarjeta ingresado." << endl;
        return;
    }

    if(obj.FechaDelViaje.getAnio() != 2023)
    {
        cout << "El viaje no corresponde al año 2023." << endl;
        return;
    }

    cout << "CANTIDAD DE VIAJES EL AÑO PASADO EN COLECTIVO: " << obj.cuantosViajesPorTransporte[0] << " VIAJES" << endl;
    cout << "CANTIDAD DE VIAJES EL AÑO PASADO EN SUBTE: " << obj.cuantosViajesPorTransporte[1] << " VIAJES" << endl;
    cout << "CANTIDAD DE VIAJES EL AÑO PASADO EN TREN: " << obj.cuantosViajesPorTransporte[2] << " VIAJES" << endl;
}

class ArchivoViajes
{
    private:
        char nombre[30];
    public:
        ///CONSTRUCTOR: INICIALIZAR EL ARCHIVO CON EL NOMBRE QUE PASAMOS POR PARAMETRO
        ArchivoViajes(const char *n = "Viajes.dat"){strcpy(nombre, n);}
        ///METODOS DE LA CLASE (QUE ACCIONES VA A REALIZAR EL ARCHIVO QUE MANIPULA LOS VIAJES)
        bool grabarRegistro(Viaje obj);
        Viaje leerRegistro(int pos);
        int buscarRegistro(int num);
        int contarRegistros();
        bool modificarRegistro(Viaje obj, int pos);
        bool listarRegistros();
        int buscarRegistroPorNumeroDeTarjeta(int num);
};

bool ArchivoViajes::grabarRegistro(Viaje obj){
    FILE *p;
    p=fopen(nombre, "ab");
    if(p==NULL) return false;
    bool escribio=fwrite(&obj, sizeof (Viaje), 1, p);
    fclose(p);
    return escribio;
}


bool ArchivoViajes::listarRegistros(){
    FILE *p;
    Viaje obj;
    p=fopen(nombre, "rb");
    if(p==NULL) return false;
    while(fread(&obj, sizeof obj, 1, p)==1)
    {
        obj.Mostrar();
        cout<<endl;
    }
    fclose(p);
    return true;
}

int ArchivoViajes::buscarRegistro(int num){
        FILE *p;
        Viaje obj;
        p=fopen(nombre, "rb");
        int pos=0;
        if(p==NULL) return -1;
        while(fread(&obj, sizeof obj, 1, p)==1)
        {
            if(obj.getNroViaje()==num)
            {
                fclose(p);
                return pos;
            }
            pos++;
        }
        fclose(p);
        return -2;
    }

int ArchivoViajes::buscarRegistroPorNumeroDeTarjeta(int num)
{
    FILE *p;
    Viaje obj;
    p=fopen(nombre, "rb");
    int pos=0;
    if(p==NULL) return -1;
    while(fread(&obj, sizeof obj, 1, p)==1)
    {
        if(obj.getNroTarjeta()==num)
        {
            fclose(p);
            return pos;
        }
        pos++;
    }
    fclose(p);
    return -2;
}

Viaje ArchivoViajes::leerRegistro(int pos){
        FILE *p;
        Viaje obj;
        p=fopen(nombre, "rb");
        if(p==NULL) return obj;
        fseek(p, pos*sizeof obj,0);///función que permite ubicarse dentro del archivo
        fread(&obj, sizeof obj, 1, p);
        fclose(p);
        return obj;
    }

bool ArchivoViajes::modificarRegistro(Viaje obj, int pos){
        FILE *p;
        p=fopen(nombre, "rb+"); ///agrega al modo de apertura lo que le falta (modifica)
        if(p==NULL) return false;
        fseek(p, pos*sizeof obj,0);///función que permite ubicarse dentro del archivo
        bool escribio = fwrite(&obj, sizeof obj, 1, p);
        fclose(p);
        return escribio;
    }

int ArchivoViajes::contarRegistros(){
        FILE *p;
        p=fopen(nombre, "rb");
        if(p==NULL) return -1;
        fseek(p, 0,2);///función que permite ubicarse dentro del archivo, en este caso lo ubiqué al final EOF
        int tam=ftell(p);///me devuelve la cantidad de bytes que hay desde el principio del archivo a la posición actual del indicador de pFILE.
        fclose(p);
        return tam/sizeof(Viaje);
    }
void altaViaje(){
    Viaje reg;
    ArchivoViajes archiViajes("Viajes.dat");
    reg.Cargar();
    archiViajes.grabarRegistro(reg);
}

void ManagerMenuTarjetas(){
        ArchivoTarjetas archiTarjetas;
        Tarjeta aux;

        while(true){
        ///DECLARO ESTAS VARIABLES PARA MANIPULAR LOS ARCHIVOS "TARJETAS.DAT" Y "VIAJES.DAT"
        int opc;
        int numeroDeTarjeta, pos;
        system("cls");
        cout<<"MENU DE TARJETAS"<<endl;
        cout<<"--------------"<<endl;
        cout<<"1 - ALTA TARJETA"<<endl;
        cout<<"2 - BAJA LOGICA TARJETA"<<endl;
        cout<<"3 - MODIFICAR SALDO DE TARJETA"<<endl;
        cout<<"4 - LISTAR TARJETAS"<<endl;
        cout<<"5 - BUSCAR TARJETA POR NUMERO"<<endl;
        cout<<"0 - VOLVER AL MENU PRINCIPAL"<<endl;
        cout<<"----------------------------------------"<<endl;
        cout<<"INGRESE LA OPCION: ";
        cin>>opc;
        system("cls");
        switch(opc){
            case 1:
                altaTarjeta();
                break;
            case 2:
                bajaLogicaTarjeta();
                break;
            case 3:
                modificarSaldoTarjeta();
                break;
            case 4:
                archiTarjetas.listarRegistros();
                system("pause");
                break;
            case 5:
                cout << "INGRESE NUMERO DE TARJETA A BUSCAR: " << endl;
                cin >> numeroDeTarjeta;
                pos = archiTarjetas.buscarRegistro(numeroDeTarjeta);
                if(pos >= 0){
                    aux = archiTarjetas.leerRegistro(pos);
                    aux.Mostrar();
                }else{
                    cout << "EL NUMERO DE TARJETA INGRESADO NO EXISTE, POR FAVOR INGRESE UNO CORRECTO" << endl;
                }
                system("pause");
                break;
            case 0:
                return;
            default:
                cout << "LA OPCION INGRESADA NO ES CORRECTA" << endl;
                system("pause");
                break;
            }
        }
}

void ManagerMenuViajes(){
        Viaje aux, variableViaje;
        ArchivoViajes archiViajes("Viajes.dat");
        int numeroDeTarjeta, pos, cantidadRegistros;

        while(true){
            ///DECLARO ESTAS VARIABLES PARA MANIPULAR LOS ARCHIVOS "TARJETAS.DAT" Y "VIAJES.DAT"
            int opc;
            system("cls");
            cout<<"MENU DE VIAJES"<<endl;
            cout<<"--------------"<<endl;
            cout<<"1 - CANTIDAD DE VIAJES REALIZADOS EN CADA UNO DE LOS MEDIOS DE TRANSPORTE EL AÑO PASADO (2023) POR TARJETA"<<endl;
            cout<<"2 - LISTAR VIAJES"<<endl;
            cout<<"3 - ALTA DE VIAJE"<<endl;
            cout<<"4 - CONTAR CANTIDAD DE VIAJES"<<endl;
            cout<<"5 - BUSCAR TARJETA POR NUMERO"<<endl;
            cout<<"0 - VOLVER AL MENU PRINCIPAL"<<endl;
            cout<<"----------------------------------------"<<endl;
            cout<<"INGRESE LA OPCION: ";
            cin>>opc;
            system("cls");
            switch(opc){
                case 1:
                    cout << "INGRESE NUMERO DE TARJETA A ANALIZAR: " << endl;
                    cin >> numeroDeTarjeta;
                    pos = archiViajes.buscarRegistroPorNumeroDeTarjeta(numeroDeTarjeta);
                    variableViaje = archiViajes.leerRegistro(pos);
                    aux.CuantosViajesEnCadaTransporte(variableViaje, numeroDeTarjeta);
                    system("pause");
                    break;
                case 2:
                    archiViajes.listarRegistros();
                    system("pause");
                    break;
                case 3:
                    altaViaje();
                    break;
                case 4:
                    cantidadRegistros = archiViajes.contarRegistros();
                    cout << cantidadRegistros;
                    system("pause");
                    break;
                case 5:
                    system("pause");
                    break;
                case 0:
                    return;
                default:
                    system("pause");
                    break;
            }
        }
}

///MAIN

    int main()
    {
        setlocale(LC_ALL, "spanish");
        int eleccion;

        do{
            cout << "BIENVENIDO! ¿A QUE MENÚ DESEA INGRESAR?" << endl;
            cout << "1- MENU TARJETAS" << endl;
            cout << "2- MENU VIAJES" << endl;
            cout << "0- SALIR" << endl;
            cin >> eleccion;
            system("cls");

            if(eleccion == 1){
                ManagerMenuTarjetas();
            }else if(eleccion == 2){
                ManagerMenuViajes();
            }else if(eleccion != 0){
                cout << "INGRESE UN NÚMERO VÁLIDO EN EL MENÚ! " << endl;
                system("pause");
                system("cls");
            }

        }while(eleccion != 0);

        cout << "¡GRACIAS POR UTILIZAR SISTEMAS GAUNA!" << endl;

        system("pause>nul");
        return 0;
    }
