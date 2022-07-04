#include <iostream>
#include<stdio.h>
using namespace std;
#include <fstream>
#include<string>
#include <sys/stat.h>
#include<time.h>
#include <conio.h>

#include <cstdlib>

struct stat info;

void login();
string color(string colorName);
void colorize(string msg,string colorName);
void clear();
int askForNumber(int min,int max);
int authenticate(string username,string password,string role);
void adminDashboard();
void userDashboard();
string colorizeReturn(string msg,string colorName);
void listUsers();
void registerUser();
bool doesPathExist(string s);
void isFolder(string);
string takeStringInput(string msg);
int mobileNumberExists(string mobileNumber);
long int accountNumber();
string generateRandomPassword();
int generateCharacter(int,int);
int sendNotification(string msg,string number,int isNew);
void setUnseenNotification(string number,int isNew);
int removeAndRename(string tempFile,string originalFile);
void toHtml();
int countLinesInFile(string filename);
int depositMoney(string mobileNumber,float amount);
void addBalance(string mobileNumber,float amount);
void superNotification(char msg[]);

int changePasswordOrPin(string choice);
void showNotifications();
int transferMoney(string toMobile,float amount); //login to user acc before transferring as sender mobile is global variable set in authenticate()
string askPassword();
void firstTimeLogin();
int noOfUnseenNotification(); // returns no of unseen notification for currentUserMobile (current logged in user)
float checkBalance(string number);





// global variables
string currentUser,currentUserMobile;
int firstLogin,currentUserPin;

class BaseUser{
    protected:
        string name,number;
        int age;
        char role;
    
};



class Member:public BaseUser{
    public:
        Member(){

        }
        Member(string name1,string number1,int age1){
            name = name1;
            number = number1;
            age = age1;
        }
        void show(){
            cout << "name: "<< name <<endl;
            cout << "number: "<< number <<endl;
            cout << "strging "<< name[2] <<endl;

        }

};

int main(){
	srand(time(0));
    clear();
    login();
}

