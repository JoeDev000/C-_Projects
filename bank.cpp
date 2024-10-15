#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

const string filename_clients = "clients.txt";
const string filename_users = "users.txt";

struct stClientInfo{
  string accountNumber, name, pinCode, phoneNumber;
  double balance;
  bool deleteClient = false;
};

struct stUserInfo{
  string username, password;
  int permissions;
  bool deleteUser = false;
};

void MainMenu(stUserInfo);
void ManageUsersMenu();

void backToMainMenu() {
  cout << "\nPress Any Key to go to the Main Menu...";
  system("pause>0");
}

void backToTransactionMenu() {
  cout << "\nPress Any Key to go to Transactions Menu...";
  system("pause>0");
} 

void backToManageUsersMenu() {
  cout << "\nPress Any Key to go to Manage Users Menu...";
  system("pause>0");
}

string readAccountNumber() {
string accountNumber;
  cout << "Entre Account Number: ";
  cin >> accountNumber;
  return accountNumber;
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

//convert string vector to struct vector
//v is a splited string
stClientInfo lineToStruct_clients(string line) {
vector<string> vLine = splitStr(line);
stClientInfo c; //client
  c.accountNumber = vLine[0];
  c.pinCode = vLine[1];
  c.name = vLine[2];
  c.phoneNumber = vLine[3];
  c.balance = stod(vLine[4]);
  return c;
}

stUserInfo lineToStruct_users(string line) {
vector<string> vLine = splitStr(line);
stUserInfo u; //user
  u.username = vLine[0];
  u.password = vLine[1];
  u.permissions = stoi(vLine[2]);
  return u;
}

void loadAllClientsFromFileToVector(vector<stClientInfo> &vAllClients) {
vAllClients.clear(); //Any pravious data should be cleared before loading data from file
fstream file;
string line;
  file.open(filename_clients, ios::in);
  if (file.is_open()) 
    while (getline(file, line)) 
      vAllClients.push_back(lineToStruct_clients(line));
  file.close();
}

void printSingleClientInfo(stClientInfo c, bool header = false) {
  if (header) {
    cout << "\n -----------------------------------------------------------------------------------------------\n";
    cout << "| Account Number | PIN code |         Client Name         |        Phone        |     Balance   |\n";
    cout << " -----------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(16) << " " + c.accountNumber << "|";
    cout << left << setw(10) << " " + c.pinCode << "|";
    cout << left << setw(29) << " " + c.name << "|";
    cout << left << setw(21) << " " + c.phoneNumber << "|";
    cout << " " << left << setw(14) << c.balance << "|\n";
    cout << " ------------------------------------------------------------------------------------------------\n";
  }
  else {
    cout << "|" << left << setw(16) << " " + c.accountNumber << "|";
    cout << left << setw(10) << " " + c.pinCode << "|";
    cout << left << setw(29) << " " + c.name << "|";
    cout << left << setw(21) << " " + c.phoneNumber << "|";
    cout << " " << left << setw(14) << c.balance << "|\n";
  }
}

void printTable(vector<stClientInfo> vAllClients) {
  cout << "\n -----------------------------------------------------------------------------------------------\n";
  cout << "| Account Number | PIN code |         Client Name         |        Phone        |     Balance   |\n";
  cout << " -----------------------------------------------------------------------------------------------\n";
  for (stClientInfo& client : vAllClients)
    printSingleClientInfo(client);
  cout << " ------------------------------------------------------------------------------------------------\n";
}

int find(vector<stClientInfo> vAllClients, string accountNumber) {
  for (int i = 0; i < vAllClients.size(); i++) 
    if (accountNumber == vAllClients[i].accountNumber) {
      return i;
    }
  return -1;
}

void findClient() {
vector<stClientInfo> vAllClients;
  loadAllClientsFromFileToVector(vAllClients);
string accountNumber;
int pos;
  system("cls");
  cout << "Entre Account Number: ";
  cin >> accountNumber;
  if ((pos = find(vAllClients, accountNumber)) == -1) {
    cout << "Client is NOT found\n";
    backToMainMenu();
  }
  else {
    cout << "Client is Found\n";
    printSingleClientInfo(vAllClients[pos], true);
    backToMainMenu();
  }
}

//input new clinet
stClientInfo readClientInfo(string accounNumber) {
stClientInfo newClinet;
  cout << "----------------------\n";
  newClinet.accountNumber = accounNumber;
  cout << "Name: ";
  getline(cin >> ws, newClinet.name);
  cout << "PIN code: ";
  cin >> newClinet.pinCode;
  cout << "Phone Number: ";
  getline(cin >> ws, newClinet.phoneNumber);
  cout << "Balance: ";
  cin >> newClinet.balance;
  return newClinet;
}

string joinStructVariableMembers_Clients(stClientInfo client, string dlim = "#//#") {
string s = "";
  s += client.accountNumber + dlim;
  s += client.pinCode + dlim;
  s += client.name + dlim;
  s += client.phoneNumber + dlim;
  s += to_string(client.balance);
  return s;
}

void uploadDataToFile(string line, string filename) {
fstream file;
  file.open(filename, ios::out | ios::app);
  if (file.is_open()) 
    file << line << endl;
  file.close();
}

void AddNewClients() {
char responce = 'y';
string accountNumber;
vector<stClientInfo> vAllClients;
  loadAllClientsFromFileToVector(vAllClients);
  while (responce == 'y' || responce == 'Y') {
    system("cls");
    cout << "====== New Client ======\n";
    cout << "Entre Account Number: ";
    cin >> accountNumber;
    if (find(vAllClients, accountNumber) == -1) {
      uploadDataToFile(joinStructVariableMembers_Clients(readClientInfo(accountNumber)), filename_clients);
      cout << "\nClients Added Successfully\n";
    }
    else
      cout << "Account Number Is Already Exist, Can't be Added\n";
    cout << "Entre Another Clinet? (y/n): ";
    cin >> responce;
  }
  backToMainMenu();
} 

void showClients() {
vector<stClientInfo> vAllClients;
loadAllClientsFromFileToVector(vAllClients);
  system("cls");
  if (vAllClients.size() == 0) {
    cout << "There is NO Clients in the DataBase\n";
    backToMainMenu();
    return;
  }
  cout << "\t\t\t\t--- [" << vAllClients.size() << "] Client(s) ---";
  printTable(vAllClients);
  backToMainMenu();
}

void uploadAllValidClientsToFile(vector<stClientInfo> vAllClients) {
fstream file;
  file.open(filename_clients, ios::out);
  if (file.is_open()) 
    for (stClientInfo & c : vAllClients) 
      if (c.deleteClient == false) 
        file << joinStructVariableMembers_Clients(c) << endl;
  file.close();
}

void deleteClientt() {
string accountNumber;
int pos;
char responce;
vector<stClientInfo> vAllClients;
  loadAllClientsFromFileToVector(vAllClients);
  system("cls");
  cout << "Entre the Account Number of the Client you want to delete: ";
  cin >> accountNumber;
  if ((pos = find(vAllClients, accountNumber)) == -1) {
    cout << "The Client your are looking for does NOT exist\n";
    backToMainMenu();
  }
  else {
    cout << "\n\t\t\t\t---- Client Info ----";
    printSingleClientInfo(vAllClients[pos], true);
    cout << "Are You Sure You want to delete this Client? (y/n): ";
    cin >> responce;
    if (responce == 'n' || responce == 'N') {
      cout << "Client Not Deleted";
      backToMainMenu();
      return;
    }
    vAllClients[pos].deleteClient = true;
    uploadAllValidClientsToFile(vAllClients);
    cout << "\nClient Deleted Successfully\n";
    backToMainMenu();
  }
}

void uploadAllClientsToFile_Updated(vector<stClientInfo> vAllClients) {
fstream file;
  file.open(filename_clients, ios::out);
  if (file.is_open()) 
    for (stClientInfo & c : vAllClients) 
        file << joinStructVariableMembers_Clients(c) << endl;
  file.close();
}

void updateClientInfo(stClientInfo &client) {
  cout << "-------------\n";
  cout << "PIN code: ";
  cin >> client.pinCode;
  cout << "Name: ";
  getline(cin >> ws, client.name);
  cout << "Phone Number: ";
  cin >> client.phoneNumber;
  cout << "Balance: ";
  cin >> client.balance; 
}

void updateClient() {
string accounNumber;
vector<stClientInfo> vAllClients;
int pos;
  loadAllClientsFromFileToVector(vAllClients);
  system("cls");
  cout << "Entre Client Account Number you wnat to update: ";
  cin >> accounNumber;
  if ((pos = find(vAllClients, accounNumber)) == -1) {
    cout << "the Client you are looking for doen NOT Exist\n";
    backToMainMenu();
  }
  else {
    cout << "\n\t\t\t\t---- Client Current Info ----";
    printSingleClientInfo(vAllClients[pos], true);
    updateClientInfo(vAllClients[pos]);
    uploadAllClientsToFile_Updated(vAllClients);
    cout << "\nClient Updated Successfully\n";
    backToMainMenu();
  }
}

bool thereIsEnoughBalance(double balance, double amount) {
  return balance >= amount;
}

bool confirm() {
char c = 'y';
  cout << "\nAre you sure you want to confirm this Transaction? (y/n): ";
  cin >> c;
  return c == 'y' || c == 'Y';
}

void updateAccountBalance(stClientInfo &client, bool add) {
double amount;
  //deposit action
  if (add) {
    cout << "Put the Amount you want to Deposit: ";
    cin >> amount;
    if (confirm()) {
      client.balance += amount;
      cout << "Balance Updated Successfuly, your current Balance is: " << client.balance << endl;
      backToTransactionMenu();
    }
    else {
      cout << "Balance does NOT Updated, your current Balance is: " << client.balance << endl;
      backToTransactionMenu();
    }
  }
  //withdraw action
  else {
    cout << "Put the Amount you want to Withdraw: ";
    cin >> amount;
    if (!thereIsEnoughBalance(client.balance, amount)) {
      cout << "There is no Enough Balance (your Balance is: " << client.balance << ")\n";
      backToTransactionMenu();
      return;
    }
    //there is enough balance
    else {
      if (confirm()) {
        client.balance -= amount;
        cout << "Balance Updated Successfuly, your current Balance is: " << client.balance << endl;
        backToTransactionMenu();
      }
      else {
        cout << "Balance does NOT Updated, your current Balance is: " << client.balance << endl;
        backToTransactionMenu();
      }
    }
  }
}

void printScreen(string mode) {
  cout << "--------------------------------\n";
  cout << "\t" << mode << " Screen\n";
  cout << "--------------------------------\n";
}

void deposit() {
vector<stClientInfo> vAllClients;
short pos;
string accountNumber;
  loadAllClientsFromFileToVector(vAllClients);
  system("cls");
  printScreen("Deposit");
  accountNumber = readAccountNumber();
  while(true) {
    if ((pos = find(vAllClients, accountNumber)) == -1) {
      cout << "Account Number does NOT Exist\n";
      accountNumber = readAccountNumber();
    }
    else {
      printSingleClientInfo(vAllClients[pos], true);
      updateAccountBalance(vAllClients[pos], true); //deposit mode
      uploadAllClientsToFile_Updated(vAllClients);
      return;
    }
  }
}

void withdraw() {
vector<stClientInfo> vAllClients;
short pos;
string accountNumber;
  loadAllClientsFromFileToVector(vAllClients);
  system("cls");
  printScreen("Withdraw");
  accountNumber = readAccountNumber();
  while(true) {
    if ((pos = find(vAllClients, accountNumber)) == -1) {
      cout << "Account Number does NOT Exist\n";
      accountNumber = readAccountNumber();
    } 
    else {
      printSingleClientInfo(vAllClients[pos], true);
      updateAccountBalance(vAllClients[pos], false); //withdraw mode
      uploadAllClientsToFile_Updated(vAllClients);
      return;
    }
  }
}

void printSingleClientBalance (stClientInfo c) {
    cout << "|" << left << setw(16) << " " + c.accountNumber << "|";
    cout << left << setw(29) << " " + c.name << "|";
    cout << " " << left << setw(14) << c.balance << "|\n";
}

double printBalancesTableAndReturnSumOfBalances(vector<stClientInfo> vAllClients) {
double totalBalnces = 0;
  cout << "\n -------------------------------------------------------------\n";
  cout << "| Account Number |         Client Name         |     Balance   |\n";
  cout << " -------------------------------------------------------------\n";
  for (stClientInfo& client : vAllClients) {
    printSingleClientBalance(client);
    totalBalnces += client.balance;
  }
  cout << " -------------------------------------------------------------\n";
  return totalBalnces;
}

void showAllBalances() {
vector<stClientInfo> vAllClients;
  loadAllClientsFromFileToVector(vAllClients);
  system("cls");
double totalBalances = printBalancesTableAndReturnSumOfBalances(vAllClients);
  cout << "\n--------------\n";
  cout << "Total Balances = " << totalBalances << endl;
  backToTransactionMenu();
}

short printMainMenuAndReturnChoice() {
short choice;
  system("cls");
  cout << "======= Main Menu =======" << endl;
  cout << "[1] Show List" << endl;
  cout << "[2] Add New Client" << endl;
  cout << "[3] Delete Client" << endl;
  cout << "[4] Update Client" << endl;
  cout << "[5] Find Client" << endl;
  cout << "[6] Transactions Menu" << endl;
  cout << "[7] Manage Users Menu" << endl;
  cout << "[8] Show Current User" << endl;
  cout << "[9] Logout" << endl;
  cout << "-------------------------" << endl;
  cout << "Choose a section: ";
  cin >> choice;
  return choice;
}

short printTransactionsMenuAndReturnChoice() {
short choice;
  system("cls");
  cout << "======= Transactions Menu =======" << endl;
  cout << "[1] Deposit" << endl;
  cout << "[2] Withdraw" << endl;
  cout << "[3] Total Balacnes" << endl;
  cout << "[4] Main Menu" << endl;
  cout << "Choose a section: ";
  cin >> choice;
  return choice;
}

void transactionsMenu() {
short choice;
  while(true) {
    choice = printTransactionsMenuAndReturnChoice();
    if (choice == 4)
      break;
    else if (choice == 1)
      deposit();
    else if (choice == 2)
      withdraw();
    else if (choice == 3)
      showAllBalances();
  }
}

void printUsersHeader() {
  cout << "\t\t--- User(s) Info ---";
  cout << "\n -------------------------------------------------\n";
  cout << "| Username            " << " | Password  " << " | Permissions |\n";
  cout << " -------------------------------------------------\n";
}

void showUserInfo(stUserInfo u, bool header = 1, bool underHeader = 1) {
  if (header) 
    printUsersHeader();
  cout << "| " << left << setw(20) << u.username << " | " << left << setw(10) << u.password << " | " << left << setw(11) << u.permissions << " |\n";
  if (underHeader)
    cout << " -------------------------------------------------\n\n";
} 

void showCurrentUser(stUserInfo user) {
  system("cls");
  showUserInfo(user);
  backToMainMenu();
} 

vector<short> getAccessedSections(short permissionss) {
vector<short> vPermissionss;
short bit;
  for (int i = 0; i < 7; i++) {
    bit = pow(2, i);
    if ((permissionss & bit) != 0) 
      vPermissionss.push_back(i + 1);
  }
  return vPermissionss;
}

bool HasPermissions(vector<short> vPermissionss, short sectionNumber) {
  for (short & p : vPermissionss)
    if (p == sectionNumber) return true;
  return false;
}

void MainMenu(stUserInfo user) {
short choice = 0;
vector<short> vPermissionss = getAccessedSections(user.permissions);
  while(true) {
    choice = printMainMenuAndReturnChoice();
    if (choice == 9)
      break;
    if (choice == 1)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 1))
          showClients();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 2)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 2)) 
          AddNewClients();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 3)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 3))
          deleteClientt();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 4)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 4))
          updateClient();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 5)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 5))
        findClient();  //if the elemnt is found return it's pos (index), if not return -1
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }        
    else if (choice == 6)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 6))
          transactionsMenu();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 7)
      if (user.permissions == -1 || HasPermissions(vPermissionss, 7))
          ManageUsersMenu();
      else {
        cout << "\nAccess Denied, Contact Your Adminstration\n";
        backToMainMenu();
      }
    else if (choice == 8)
      showCurrentUser(user);
  }
}

