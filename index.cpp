#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

const int atm[5]   = {0, 11000, 20000, 32000, 47000};
const double a[5]  = {-0.0065, 0, 0.001, 0.0028, 0};
const double r     = 287.00;
const double g     = 9.80665;
const double e     = 2.71828182;
const double pi    = 3.14159265;
const double gamma = 1.40;
const double seaLevelDensity = 1.225;

double temperature(double t0, double a, double h);
double pressure(double p0, double t0, double t1, double a);
double pressure2(double p0, double t, double h);
double density(double p, double t);
double lift(double cl, double density, double vel, double s);
double drag(double cd, double density, double vel, double s);
double thrust(double mdot, double vj, double tas);
double weight(double m);
double velocity(double lift, double cl, double density, double s);
double easVel(double tas, double density);
double tasVel(double eas, double density);
double mach(double vel, double speedSound);
double speedOfSound(double temperature);
double massFlow(double thrustAircraft, double tas, double vj);
double aspectRatio(double b, double s);
double dragCoefficient(double cl, double cd0, double aspRat, double eff);
double balloonLiftTemp(double density, double temperature, double tempGas, double volumen);
double balloonLiftMass(double density, double temperature, double tempGas, double volumen);
double balloonVolumen(double density, double airMass, double gasMass, double m);
string printMachClass(double mach);
void proccess(double alt);
void printResults(double t0, double p0, double d);
void printTitle();
void printMenu();
void printMenuBalloon();
void printAircraft();
void printBalloon();
void centerScreen();

void centerScreen(){
	
	HWND ConsoleWindow;
	ConsoleWindow = GetForegroundWindow();

	HWND   hwndScreen;
	RECT   rectScreen;		
	hwndScreen = GetDesktopWindow();  
	GetWindowRect(hwndScreen, &rectScreen); 		
	
	SetWindowPos(ConsoleWindow, NULL, 0, 0, 1000, 500, SWP_SHOWWINDOW); 

	RECT rConsole;
	GetWindowRect(ConsoleWindow, &rConsole); 		
	int Width  = rConsole.left = rConsole.right;
	int Height = rConsole.bottom - rConsole.top;
		
	int ConsolePosX;
	int ConsolePosY;		
	ConsolePosX = ((rectScreen.right - rectScreen.left) / 2 - Width / 2 );
	ConsolePosY = ((rectScreen.bottom - rectScreen.top) / 2 - Height / 2 );	
	SetWindowPos(ConsoleWindow, NULL, ConsolePosX, ConsolePosY, Width, Height, SWP_SHOWWINDOW || SWP_NOSIZE);
	
}

void printTitle(){
	cout<<  
	"\t\t   _____                                                    __    __                 \n"
	"\t\t  /  _  \\    ____  _______   ____    ____  _____    __ __ _/  |_ |__|  ____  _____   \n"
	"\t\t /  /_\\  \\ _/ __ \\ \\_  __ \\ /  _ \\  /    \\ \\__  \\  |  |  \\\\   __\\|  |_/ ___\\ \\__  \\  \n"
	"\t\t/    |    \\\\  ___/  |  | \\/(  <_> )|   |  \\ / __ \\_|  |  / |  |  |  |\\  \\___  / __ \\_\n"
	"\t\t\\____|__  / \\___  > |__|    \\____/ |___|  /(____  /|____/  |__|  |__| \\___  >(____  /\n"
	"\t\t        \\/      \\/                      \\/      \\/                        \\/      \\/ \n";

	cout<<"\n";
    
}

void printAircraft(){
	cout<<
	"\t\t	                                    |\n"
	"\t\t\t                                    |\n"
	"\t\t\t                                    |\n"
	"\t\t\t                                  .-'-.\n"
	"\t\t\t                                 ' ___ '\n"
	"\t\t\t                       ---------'  .-.  '---------\n"
	"\t\t\t       _________________________'  '-'  '_________________________\n"
	"\t\t\t        ''''''-|---|--/    \\==][^',_m_,'^][==/    \\--|---|-''''''\n"
	"\t\t\t                      \\    /  ||/   H   \\||  \\    /\n"
	"\t\t\t                       '--'   OO   O|O   OO   '--'\n";
	cout<<"\n";

}

