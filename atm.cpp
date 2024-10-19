#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const string fileName = "clients.txt";

struct stClientInfo{
  string accountNumber, name, pinCode, phoneNumber;
  double balance;
  bool deleteClient = false;
};

struct stLoginInfo {
  string accountNumber, pinCode;
};

void backToLoginMenu() {
  cout << "Press Any Key To Back To Login Menu...";
  system("pause>0");
}

void backToMainMenu() {
  cout << "Press Any Key To Back To Main Menu...";
  system("pause>0");
}

vector<string> splitStr(string str, string dlim = "#//#") {
int pos;
string word;
vector<string> vWords;
  while ((pos = str.find(dlim)) != string::npos) {
    word = str.substr(0, pos);
    if (word != "")
      vWords.push_back(word);
    str.erase(0, pos + dlim.length());
  }
  if (str != "")
    vWords.push_back(str);
  return vWords;
}

stClientInfo convertLineToStruct(string line) {
stClientInfo client;
vector<string> vClientInfo = splitStr(line);
  client.accountNumber = vClientInfo[0];
  client.pinCode = vClientInfo[1];
  client.name = vClientInfo[2];
  client.phoneNumber = vClientInfo[3];
  client.balance = stod(vClientInfo[4]);
  return client;
}

string convertStructToLine(stClientInfo client, string dlim = "#//#") {
string s = "";
  s += client.accountNumber + dlim;
  s += client.pinCode + dlim;
  s += client.name + dlim;
  s += client.phoneNumber + dlim;
  s += to_string(client.balance);
  return s;
}

void loadAllClientFromFileToVector(vector<stClientInfo> &vAllClients) {
fstream file;
string line;
  file.open(fileName, ios::in);
  if (file.is_open()) {
    while (getline(file, line))
      vAllClients.push_back(convertLineToStruct(line));
  }
  file.close();
}

void uploadUpdatedDataToFile(vector<stClientInfo> vAllClients_updated) {
fstream file;
  file.open(fileName, ios::out);
  if (file.is_open()) {
    for (stClientInfo & c : vAllClients_updated)
    file << convertStructToLine(c) << endl;
  }
  file.close();
}

bool readBooleanInput(string msg, bool firstTime = 0) {
  if (firstTime) return true;
char c;
  while (true) {
    cout << msg;
    cin >> c;
    if (c != 'y' && c != 'Y' && c != 'n' && c != 'N')
      cout << "Invalid Input, Try Again\n";
    else
      break;
  }
  return c == 'y' || c == 'Y';
}

bool checkAccountNumber(string accountNumber, stClientInfo & currentClient) {
vector<stClientInfo> vAllClients;
  loadAllClientFromFileToVector(vAllClients);
  for (stClientInfo & c : vAllClients) {
    if (c.accountNumber == accountNumber) {
      currentClient = c;
      return true;
    }
  }
  return false;
}

bool checkPinCode(string pinCode, stClientInfo currnetClient) {
vector<stClientInfo> vAllClients;
  loadAllClientFromFileToVector(vAllClients);
  return currnetClient.pinCode == pinCode;
}

bool ckeckInputClientInfo(stLoginInfo loginInfo, stClientInfo & currentClient) {
  return checkAccountNumber(loginInfo.accountNumber, currentClient) ? checkPinCode(loginInfo.pinCode, currentClient) : false;
}

stLoginInfo readLoginInfo() {
stLoginInfo loginInfo;
  cout << "Entre Account Number: ";
  cin >> loginInfo.accountNumber;
  cout << "Pin Code: ";
  cin >> loginInfo.pinCode;
  return loginInfo;
}

void printLoginScreen() {
  cout << "------------------------------------\n";
  cout << "\tLogin Screen";
  cout << "\n------------------------------------\n";
}

bool loginSuccess(stClientInfo & currentClient) {
  if (!ckeckInputClientInfo(readLoginInfo(), currentClient)) {
    cout << "\nWrong Account Number or PIN code, try again\n";
    return false;
  }
  return true;
}

short readChoice(short from, short to) {
short choice;
  printf("Your Choice [%i - %i]: ", from, to);
  cin >> choice;
  return choice;
}

short readChoiceInRange(short from, short to) {
short choice = readChoice(from, to);
  while (choice < from || choice > to) {
    cout << "Invalid Choice, try again\n\n";
    choice = readChoice(from, to);
  }
  return choice;
}

void printQuickWithdrawMenu() {
  system("cls");
  cout << "------------------------------------------------\n";
  cout << "\tQuick Withdraw Menu\n";
  cout << "------------------------------------------------\n";
  cout << "[1] 20\t[2] 50\n";
  cout << "[3] 100\t[4] 200\n";
  cout << "[5] 400\t[6] 600\n";
  cout << "[7] 800\t[8] 1000\n";
  cout << "[9] Exit\n";
  cout << "------------------------------------------------\n";
}

short getIndex(string clientAccountNumber) {
vector<stClientInfo> vAllClients;
  loadAllClientFromFileToVector(vAllClients);
  for (int i = 0; i < vAllClients.size(); i++) {
    if (vAllClients[i].accountNumber == clientAccountNumber)
      return i;
  }
  return -1;
}

bool exceeds(double b) {
  return b < 1;
}