void loadAllUsersFromFileToVector(vector<stUserInfo> &vAllUsers) {
vAllUsers.clear(); //Any pravious data should be cleared before loading data from file
fstream file;
string line;
  file.open(filename_users, ios::in);
  if (file.is_open()) 
    while (getline(file, line)) 
      vAllUsers.push_back(lineToStruct_users(line));
  file.close();
}

int find_user(vector<stUserInfo> vAllUsers, string username) {
  for (int i = 0; i < vAllUsers.size(); i++) 
    if (username == vAllUsers[i].username) {
      return i;
    }
  return -1;
}

short checkUsername(vector<stUserInfo> vAllUsers, string username) {
  return find_user(vAllUsers, username);
}

bool checkPassword(stUserInfo user, string password) {
  return user.password == password;
}

bool read_check_binaryChoice(string message, bool firstTime = 0) {
  if (firstTime) return true;
char binaryChoice;
  cout << message;
  cin >> binaryChoice;
  while (binaryChoice != 'y' && binaryChoice != 'Y' && binaryChoice != 'n' && binaryChoice != 'N') {
    cout << "Invalid Input, try again\n";
    cout << "Choice (y/n): ";
    cin >> binaryChoice;
  }
  return binaryChoice == 'y' || binaryChoice == 'Y';
}