void printBalloon(){
	cout<<
	"\t\t\t\t\t	  ,~-.\n"
	"\t\t\t\t\t         (  ' )-.          ,~'`-.\n"
	"\t\t\t\t\t      ,~' `  ' ) )       _(   _) )\n"
	"\t\t\t\t\t     ( ( .--.===.--.    (  `    ' )\n"
	"\t\t\t\t\t      `.%%.;::|888.#`.   `-'`~~=~'\n"
	"\t\t\t\t\t      /%%/::::|8888\\##\\\n"
	"\t\t\t\t\t     |%%/:::::|88888\\##|\n"
	"\t\t\t\t\t     |%%|:::::|88888|##|.,-.\n"
	"\t\t\t\t\t     \\%%|:::::|88888|##/    )_\n"
	"\t\t\t\t\t      \\%\\:::::|88888/#/ ( `'  )\n"
	"\t\t\t\t\t       \\%\\::::|8888/#/(  ,  -'`-.\n"
	"\t\t\t\t\t   ,~-. `%\\:::|888/#'(  (     ') )\n"
	"\t\t\t\t\t  (  ) )_ `\\__|__/'   `~-~=--~~='\n"
	"\t\t\t\t\t ( ` ')  ) [VVVVV]\n"
	"\t\t\t\t\t(_(_.~~~'   \\|_|/ \n"
	"\t\t\t\t\t            [XXX]\n\n";
}

void printMenu(){
	cout<<"\t\t\t";
	cout<<"[1] Atmosfera a una altitud\t\t";
	cout<<"[9] Numero Mach (M)\n";
	cout<<"\t\t\t";
	cout<<"[2] Sustentacion (L)\t\t\t";
	cout<<"[10] Velocidad del sonido (a)\n";
	cout<<"\t\t\t";
	cout<<"[3] Resistencia (D)\t\t\t";
	cout<<"[11] Eficiencia (nj)\n";
	cout<<"\t\t\t";
	cout<<"[4] Empuje (T)\t\t\t\t";
	cout<<"[12] Flujo masico\n";
	cout<<"\t\t\t";
	cout<<"[5] Peso (W)\t\t\t\t";
	cout<<"[13] Relacion de aspecto\n";
	cout<<"\t\t\t";
	cout<<"[6] Velocidad (V)\t\t\t";
	cout<<"[14] Cd(Cl)\n";
	cout<<"\t\t\t";
	cout<<"[7] Velocidad equivalente (EAS)\t\t";
	cout<<"[15] Globo aerostatico\n";
	cout<<"\t\t\t";
	cout<<"[8] Velocidad verdadera (TAS)\t\t";
	cout<<"[16] Salir";
	
}

void printMenuBalloon(){
	cout<<"\t\t\t";
	cout<<"[1] Sustentacion por temperatura\t";
	cout<<"[3] Volumen necesario para elevar un cuerpo\n";
	cout<<"\t\t\t";
	cout<<"[2] Sustentacion por masa molar\t\t";
	cout<<"[4] Volver al menu principal";
	
}

double temperature(double t0, double a, double h){
	
	return t0 + (a * h);
}

double pressure(double p0, double t0, double t1, double a){
	
	double aux = (-1)*g / (a * r);
	
	return pow(t1 / t0, aux) * p0;
}

double pressure2(double p0, double t, double h){
	
	double aux  = (-1)*g / (t * r);
	double aux2 = aux * h;
	
	return pow(e, aux2) * p0;                                                 
}

double density(double p, double t){
	
	return p / (r * t);
}

void printResults(double t0, double p0, double d){

	cout<<"\n\t\tTemperatura: \t"<<fixed<<setprecision(8)<<t0<<" K";
	cout<<"\n\t\tPresion: \t"<<fixed<<setprecision(8)<<p0<<" Pa";
	cout<<"\n\t\tDensidad: \t"<<fixed<<setprecision(8)<<d<<" kg/m3\n";
}