//  read from file -> search -> change -> rewrite the file
void quickWithdraw(short index) {
vector<stClientInfo> vAllClinets;
  loadAllClientFromFileToVector(vAllClinets);
int currentBalance = vAllClinets[index].balance;
  printQuickWithdrawMenu();
  cout << "Your Balance Is: " << currentBalance << endl;
short choice = readChoiceInRange(1,9);
  if (choice == 9) return;
int balances[] {currentBalance - 20, 
                currentBalance - 50, 
                currentBalance - 100, 
                currentBalance - 200, 
                currentBalance - 400, 
                currentBalance - 600, 
                currentBalance - 800, 
                currentBalance - 1000};
  if (balances[choice - 1] < 0) 
    cout << currentBalance - balances[choice - 1] << " Exceeds Your Balance\n\n";
  else {
    if (!readBooleanInput("Are You Sure You Want To Confirm This Transaction(y/n): ")) {
      cout << "The Transaction Was Canceled\n\n";
      backToMainMenu();
      return;
    }
    vAllClinets[index].balance = balances[choice - 1]; //update client balance
    uploadUpdatedDataToFile(vAllClinets);
    cout << "\nWithdraw Doen Successfuly\n";
    cout << "Your Current Balance Is: " << vAllClinets[index].balance << "\n\n"; 
  }
  backToMainMenu();
  system("cls");
}

int readWithdrawed_Balance() {
int withdrawed_balance;
  cout << "Entre The Balance You Want To Withdraw [Multiable of 5]: ";
  cin >> withdrawed_balance;
  while (withdrawed_balance % 5 != 0 || withdrawed_balance <= 0) {
    cout << "Invalid Input Balance, Try Again\n";
    cout << "Entre The Balance You Want To Withdraw [Multiable of 5]: ";
    cin >> withdrawed_balance;
  }
  return withdrawed_balance;
}

void normalWithdraw(short index) {
vector<stClientInfo> vAllClinets;
  loadAllClientFromFileToVector(vAllClinets);
int currentBalance = vAllClinets[index].balance;
  system("cls");
  cout << "Your Balance Is: " << currentBalance << "\n\n";
int withdrawed_Balance = readWithdrawed_Balance();
  if (currentBalance - withdrawed_Balance < 0)
    cout << withdrawed_Balance << " Exceeds Your Balance\n\n";
  else {
    if (!readBooleanInput("Are You Sure You Want To Confirm This Transaction(y/n): ")) {
      cout << "The Transaction Was Canceled\n\n";
      backToMainMenu();
      return;
    }
    vAllClinets[index].balance -= withdrawed_Balance; //update client balance
    uploadUpdatedDataToFile(vAllClinets);
    cout << "\nWithdraw Doen Successfuly\n";
    cout << "Your Current Balance Is: " << vAllClinets[index].balance << "\n\n"; 
  }
  backToMainMenu();
  system("cls");
}

short printMainMenuAndReturnChoice() {
  system("cls");
  cout << "------------------------------------\n";
  cout << "\tATM Main Menu Screen";
  cout << "\n------------------------------------\n";
  cout << "[1] Quick Withdraw\n";
  cout << "[2] Normal Withdraw\n";
  cout << "[3] Deposit\n";
  cout << "[4] Check Balance\n";
  cout << "[5] Logout\n";
  cout << "------------------------------------\n";
short choice = readChoiceInRange(1, 5);
  return choice;
}

int readDeposit_Balance() {
int deposit_Balance;
  cout << "Entre The Balance You Want To Deposit [Multiable of 5]: ";
  cin >> deposit_Balance;
  while (deposit_Balance % 5 != 0 || deposit_Balance <= 0) {
    cout << "Wrong Input Balance, Try Again\n";
    cout << "Entre The Balance You Want To Deposit: ";
    cin >> deposit_Balance;
  }
  return deposit_Balance;
}

void deposit(short index) {
vector<stClientInfo> vAllClinets;
  loadAllClientFromFileToVector(vAllClinets);
int currentBalance = vAllClinets[index].balance;
  system("cls");
  cout << "Your Balance Is: " << currentBalance << endl;
int deposit_Balance = readDeposit_Balance();
  if (!readBooleanInput("Are You Sure You Want To Confirm This Transaction(y/n): ")) {
    cout << "The Transaction Was Canceled\n\n";
  }
  else {
    vAllClinets[index].balance += deposit_Balance;
    uploadUpdatedDataToFile(vAllClinets);
    cout << "\nDeposite Doen Successfuly.\n";
    cout << "Your Current Balance Is: " << vAllClinets[index].balance << "\n\n";
  }
  backToMainMenu();
  system("cls");
}

void checkBalance(short index) {
vector<stClientInfo> vAllClients;
  loadAllClientFromFileToVector(vAllClients);
  system("cls");
  cout << "Hi Mr(s) " << vAllClients[index].name << endl;
  cout << "Your Current Balance Is: " << vAllClients[index].balance << "\n\n";
  backToMainMenu();
}

void MainMenu(string clientAccountNumber) {
short choice;
  while (true) {
    choice = printMainMenuAndReturnChoice();
    if (choice == 5)
      break;
    else if (choice == 1)
      quickWithdraw(getIndex(clientAccountNumber));
    else if (choice == 2) 
      normalWithdraw(getIndex(clientAccountNumber));
    else if (choice == 3) 
      deposit(getIndex(clientAccountNumber));
    else if (choice == 4) 
      checkBalance(getIndex(clientAccountNumber));
    system("cls");
  }
}

void login() {
bool firstTime = 1;
stClientInfo currentClient;
  while (readBooleanInput("Login To Another Account(y/n) : ", firstTime)) {
    firstTime = 0;
    system("cls");
    printLoginScreen();
    if (loginSuccess(currentClient)) //takes currentClient by ref and pass it to main menu
      MainMenu(currentClient.accountNumber);
    system("cls");
  }
  system("cls");
}

int main() {
  system("cls");
  login();
return 0;
}

//update client -> search account number - add new clinet to vector - upload vector to file 

/*
void printClientInfo(stClientInfo c) {
  cout << c.accountNumber << endl;
  cout << c.pinCode << endl;
  cout << c.name << endl;
  cout << c.phoneNumber << endl;
  cout << c.balance << endl;
}
*/