void login() {
stUserInfo user;
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
short pos;
bool firstTime = 1;
  while (read_check_binaryChoice("======= Login Menu =======\n[y] Login Again\n[n] Exit\n", firstTime)) {
    firstTime = 0;
    system("cls");
    cout << "======= Login Menu =======" << endl;
    cout << "Username: ";
    cin >> user.username;
    cout << "Password: ";
    cin >> user.password;
    if ((pos = checkUsername(vAllUsers, user.username)) == -1 || !checkPassword(vAllUsers[pos], user.password)) 
      cout << "Username or Password is NOT correct, plz try again or Exit\n";
    else {
      user.permissions = vAllUsers[pos].permissions;
      cout << "\n\nLogged In Successfully\n";
      cout << "\nPress Any Key To Go To Main Menu...\n";
      system("pause>0");
      MainMenu(user);
    }
      loadAllUsersFromFileToVector(vAllUsers); //for the next loop all users should be up-to-date
      system("cls");
  }
}

short printManageUsersMenuAndReturnChoice() {
short choice;
  system("cls");
  cout << "======= Manage Users Menu =======" << endl;
  cout << "[1] List Users" << endl;
  cout << "[2] Add New User" << endl;
  cout << "[3] Delete User" << endl;
  cout << "[4] Update User" << endl;
  cout << "[5] Find User" << endl;
  cout << "[6] Back To Main Menu" << endl;
  cout << "-------------------------" << endl;
  cout << "Choose a section: ";
  cin >> choice;
  return choice;
}