void proccess(double alt){
	
	int i     = 1;
	double t1 = 0;
	double t0 = 288.15;
	double p1 = 0;
	double p0 = 101325;
	double d  = 0;
	double h0 = atm[1];
	double h1 = alt;
	
	if(alt < h0){
		
		t1 = temperature(t0, a[0], h1);
		p1 = pressure(p0, t0, t1, a[0]);
			
		t0 = t1;
		p0 = p1;
		
	}else{
		
		while(alt >= h0 && i < 5){
			
			h1 = h0;
			h0 = atm[i-1];
		
			if(a[i-1] != 0){
				
				t1 = temperature(t0, a[i-1], h1 - h0);
				p1 = pressure(p0, t0, t1, a[i-1]);
				
				t0 = t1;
				p0 = p1;
				
			}else{
				p1 = pressure2(p0, t0, h1 - h0);
				p0 = p1;	
			}
			i++;
			h0 = atm[i];
		}

		if(i < 5){
			i--;
			if(alt != atm[i]){
				
				if(a[i] != 0){
				
				t1 = temperature(t0, a[i], alt - atm[i]);
				p1 = pressure(p0, t0, t1, a[i]);
				
				t0 = t1;
				p0 = p1;
				
				}else{
					p1 = pressure2(p0, t0, alt - atm[i]);
					p0 = p1;
				}
			}
			
		}else{
			p1 = pressure2(p0, t0, alt - atm[4]);
			p0 = p1;	
		}
	}
	
	d = density(p0, t0);
	
	printResults(t0, p0, d);

}

double lift(double cl, double density, double vel, double s){
	
	return cl * 0.5 * density * pow(vel, 2) * s;
}

double drag(double cd, double density, double vel, double s){
	
	return cd * 0.5 * density * pow(vel, 2) * s;
}

double thrust(double mdot, double vj, double tas){
	
	return mdot * (vj - tas);
}

double weight(double m){
	
	return m * g;
}

double velocity(double lift, double cl, double density, double s){
	
	return sqrt(2 * lift / (cl * density * s));
}

double easVel(double tas, double density){
	
	return tas / sqrt(seaLevelDensity / density);
}

double tasVel(double eas, double density){
	
	return eas * sqrt(seaLevelDensity / density);
}

double mach(double vel, double speedSound){
	
	return vel / speedSound;
}

string printMachClass(double mach){
	
	if(mach <= 0.7) return "Subsonico";
	
	if(mach > 0.7 && mach <= 1.2) return "Transonico";
	
	if(mach > 1.2 && mach <= 5) return "Supersonico";
	
	if(mach > 5) return "Hipersonico";

}

double speedOfSound(double temperature){
	
	return sqrt(gamma * r * temperature);
}

double massFlow(double thrustAircraft, double tas, double vj){
	
	return thrustAircraft / (vj - tas);
}

double aspectRatio(double b, double s){
	
	return pow(b, 2) / s;
}

double dragCoefficient(double cl, double cd0, double aspRat, double eff){
	
	return cd0 + (pow(cl, 2) / (pi * aspRat * eff));
}

double balloonLiftTemp(double density, double temperature, double tempGas, double volumen){
	
	return density * volumen * g *(1 - (temperature / tempGas));
}

double balloonLiftMass(double density, double airMass, double gasMass, double volumen){
	
	return density * volumen * g *(1 - (gasMass / airMass));
}

double balloonVolumen(double density, double airMass, double gasMass, double m){
	
	return m / ((1 - (gasMass / airMass)) * density);
}