bool doesPathExist(string s){
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

void isFolder(string dirname){
	if (!doesPathExist(dirname)){
		string cmd;
		cmd = "mkdir " + dirname;
		system(cmd.c_str());
	}
}

void login(){
	string role,username,password;
	cout << "Login to Continue\n";
	colorize("If you are not register contact administrator\n","yellow");
	cout << "1. Admin Login\n2. User Login\n";
	int userChoice = askForNumber(1,2);
	clear();
	if (userChoice==1){
		role="a";//admin
		cout << "Admin Login\n";
	}else{
		role = "u";//user
		cout << "User Login\n";
	}	
	cout << "Username : ";
	cin >> username;
	cout << "Password : ";
	password = askPassword();
    authenticate(username,password,role);

}

void colorize(string msg,string colorname){
    cout << color(colorname) << msg << color("reset");
}

string color(string colorName){
	if (colorName == "red"){
		return "\033[1;31m";
	}
	else if (colorName == "green"){
		return "\033[1;32m";
	}
	else if (colorName == "yellow"){
		return "\033[1;33m";
	}
	else if (colorName == "blue"){
		return "\033[1;34m";
	}
	else if (colorName == "magenta"){
		return "\033[1;35m";
	}
	else if (colorName == "cyan"){
		return "\033[1;36m";
	}
	else if (colorName == "white"){
		return "\033[1;37m";
	}
	else if (colorName == "reset"){
		return "\033[1;m";
	}
	else if (colorName == "blueUnderline"){
		return "\033[1;34;4m";
	}
}

void clear(){
	#ifdef __WIN32
	system("cls");
	#endif
	#ifdef linux
	system("clear");
	#endif
}

int askForNumber(int min,int max){
	int n;
    char errMsg[100];
	while (1){
		cout << "Choose ("<< min <<" ... " << max <<") : ";
        cin >> n;
		if (n>=min && n<=max){
			return n;
		}
		else{
			sprintf(errMsg,"Invalid Input: Choose a number between %d and %d\n", min,max);	
			colorize(errMsg,"red");		
		}
	}
}

int authenticate(string username,string password,string role){
    string filename,user,pass,fname;
	int usrFound =0,pin;
	if (role == "a"){
        filename = "login/admin.txt";
	}
	else{
        filename = "login/users.txt";
	}
    ifstream fp(filename.c_str());
	while (!fp.eof()){
        fp >> user>>pass>>fname>>firstLogin>>pin;
		if (username == user && password == pass){
			
			clear();
			if (role=="a"){
                fp.close();
				currentUser == username; //setting up value for global variable "currentUser"
				adminDashboard();
				
			}
			else{
				// fclose(fp);
                fp.close();
				currentUser == fname; // third entry is fname in users.txt
				currentUserMobile = user; // first entry is mobile num " "
				currentUserPin = pin;
				userDashboard();
				
			}
			usrFound = 1;
			return 0;			
		}
	}
	if (!usrFound){
		colorize("\nIncorrect username or password.\n","red");
		colorize("Forgot Password? (y/n) : ","white");
		char ch;
		// cleanStdin();
		cin >> ch;
		if(ch=='Y' || ch=='y'){
			printf("Relax and Try to remember...;)");
		}
	}
	
}

void adminDashboard(){
	cout << "welcome to admin dashboard, "<<currentUser <<endl;
	cout << "1. Register User\n";
	cout << "2. List User\n";
	cout << "3. Deposit\n";
	cout << "4. Send Notification to all\n";
	cout << "5. Exit\n";
	int choice = askForNumber(1,5);
	clear();
	if (choice==1){
		registerUser();
	}
	else if (choice ==2 ){
		listUsers();
		char ch;
		colorize("\nExport to html and json? (y/n): ","white");
		cin >> ch;
		if (ch=='y'){
			toHtml();
		}
	}
	else if (choice ==3){
        string number;
		char msg[200];
		float amount;
		cout << "Mobile Number: ";
		cin >> number;
		cout << "Amount: ";
		cin >> amount;
		if (depositMoney(number,amount)==1){
			sprintf(msg,"Rs. %.2f has been deposited to your account",amount);
			cout << endl << colorizeReturn(msg,"green") << endl;
			sendNotification(msg,number,0);
		}
		else{
			colorize("\nsomething wrong\n","red");
		}
	}
	else if (choice==4){
        char msg[200];
		cout << "Message or(q to cancel): ";
		fflush(stdin);
        cin.getline(msg,200);
		if (strlen(msg) == 1 && msg[0]=='q'){
			clear();
			adminDashboard();
		}
		superNotification(msg);
		colorize("Notification sent to all existing users\n","green");
	}
	else{
		exit(0);
	}
	system("pause");
	clear();
	adminDashboard();
}

string colorizeReturn(string msg,string colorName){
    char tempString[200];
	sprintf(tempString,"%s%s%s",color(colorName).c_str(),msg.c_str(),color("reset").c_str());
    return tempString;
}

void listUsers(){
	
	string filename = "details/customerdetails.txt";
	if (!doesPathExist("details")){
		colorize("No User Data Found.","red");
		return;
	}
    ifstream fp(filename.c_str());
	string firstname,lastname,number,gender,DOB,ac;
	string fullname,formattedStr;
	int age;
    string extraSpace;
	char finalLine[200];
	int counter = countLinesInFile(filename);
	cout << colorizeReturn("Acc no.\t\tName\t\t\tNumber\t\tGender\tAge\tDOB\n","blueUnderline");
    for (int i=1;i<=counter;i++){
        fp >> ac >> firstname >> lastname >> number >> gender >> age >> DOB;
		fullname = firstname + " " + lastname;
        string str = number;
        str.length() < 8 ? extraSpace = "\t" : extraSpace = "";
		sprintf(finalLine,"%s\t%s\t\t%s\t%s%s\t%d\t%s",ac.c_str(),fullname.c_str(),number.c_str(),extraSpace.c_str(),gender.c_str(),age,DOB.c_str());
		// finalLine =  ac + "\t" + fullname + "\t\t" + number + "\t" + extraSpace  + gender + "\t" + to_string(age) + "\t" + DOB;
		cout << finalLine<<endl;
		
    }
    fp.close();
}

void registerUser(){
    // creates details folder if not exists
	string firstname,lastname,number,gender,dob,age;
	char line[200];
    isFolder("details");
    ofstream fp("details/customerdetails.txt",ios::app);
	firstname = takeStringInput("Firstname : ");
	lastname = takeStringInput("Lastname : ");
	while (1){
		number = takeStringInput("Number : ");
		if (mobileNumberExists(number)){
			colorize("Mobile number already exists\n","red");
		}
		else{
			break;
		}
	}
	gender = takeStringInput("Gender : ");
	age = takeStringInput("Age : ");
	dob = takeStringInput("date of birth : ");

    long int ac = accountNumber(); 
    sprintf(line,"%ld %s %s %s %s %s %s\n",ac,firstname.c_str(),lastname.c_str(),number.c_str(),gender.c_str(),age.c_str(),dob.c_str());
	fp << line;
	fp.close();
 	// // adding 0 balance to user in balance file
 	isFolder("balance");
 	ofstream balance("balance/allbalances.txt",ios::app);
 	sprintf(line, "%ld %s %f\n",ac,number.c_str(),0.0);
	balance << line;
	balance.close();

 	// // generating temporary password and saving login/users.txt
	// // format -> number password firstname
	string tempPass;
	tempPass = generateRandomPassword();
	ofstream userFile("login/users.txt",ios::app);
	sprintf(line, "%s %s %s 1 9999\n",number.c_str(),tempPass.c_str(),firstname.c_str());
	userFile << line;
	userFile.close();
 	colorize("User is registered Successfully\n","green");
 	cout << "Account No.\t: "<< ac << endl;
 	cout << "Username \t: " << number << endl;
 	cout << "Password \t: "<< tempPass<<endl;
 	cout << "\nChange password and set transation pin on first login\n";
	char tosnd[50] = "Account Registration Complete";
	sendNotification(tosnd,number,1); // 1 new notification for new account so isNew=1
 }

string takeStringInput(string msg){
	string s;
	cout << msg;
	cin >> s;
	return s;
}

int mobileNumberExists(string mobileNumber){
	char filename[30] = "details/customerdetails.txt";
	char ac[10],fname[20],lname[20],num[15],gender[10],dob[15];
	int age;
	ifstream fp(filename);
	while (!fp.eof()){
		fp >> ac>>fname>>lname>>num>>gender>>age>>dob;
		if (mobileNumber == num){
			return 1;
		}
	}
	fp.close();
	return 0;
}

long int accountNumber(){
	return time(0);
}

string generateRandomPassword(){
	static char pass[5];
	int charType; //0->lowercase 1-->uppercase 2->number
	for (int i=0;i<5;i++){
		charType = rand()%3;
		if (charType==0){
			pass[i] = generateCharacter(97,122);
		}
		else if (charType == 1){
			pass[i] = generateCharacter(65,90);
		}
		else{
			pass[i] = generateCharacter(50,57);
			// 0 and 1 is exclude as user might get confused with letter O and l
		}
	}
	return pass;
}

int generateCharacter(int min,int max){	
	return (rand() % (max-min+1))+min;	
}

int sendNotification(string msg,string number,int isNew){
	char filename[40],finalNofiticationMsg[200];
	sprintf(filename,"%s.txt",number.c_str());
	char path[20] = "notifications",eachLine[200],originalFile[100],eachLine1[200];
	isFolder(path);
	sprintf(originalFile,"%s/%s",path,filename);
	ofstream nofitication_file(originalFile,ios::app);
	sprintf(finalNofiticationMsg,"%s | %s\n",msg.c_str(),__DATE__);
	nofitication_file << finalNofiticationMsg;
	
	nofitication_file.close();
	setUnseenNotification(number,isNew);
}

void setUnseenNotification(string number,int isNew){ // if new is 1 set notification to 0 else +1
	if (!(isNew == -1 || isNew == 0 || isNew == 1)){
		colorize("Invalid Value for 'isNew' Available Values[-1,0,1] \n","red");
		colorize("if New Account set its value to 1\n","red");
		colorize("if old Account, and want to increase unseen notification number set its value to 0","red");
		colorize("if old Account, and want to set unseen notification to 0 set its vlaue to -1","red");
		exit(0);
	}
	char filename[50] = "notifications/unseen_notifications.txt";
	if (isNew == 1){		
		ofstream fp(filename,ios::app);
		fp << number << " 1"<<endl; // 1 = new account so unseen notification is set to 1
		fp.close();
	}
	else{//increase number of unseen notification by 1 if isNew == 0 (not new account)
		int lines = countLinesInFile(filename);
		char tempFile[50] = "notifications/temp.txt",chunk[100]; 
		ifstream org(filename);
		ofstream temp(tempFile);

		int unseenNum;
		string mob;

		for (int i=1;i<=lines;i++){
			org >> mob >> unseenNum;
			if (mob == number){
				if (isNew == 0){
					sprintf(chunk,"%s %d\n",mob.c_str(),unseenNum+1);
				}
				else{
					sprintf(chunk,"%s 0\n",mob.c_str());
				}
			}
			else{
				sprintf(chunk,"%s %d\n",mob.c_str(),unseenNum);
				
			}
			temp << chunk;
			
		}
		org.close();
		temp.close();
		string tempfile = "notifications/temp.txt";
		string orgfile = "notifications/unseen_notifications.txt";
		removeAndRename(tempFile,orgfile);
	}	
}

int removeAndRename(string tempFile,string originalFile){
	remove(originalFile.c_str());
	rename(tempFile.c_str(),originalFile.c_str());
	return 1;
}

int countLinesInFile(string filename){
	int lines = 0;
	char c;
	ifstream fopen(filename.c_str());
	while(!fopen.eof()){
		fopen.get(c);
		if (c=='\n'){
			lines ++;
		}
	}
	fopen.close();
	return lines-1;
}

void toHtml(){
	int noOfCols=7;
	char cols[7][50] = {"id","A/C no","name","mobile number","gender","age","DOB"},chunk[200];
	int counter = countLinesInFile("details/customerdetails.txt");
	ifstream fp;
	ofstream html;
	fp.open("details/customerdetails.txt"); // file to read and write can be passed as argument
	html.open("index.html");
	char row[noOfCols][20]; // will contain all values of row
	// writing html
	html << "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t";
	// fprintf(html, "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t");
	html << "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x' crossorigin='anonymous'>";
	html <<  "\n\t\t<title>User Details</title>\n\t\t<style>\n\t\t\ttable{overflow:hidden;}tr{transition:0.3s;}tbody tr:hover{transform:scale(1.03);box-shadow: 4px 3px 8px 1px #969696;}\n\t\t</style>\n\t</head>\n<body>\n";
	html << "<div class='container'><h2 class='text-center'>User Details</h2>";
	html << "<table class='table table-striped text-center'>\n\t<thead>\n\t\t<tr id='thead'>";
	for (int i=0;i<noOfCols;i++){
		html << "\n\t\t\t<th>" << cols[i] << "</th>";// for table heading with custom col name
	}
	html << "\n\t\t</tr>\n\t</thead>\n\t<tbody id='tbody'>\n\t";
	char name[50];
	for (int i=1; i<=counter;i++){
		fp >> row[0] >> row[1] >> row[2] >> row[3] >> row[4] >> row[5] >> row[6];
		sprintf(name,"%s %s",row[1],row[2]);
		sprintf(chunk,"\t<tr>\n\t\t\t<td>%d</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t</tr>\n\t",i,row[0],name,row[3],row[4],row[5],row[6]);
		html << chunk;
	}
	html <<  "\n\t</tbody>\n</table>\n";
	html << "<button onclick='getJson()' class='btn btn-primary'> get json</button>\n";
	html << "<script src='https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/2.0.0/FileSaver.min.js' integrity='sha512-csNcFYJniKjJxRWRV1R7fvnXrycHP6qDR21mgz1ZP55xY5d+aHLfo9/FcGDQLfn2IfngbAHd8LdfsagcCqgTcQ==' crossorigin='anonymous' referrerpolicy='no-referrer'></script>";
	html << "\n<script  src='https://cdn.jsdelivr.net/gh/raseekshrestha/cdn/extract.js'></script>\n</body>\n</html>";
	fp.close();
	html.close();
	colorize("Successfully exported to index.html\n","green");
}

int depositMoney(string mobileNumber,float amount){
	if (mobileNumberExists(mobileNumber)){
		addBalance(mobileNumber,amount);
		return 1;
	}
	else{
		colorize("Given Mobile number doesn't exist in our database\n","red");
		return 0;
	}
}

void addBalance(string mobileNumber,float amount){
	char filename[30] = "balance/allbalances.txt",chunk[200];
	int lines = countLinesInFile(filename);
	// char ac[15],num[15];
	string num,ac;
	float balance;
	ifstream fp(filename);
	ofstream tempfile("balance/temp.txt");
	for (int i=1;i<=lines;i++){
		fp >> ac >> num >> balance;
		if (mobileNumber == num ){
			sprintf(chunk,"%s %s %f\n",ac.c_str(),num.c_str(),balance+amount);
		}
		else{
			sprintf(chunk,"%s %s %f\n",ac.c_str(),num.c_str(),balance);
			
		}
		tempfile << chunk;
	}
	fp.close();
	tempfile.close();
	remove(filename);
	rename("balance/temp.txt",filename);

}

void superNotification(char msg[]){
	char customersFile[30] = "details/customerdetails.txt";
	int lines = countLinesInFile(customersFile);
	char ac[20],fname[20],lname[20],num[20],gender[10],dob[20];
	int age;
	ifstream fp(customersFile);
	for (int i=1;i<=lines;i++){
		fp >> ac >> fname >> lname >> num >> gender >> age >> dob;
		sendNotification(msg,num,0);
	}
	fp.close();
}

void userDashboard(){
	clear();
	if (firstLogin==1){
		cout << "welcome to user dashboard, " << colorizeReturn(currentUser,"magenta")<< endl;
		cout << "====================================\n";
		cout << "Looks like you are a new user\n";
		cout << "Secure you account by changing password and transaction pin\n";
		firstTimeLogin();
	}
	else{
		cout << "welcome to user dashboard, "<< colorizeReturn(currentUser,"magenta") <<endl;
		cout << "=====================================\n";
	}
	cout << "1. Balance Query\n";
	cout << "2. Transfer\n";
	cout << "3. Notification ("<< noOfUnseenNotification() << " new)\n";
	cout << "4. Security\n";
	cout << "5. Exit\n";
	int choice = askForNumber(1,5);
	if (choice == 1){
		clear();
		cout << "Your balance is Rs. " << checkBalance(currentUserMobile) ;
	}
	else if (choice == 2){
		clear();
		char number[20],message[100];
		float amount;
		cout << "Receiver's Mobile: "; cin >> number;
		cout << "Amount: "; cin >> amount;
		while (1){
			cout << "Transaction pin: ";
			int txPin = askForNumber(1000,9999);
			if (txPin == currentUserPin){
				break;
			}
			else{
				colorize("Incorrect Pin\n","red");
			}
		}
		transferMoney(number,amount);
	}
	else if (choice == 3){
		clear();
		showNotifications();
	}
	else if (choice ==4){
		clear();
		colorize("Security Panel\n","magenta");
		colorize("==============\n","magenta");
		cout << "1. Change Password\n";
		cout << "2. Change Pin\n";
		cout << "3. Back\n";
		int ch = askForNumber(1,3);
		if (ch==1){
			changePasswordOrPin("password");
		}
		else if (ch==2){
			changePasswordOrPin("pin");
		}
		else{
			clear();
			userDashboard();
		}
		
	}else{
		exit(0);
	}
	cout << endl;
	system("pause");
	clear();
	userDashboard();

}

int changePasswordOrPin(string choice){
	int lines = countLinesInFile("login/users.txt"),firstLogin,pin;
	string number,orgpass,fname;
	ifstream fp("login/users.txt");
	for (int i=1;i<=lines;i++){
		fp >> number >> orgpass >> fname >> firstLogin >> pin;
		if (number == currentUserMobile){
			break;
		}
	}
	string oldPass,newPass,confirmPass;
	int oldPin,newPin;
	while (1){
		if (choice == "password"){
			cout << "Old pass\t: ";
			oldPass = askPassword();

			if (oldPass == orgpass){
				break;
			}
			else{
				colorize("\nIncorrect Old password\n","red");
			}	
		}
		else if (choice == "pin"){
			cout << "Old Pin\t:";
			oldPin = askForNumber(1000,9999);
			if (oldPin == pin){
				break;
			}
			else{
				colorize("\nIncorrect Old pin\n","red");
			}

		}
	}
	while (1){
		if (choice == "password"){
			cout <<  "\nNew pass\t: ";
			newPass = askPassword();
			cout << "\nConfirm pass\t: ";
			confirmPass = askPassword();
			if (newPass == confirmPass){
				break;
			}
			else{
				colorize("\n\nNew Password and Confirm Password doesn't match\n","red");
			}	
		}
		else if (choice == "pin"){
			cout << "\nNew Pin\t: ";
			newPin = askForNumber(1000,9999);
			currentUserPin = newPin;
			break;
		}
	}
	fp.seekg(0);
	ofstream temp("login/tempusers.txt");
	for (int i=1;i<=lines;i++){
		fp >> number >> orgpass >> fname >> firstLogin >> pin ;
		if (number == currentUserMobile){
			if (choice == "password"){
				temp << number << " " << newPass << " " << fname << " " << firstLogin << " " << pin << endl;
			}
			else if (choice == "pin"){
				temp << number << " " << orgpass << " " << fname << " " <<  firstLogin << " " << newPin << endl;
			}
		}
		else{
			temp << number << " " << orgpass << " " << fname << " " <<  firstLogin << " " << pin << endl;
		}
	}
	fp.close();
	temp.close();
	char msg[1000];
	if (removeAndRename("login/tempusers.txt","login/users.txt")){
		sprintf(msg,"\n%s changed Successfully\n",choice.c_str());
		colorize(msg,"green");
		sprintf(msg,"%s Changed Successfully, if you didn't request a new %s contact nearest branch immediately",choice.c_str(),choice.c_str());
		sendNotification(msg,currentUserMobile,0);
	}
	else{
		colorize("\nError Occured\n","red");
	}
}

void showNotifications(){
	char filename[100],eachLine[200];
	char ch;
	sprintf(filename,"notifications/%s.txt",currentUserMobile.c_str());
	int lines = countLinesInFile(filename);
	ifstream fp(filename);
	for (int i=1;i<=lines;i++){
		while (!fp.eof()){
			fp.get(ch);
			cout << ch;
		}

	}
	setUnseenNotification(currentUserMobile,-1);
}

float checkBalance(string number){
	int lines = countLinesInFile("balance/allbalances.txt");
 	ifstream fp("balance/allbalances.txt");
 	char line[60];
 	char acFromFile[15];
 	float userBalance;
	string userNumber;
	for (int i=1;i<=lines;i++){
		fp >> acFromFile >> userNumber >> userBalance;
		if (userNumber == number){
			fp.close();
			return userBalance;
		}
	}
 	fp.close();
 }

int transferMoney(string toMobile,float amount){
	if (toMobile == currentUserMobile){
		colorize("You don't need to send money to yourself. Try other number.","red");
		return 0;
	}
	if (mobileNumberExists(toMobile)){
		float balance = checkBalance(currentUserMobile);
		char message[100],filename[30];
		if (balance >= amount){
			if (depositMoney(toMobile,amount) && depositMoney(currentUserMobile,-1*amount)){
				// disp success message after function returns 
				sprintf(message,"transferred Rs.%.2f to %s",amount,toMobile.c_str());
				colorize(message,"green");
				cout << endl;
				
				//sending notification to both sender and receiver
				sendNotification(message,currentUserMobile,0); //sender
				sprintf(message,"Received Rs.%.2f from %s",amount,currentUserMobile.c_str());
				sendNotification(message,toMobile,0); //receiver
				return 1;
			}
		}
		else{
			colorize("Insufficient balance\n","red");
		}
	}
	else{
		colorize("given Mobile number doesn't exists in out database\n","red");
	}
	return 0;
}

void firstTimeLogin(){
	string newPass,confirmPass;
	int pin,linesInUsersTxt;
	fflush(stdin);
	while (1){
		printf("Enter New Password : ");
		newPass = askPassword();
		printf("\nEnter Confirm Pass : ");
		confirmPass = askPassword();
		if (newPass == confirmPass){
			break;
		}
		else{
			colorize("\nNew password and Confirm Password doesn't match\nTry again\n","red");
		}
	}
	printf("\nenter 4 digit pin code. ");
	string mobile,pass,fname;
	int fLogin,eachUserPin;
	pin = askForNumber(1000,9999);
	ifstream usersFile("login/users.txt");
	// usersFile = fopen("login/users.txt","r");
	ofstream tempFile("login/temp.txt");
	// tempFile  = fopen("login/temp.txt","w");
	linesInUsersTxt = countLinesInFile("login/users.txt");
	for(int i=1;i<=linesInUsersTxt;i++){
		usersFile >> mobile >> pass >> fname >> fLogin >> eachUserPin;
		if (currentUserMobile == mobile){
			tempFile << mobile << " " << newPass << " " << fname << " " << 0 << " " << pin << endl;
		}
		else{
			tempFile << mobile << " " << pass << " " << fname << " " << fLogin << " " << eachUserPin << endl;
		}
	}
	// fclose(usersFile);
	// fclose(tempFile);
	usersFile.close();
	tempFile.close();
	if (remove("login/users.txt") == 0 && rename("login/temp.txt","login/users.txt") ==0){
		colorize("New Password and Transaction Pin Updated Successfully\n","green");
		colorize("Login with new credential to continue","white");
		sendNotification("password and pin Changed",currentUserMobile.c_str(),0);
	}
	else{
		colorize("unable to remove the file\n","red");
		sendNotification("failed to remove file",currentUserMobile.c_str(),0);
	}
	
	exit(0);
}

string askPassword(){
	char password[50];
	#ifdef __WIN32
	int i=0;
	do{
		password[i] = getch();		
		if (password[i]!=13 && password[i]!=8){
			printf("*");
		}
		if (password[i] == 8){
			i--;
		}else{
			i++;
		}
	}while(password[i-1]!=13);
	password[i-1] = '\0';
	#endif
	#ifdef linux
	cin >> password;
	#endif
	string pass(password);
	return pass;
}

int noOfUnseenNotification(){
	ifstream notify("notifications/unseen_notifications.txt");
	int lines = countLinesInFile("notifications/unseen_notifications.txt");
	string number;
	int unseenNum;
	for (int i=1;i<=lines;i++){
		notify >> number >> unseenNum;
		if (number == currentUserMobile){
			notify.close();
			return unseenNum;
		}
	}
	notify.close();
	return 0;
}