void listUsers() {
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
  system("cls");
  printUsersHeader();
  for (stUserInfo & u : vAllUsers) 
    showUserInfo(u, 0, 0);
  cout << " -------------------------------------------------\n\n";
  backToManageUsersMenu();
}

short getUserPermissions() {
  if (read_check_binaryChoice("Give Access To All Permissionss (y/n): \n")) return -1;
short permissionss = 0;
string messages[] {"Give Access to Show Clients (y/n): \n", 
                   "Give Access to Add New Clients (y/n): \n", 
                   "Give Access to Delete Client (y/n): \n", 
                   "Give Access to Update Client (y/n): \n", 
                   "Give Access to Find Client (y/n): \n", 
                   "Give Access to Transactions (y/n): \n", 
                   "Give Access to Manage Users (y/n): \n"};

  for (int i = 0; i <= 6; i++) 
    if (read_check_binaryChoice(messages[i])) permissionss += pow(2, i);  //permissions += pow(2, section number - 1)
  
  return permissionss;
}

//input new user
stUserInfo readUserInfo(string username) {
stUserInfo newUser;
  cout << "----------------------\n";
  newUser.username = username;
  cout << "Password: ";
  getline(cin >> ws, newUser.password);
  newUser.permissions = getUserPermissions();
  return newUser;
}