int main(){
	int option, value;
	
	system("Color 0A");
	centerScreen();
	printTitle();
	printAircraft();
	printMenu();
	cout<<"\n\t\t\t\t\t\tSU ELECCION: ";
	cin>>option;
	system("cls");
	printTitle();
	printAircraft();
	while(option != 16){
		system("cls");
		printTitle();
		switch(option){
			case 1: 
				double altitude;
				cout<<"\n\t\tAltitud (en metros): ";
				cin>>altitude;
				cout<<"\n\t\tCondiciones atmosfericas a una altitud de "<<altitude<<"m\n";
				cout<<"\t\t________________________________________________";
				proccess(altitude);
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 2: 
				double cl, density, vel, s;
				cout<<"\n\t\tCoeficiente de sustentacion: ";
				cin>>cl;
				cout<<"\n\t\tDensidad (en kg/m3): ";
				cin>>density;
				cout<<"\n\t\tVelocidad (en m/s): ";
				cin>>vel;
				cout<<"\n\t\tSuperficie alar (en m2): ";
				cin>>s;
				cout<<"\n\t\tSustentacion generada por la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tL = "<<fixed<<setprecision(8)<<lift(cl, density, vel, s)<<"N\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 3: 
				double cd;
				cout<<"\n\t\tCoeficiente de resistencia: ";
				cin>>cd;
				cout<<"\n\t\tDensidad (en kg/m3): ";
				cin>>density;
				cout<<"\n\t\tVelocidad (en m/s): ";
				cin>>vel;
				cout<<"\n\t\tSuperficie alar (en m2): ";
				cin>>s;
				cout<<"\n\t\tResistencia generada por la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tD = "<<fixed<<setprecision(8)<<drag(cd, density, vel, s)<<"N\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 4: 
				double mdot, vj, tas;
				cout<<"\n\t\tFlujo masico (en kg/s): ";
				cin>>mdot;
				cout<<"\n\t\tVelocidad j (en m/s): ";
				cin>>vj;
				cout<<"\n\t\tVelocidad verdadera (en m/s): ";
				cin>>tas;
				cout<<"\n\t\tEmpuje generado por la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tT = "<<fixed<<setprecision(8)<<thrust(mdot, vj, tas)<<"N\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 5: 
				double m;
				cout<<"\n\t\tMasa (en kg): ";
				cin>>m;
				cout<<"\n\t\tPeso de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tW = "<<fixed<<setprecision(8)<<weight(m)<<"N\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 6: 
				double lift;
				cout<<"\n\t\tSustentacion (en N): ";
				cin>>lift;
				cout<<"\n\t\tCoeficiente de sustentacion: ";
				cin>>cl;
				cout<<"\n\t\tDensidad (en kg/m3): ";
				cin>>density;
				cout<<"\n\t\tSuperficie alar (en m2): ";
				cin>>s;
				cout<<"\n\t\tVelocidad de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tV = "<<fixed<<setprecision(8)<<velocity(lift, cl, density, s)<<"m/s\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 7: 
				cout<<"\n\t\tDensidad (en kg/m3): ";
				cin>>density;
				cout<<"\n\t\tVelocidad verdadera (en m/s): ";
				cin>>tas;
				cout<<"\n\t\tVelocidad equivalente de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tEAS = "<<fixed<<setprecision(8)<<easVel(tas, density)<<"m/s\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 8:
				double eas;
				cout<<"\n\t\tDensidad (en kg/m3): ";
				cin>>density;
				cout<<"\n\t\tVelocidad equivalente (en m/s): ";
				cin>>eas;
				cout<<"\n\t\tVelocidad verdadera de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tTAS = "<<fixed<<setprecision(8)<<tasVel(eas, density)<<"m/s\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 9:
				double speedSound, result;
				cout<<"\n\t\tVelocidad (en m/s): ";
				cin>>vel;
				cout<<"\n\t\tVelocidad del sonido (en m/s): ";
				cin>>speedSound;
				cout<<"\n\t\tNumero MACH de la aeronave\n";
				cout<<"\t\t________________________________________________";
				result = mach(vel, speedSound);
				cout<<"\n\t\tM = "<<fixed<<setprecision(8)<<result<<"\n";
				cout<<"\n\t\tCorresponde a un vuelo: "<<printMachClass(result)<<"\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 10:
				double temperature;
				cout<<"\n\t\tTemperatura (en K): ";
				cin>>temperature;
				cout<<"\n\t\tVelocidad del sonido a "<<temperature<<"K\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\ta = "<<fixed<<setprecision(8)<<speedOfSound(temperature)<<"m/s\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 11:
				double pa, pj;
				cout<<"\n\t\tPoder disponible (en W): ";
				cin>>pa;
				cout<<"\n\t\tPoder jet (en W): ";
				cin>>pj;
				cout<<"\n\t\tEficiencia de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tnj = "<<fixed<<setprecision(8)<<(pa / pj)<<"\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 12: 
				double thrustAircraft;
				cout<<"\n\t\tEmpuje (en N): ";
				cin>>thrustAircraft;
				cout<<"\n\t\tVelocidad verdadera (en m/s): ";
				cin>>tas;
				cout<<"\n\t\tVelocidad j (en m/s): ";
				cin>>vj;
				cout<<"\n\t\tFlujo masico de los motores\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tm(.) = "<<fixed<<setprecision(8)<<massFlow(thrustAircraft, tas, vj)<<"kg/s\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 13: 
				double b;
				cout<<"\n\t\tEnvergadura (en m): ";
				cin>>b;
				cout<<"\n\t\tSuperficie alar (en m2): ";
				cin>>s;
				cout<<"\n\t\tRelacion de aspecto de la aeronave\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tA = "<<fixed<<setprecision(8)<<aspectRatio(b, s)<<"\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 14: 
				double cd0, eff, aspRat;
				cout<<"\n\t\tCl: ";
				cin>>cl;
				cout<<"\n\t\tCd0: ";
				cin>>cd0;
				cout<<"\n\t\tRelacion de aspecto: ";
				cin>>aspRat;
				cout<<"\n\t\tCoeficiente de eficiencia: ";
				cin>>eff;
				cout<<"\n\t\tCoeficiente de resistencia\n";
				cout<<"\t\t________________________________________________";
				cout<<"\n\t\tCd = "<<fixed<<setprecision(8)<<dragCoefficient(cl, cd0, aspRat, eff)<<"\n";
				fflush(stdin);
				cout<<"\n\t\tPresione enter para volver al menu principal...";
				cin.ignore(); 
			break;
			
			case 15: 
				int option1;
				system("cls");
				printTitle();
				printBalloon();
				printMenuBalloon();
				cout<<"\n\t\t\t\t\t\tSU ELECCION: ";
				cin>>option1;
				
				while(option1 != 4){
					system("cls");
					printTitle();
					switch(option1){
						case 1:
							double volumen, tempGas;
							cout<<"\n\t\tDensidad atmosferica (en kg/m3): ";
							cin>>density;
							cout<<"\n\t\tTemperatura atmosferica (en K): ";
							cin>>temperature;
							cout<<"\n\t\tTemperatura del gas utilizado (en K): ";
							cin>>tempGas;
							cout<<"\n\t\tVolumen (en m3): ";
							cin>>volumen;
							cout<<"\n\t\tSustentacion generada por el globo\n";
							cout<<"\t\t________________________________________________";
							cout<<"\n\t\tL = "<<fixed<<setprecision(8)<<balloonLiftTemp(density, temperature, tempGas, volumen)<<"N\n";
							fflush(stdin);
							cout<<"\n\t\tPresione enter para volver al menu principal...";
							cin.ignore(); 
						break;
						case 2:
							double airMass, gasMass;
							cout<<"\n\t\tDensidad atmosferica (en kg/m3): ";
							cin>>density;
							cout<<"\n\t\tMasa molar del aire (en g/mol): ";
							cin>>airMass;
							cout<<"\n\t\tMasa molar del gas utilizado (en g/mol): ";
							cin>>gasMass;
							cout<<"\n\t\tVolumen (en m3): ";
							cin>>volumen;
							cout<<"\n\t\tSustentacion generada por el globo\n";
							cout<<"\t\t________________________________________________";
							cout<<"\n\t\tL = "<<fixed<<setprecision(8)<<balloonLiftMass(density, airMass, gasMass, volumen)<<"N\n";
							fflush(stdin);
							cout<<"\n\t\tPresione enter para volver al menu principal...";
							cin.ignore(); 
						break;
						case 3:
							cout<<"\n\t\tDensidad atmosferica (en kg/m3): ";
							cin>>density;
							cout<<"\n\t\tMasa molar del aire (en g/mol): ";
							cin>>airMass;
							cout<<"\n\t\tMasa molar del gas utilizado (en g/mol): ";
							cin>>gasMass;
							cout<<"\n\t\tMasa del cuerpo a elevar (en kg): ";
							cin>>m;
							cout<<"\n\t\tVolumen de gas necesario para elevar el cuerpo de "<<m<<"kg\n";
							cout<<"\t\t________________________________________________________";
							cout<<"\n\t\tVol = "<<fixed<<setprecision(8)<<balloonVolumen(density, airMass, gasMass, m)<<"m3\n";
							fflush(stdin);
							cout<<"\n\t\tPresione enter para volver al menu principal...";
							cin.ignore(); 
						break;
					}
					system("cls");
					printTitle();
					printBalloon();
					printMenuBalloon();
					cout<<"\n\t\t\t\t\t\tSU ELECCION: ";
					cin>>option1;
				}
			break;
		}
		system("cls");
		printTitle();
		printAircraft();
		printMenu();
		cout<<"\n\t\t\t\t\t\tSU ELECCION: ";
		cin>>option;
	}
	
	return 0;
}