string joinStructVariableMembers_Users(stUserInfo u, string dlim = "#//#") {
string s = "";
  s += u.username + dlim;
  s += u.password + dlim;
  s += to_string(u.permissions);
  return s;
}

void AddNewUsers() {
bool firstTime = 1;
string username;
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
  while (read_check_binaryChoice("Entre Another Clinet? (y/n): ", firstTime)) {
    firstTime = 0;
    system("cls");
    cout << "====== New User ======\n";
    cout << "Entre Username: ";
    getline(cin >> ws, username);
    if (find_user(vAllUsers, username) == -1) {
      uploadDataToFile(joinStructVariableMembers_Users(readUserInfo(username)), filename_users);
      cout << "\nUser Added Successfully\n";
    }
    else
      cout << "Account Number Is Already Exist, Can't be Added\n";
  }
  backToMainMenu();
} 

string readUsername(string message) {
string u;
  cout << message;
  cout << "Username: ";
  cin >> u;
  return u;
}

void uploadAllValidUsersToFile(vector<stUserInfo> vAllUsers) {
fstream file;
  file.open(filename_users, ios::out);
  if (file.is_open()) 
    for (stUserInfo & c : vAllUsers) 
      if (c.deleteUser == false) 
        file << joinStructVariableMembers_Users(c) << endl;
  file.close();
}

void changeUserInfo(stUserInfo &u) {
  cout << "\n----------------\n";
  cout << "New Password: ";
  getline(cin >> ws, u.password);
  u.permissions = getUserPermissions();
}

void deleteUser() {
bool firstTime = 1;
int pos;
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
  while (read_check_binaryChoice("Delete Another User (y/n): ", firstTime)) {
    firstTime = 0;
    system("cls");
    if ((pos = find_user(vAllUsers, readUsername("Entre Username You Want To Delete\n"))) != -1) {
      showUserInfo(vAllUsers[pos]);
      if (read_check_binaryChoice("Are You Sure You Want To Delete this User? (y/n): ")) {
        vAllUsers[pos].deleteUser = true;
        cout << "\nUser Deleted Successfuly\n\n";
      }
      else
        cout << "User Is NOT Deleted\n";
    }
    else {
      cout << "Username Is Incorrect, plz try again\n";
      if (read_check_binaryChoice("Try Again? (y/n): "))
        firstTime = 1;
    }
  }
  uploadAllValidUsersToFile(vAllUsers);
  backToManageUsersMenu();
}

void updateUser() {
bool firstTime = 1;
int pos;
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
  while (read_check_binaryChoice("Update Another User (y/n): ", firstTime)) {
    firstTime = 0;
    system("cls");
    if ((pos = find_user(vAllUsers, readUsername("Entre Username You Want To Update\n"))) != -1) {
      showUserInfo(vAllUsers[pos]);
      if (read_check_binaryChoice("Are You Sure You Want To Update this User? (y/n): ")) {
        changeUserInfo(vAllUsers[pos]);
        cout << "\nUser Updated Successfuly\n\n";
      }
      else
        cout << "User Is NOT Updated\n";
    }
    else {
      cout << "Username Is Incorrect, plz try again\n";
      if (read_check_binaryChoice("Try Again? (y/n): "))
        firstTime = 1;
    }
  }
  uploadAllValidUsersToFile(vAllUsers);
  backToManageUsersMenu();
}

void findUser() {
int pos;
bool firstTime = 1;
vector<stUserInfo> vAllUsers;
  loadAllUsersFromFileToVector(vAllUsers);
  while (read_check_binaryChoice("Find Another User? (y/n): ", firstTime)) {
    firstTime = 0;
    system("cls");
    if ((pos = find_user(vAllUsers, readUsername("Entre Username You Want To Find\n"))) != -1) {
      cout << "\nUser Found :)\n\n";
      showUserInfo(vAllUsers[pos]);
    }
    else 
      cout << "\nUser NOT Found :(\n\n";
  }
  backToManageUsersMenu();
}

void ManageUsersMenu() {
short choice;
  while (true) {
    choice = printManageUsersMenuAndReturnChoice();
    if (choice == 6)
      break;
    else if (choice == 1)
      listUsers();
    else if (choice == 2)
      AddNewUsers();
    else if (choice == 3)
      deleteUser();
    else if (choice == 4)
      updateUser();
    else if (choice == 5)
      findUser();
  }
}

int main() {
  login();
  system("cls");
return 0;
}

//delete6
//  read from file -> search -> highlight -> rewrite the file
//change info
//  read from file -> search -> change -> rewrite the file