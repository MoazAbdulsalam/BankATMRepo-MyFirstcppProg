
#include <iostream>  
#include <string>  
#include <vector>  
#include <fstream>  
#include <limits>  
#include <iomanip>  
using namespace std;
struct stUser
{
    string username;
    string pasword;
    int permissions;
    bool MarkForDelete = false;
};
struct stClient
{
    string AccountNum;
    string PinCode;
    string Name;
    string Phone;
    bool MarkForDelete = false;

    double AccountBalance;
};

enum enMainMenueOption { eShowClientList = 1, eAddNewClient, eDeleteClient, eUpdateClient, eFindClient, eTransAction, eManageUsers, eLogOut };
enum enTransaction { Deposit = 1, WithDraw, TotalBalances };
enum enManageUsers { eListUsers = 1, eAddNewUser, eDeleteUser, eUpdateUser, eFindUser, eMainmenue };
enum enPermissions {
    eAllAccess = -1,
    eShowClientListPermission = 1,
    eAddNewClientPermission = 2,
    eDeleteClientPermission = 4,
    eUpdateClientPermission = 8,
    eFindClientPermission = 16,
    eTransActionPermission = 32,
    eManageUsersPermission = 64
};
const string ClientsFile = "ClientFile.txt";
const string UserFile = "UserFile.txt";
stUser CurrentUser;

void Login();
void ShowMainMenu();
void ShowTransActionScrean();
void ShowManageUserScrean();
vector<stClient> LoadDataFromFileToVector(string);


void BackToMinue()
{
    cout << " Press any key to go back to main minue...";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}
void BackToTransactionMenue()
{
    cout << "Press any key to go back to Transaction minue...";
    cin.ignore();
    cin.get();
    ShowTransActionScrean();
}
void BackManageUserScrean()
{
    cout << "Press any key to go back to Manage User minue...";
    cin.ignore();
    cin.get();
    ShowManageUserScrean();

}

void printAccessDenied()
{

    cout << "\t\t\t\t\t! Access Denied !\n";
    cout << "\t\t\t\t-- Please contact your admin --\n\n";
    BackToMinue();
}
bool CheckPermision(enPermissions Permission)
{
    if (CurrentUser.permissions == eAllAccess)
        return true;
    return(CurrentUser.permissions & Permission) == Permission;
}

void PrintClientRecordForPrintAllClientFunction(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNum;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData()
{
    if (!CheckPermision(eShowClientListPermission))
    {
        printAccessDenied();
        return;
    }

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stClient& client : vClients)
    {
        PrintClientRecordForPrintAllClientFunction(client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


}

vector<string> Split(string s1, string delim)
{
    vector<string>words;
    short pos = 0;
    string sword;
    while ((pos = s1.find(delim)) != string::npos)
    {
        sword = s1.substr(0, pos);
        if (sword != "")
        {
            words.push_back(sword);
        }
        s1.erase(0, pos + delim.length());
    }
    if (s1 != "")
    {
        words.push_back(s1);
    }
    return words;
}

stClient ConvertLinetoRecord(string s1, string seperator = "#//#")
{
    stClient client;
    vector<string>vstring = Split(s1, seperator);
    client.AccountNum = vstring[0];
    client.PinCode = vstring[1];
    client.Name = vstring[2];
    client.Phone = vstring[3];
    client.AccountBalance = stod(vstring[4]);
    return client;
}

stUser ConvertLinetostUserRecord(string s1, string seperator = "#//#")
{
    vector<string>vstring = Split(s1, seperator);
    stUser User;
    User.username = vstring[0];
    User.pasword = vstring[1];
    User.permissions = stoi(vstring[2]);
    return User;
}

string ConvertUserRecordToLine(stUser User, string seperator = "#//#")
{
    string UserRecord = "";
    UserRecord += User.username + seperator;
    UserRecord += User.pasword + seperator;
    UserRecord += to_string(User.permissions);
    return UserRecord;
}

vector<stClient> LoadDataFromFileToVector(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        stClient client;
        string Line;
        while (getline(MyFile, Line))
        {
            client = ConvertLinetoRecord(Line);
            vClients.push_back(client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<stUser>LoadDataFromUserFileToVector(string FileName)
{
    vector<stUser> users;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        stUser User;
        string Line;
        while (getline(MyFile, Line))
        {
            User = ConvertLinetostUserRecord(Line);
            users.push_back(User);
        }
        MyFile.close();
    }
    return users;
}

string ConvertRecordToLine(stClient client, string seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += client.AccountNum + seperator;
    stClientRecord += client.PinCode + seperator;
    stClientRecord += client.Name + seperator;
    stClientRecord += client.Phone + seperator;
    stClientRecord += to_string(client.AccountBalance);
    return stClientRecord;
}

bool IsExistSearchByAccountNumber(string AccNum)
{
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    for (stClient& client : vClients)
    {
        if (client.AccountNum == AccNum)
        {
            return true;
        }
    }
    return false;
}

stClient GetClient()
{
    string accnum = "";
    stClient Client;
    cout << "Enter Account Number? ";
    cin >> Client.AccountNum;
    while (IsExistSearchByAccountNumber(Client.AccountNum))
    {
        cout << "Client with [" << Client.AccountNum << "] Already exist ,Enter another one :";
        cin >> Client.AccountNum;
    }
    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void AddDataLineToFile(string fileName, string Line)
{
    fstream ClientFile;
    ClientFile.open(fileName, ios::out | ios::app);
    if (ClientFile.is_open())
    {
        ClientFile << Line << endl;
        ClientFile.close();
    }
}

void AddNewClient()
{
    stClient Client = GetClient();
    AddDataLineToFile(ClientsFile, ConvertRecordToLine(Client));
}

void AddClients(vector<stClient>& vClients)
{
    char AddMore = 'Y';
    do
    {

        cout << "\nAdding New Client: \n\n";
        AddNewClient();
        vClients = LoadDataFromFileToVector(ClientsFile);
        cout << "Client Added successfully ,doyou Want To Add More?";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');


}

void AddNewClientScrean()
{
    if (!CheckPermision(eAddNewClientPermission))
    {
        printAccessDenied();
    }
    else
    {
        cout << "----------------------------------\n";
        cout << "      Add New Client Screan\n";
        cout << "----------------------------------\n";
        vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
        AddClients(vClients);
    }
}

void PrintClientRecord(stClient Client)
{
    cout << "\n\nThe following is the extracted client record:\n";
    cout << "------------------------------------------------------\n";
    cout << "\nAccout Number: " << Client.AccountNum;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n------------------------------------------------------\n";

}

void SaveDataToFile(string FileName, vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string Line;
        for (stClient& c : vClients)
        {
            if (c.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(c);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}

bool SearchByAccNum(string AccNum, vector<stClient>& Allclients, stClient& Client)
{
    for (stClient& client : Allclients)
    {
        if (client.AccountNum == AccNum)
        {
            Client = client;
            return true;
        }
    }
    return false;
}

void MarkClaintForDeleting(string AccNum, vector<stClient>& Allclients)
{
    for (stClient& c : Allclients)
    {
        if (AccNum == c.AccountNum)
        {
            c.MarkForDelete = true;
        }
    }
}

bool  DeleteSearchByAccNum(string AccNum, vector<stClient>& Allclients)
{
    char answer;
    stClient Client;
    if (SearchByAccNum(AccNum, Allclients, Client))
    {
        PrintClientRecord(Client);
        cout << "\nAreYouSure ";
        cin >> answer;
        if (tolower(answer) == 'y')
        {
            MarkClaintForDeleting(AccNum, Allclients);
            SaveDataToFile(ClientsFile, Allclients);
            Allclients = LoadDataFromFileToVector(ClientsFile);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccNum << ") Not Found\n";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteScrean()
{
    if (!CheckPermision(eDeleteClientPermission))
    {
        printAccessDenied();
        return;
    }

    cout << "----------------------------------\n";
    cout << "         Delete Screan\n";
    cout << "----------------------------------\n";

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    DeleteSearchByAccNum(AccountNumber, vClients);

}

stClient ChangeClientRecord(string accnum)
{
    stClient Client;
    Client.AccountNum = accnum;
    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;

}

bool  UpdateClientByAccNum(string AccNum, vector<stClient>& Allclients)
{
    char answer;
    stClient Client;
    if (SearchByAccNum(AccNum, Allclients, Client))
    {
        PrintClientRecord(Client);
        cout << "\nAreYouSure ";
        cin >> answer;
        if (tolower(answer) == 'y')
        {
            for (stClient& c : Allclients)
            {
                if (c.AccountNum == AccNum)
                {
                    c = ChangeClientRecord(AccNum);
                    break;
                }
            }
            SaveDataToFile(ClientsFile, Allclients);
            cout << "\n\nClient Updated Successfully.";

            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccNum << ") Not Found\n";
        return false;
    }
}

void ShowUpdateScrean()
{
    if (!CheckPermision(eUpdateClientPermission))
    {
        printAccessDenied();
        return;
    }
    cout << "----------------------------------\n";
    cout << "         Update Screan\n";
    cout << "----------------------------------\n";
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccNum(AccountNumber, vClients);
}

void ShowFindClientScrean()
{
    if (!CheckPermision(eFindClientPermission))
    {
        printAccessDenied();
        return;
    }

    cout << "----------------------------------\n";
    cout << "         Update Screan\n";
    cout << "----------------------------------\n";

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    if (SearchByAccNum(AccountNumber, vClients, Client))
    {
        PrintClientRecord(Client);
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found\n";
}

bool  DepositClientByAccNum(string AccNum, double Amount, vector<stClient>& Allclients)
{
    char answer = 'n';
    cout << "\nAre you sure ?";
    cin >> answer;
    if (answer == 'Y' || answer == 'y')
    {
        for (stClient& c : Allclients)
        {
            if (c.AccountNum == AccNum)
            {
                c.AccountBalance += Amount;
                SaveDataToFile(ClientsFile, Allclients);
                cout << "\n\nDone Successfully. New balance is: " << c.AccountBalance << endl;;
                return true;
            }
        }
    }
    return false;
}

void DepositScrean()
{
    cout << "----------------------------------\n";
    cout << "         Deposit Screan \n";
    cout << "----------------------------------\n";
    stClient Client;
    double Amount;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();

    while (!SearchByAccNum(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientRecord(Client);
    cout << "Pls Enter deposit Amount :";
    cin >> Amount;
    DepositClientByAccNum(AccountNumber, Amount, vClients);

}

void WithDrawScrean()
{

    cout << "----------------------------------\n";
    cout << "         Withdraw Screan \n";
    cout << "----------------------------------\n";
    stClient Client;
    double Amount;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    string AccountNumber = ReadClientAccountNumber();

    while (!SearchByAccNum(AccountNumber, vClients, Client));
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientRecord(Client);
    cout << "Pls Enter Withdraw Amount :";
    cin >> Amount;
    while (Amount > Client.AccountBalance)
    {
        cout << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Pls Enter Another Amount :";
        cin >> Amount;
    }
    DepositClientByAccNum(AccountNumber, Amount * -1, vClients);

}

void PrintClientBalance(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNum;

    cout << "| " << setw(40) << left << Client.Name;

    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintTotalBalances()
{
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFile);
    double totalBalances = 0;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";

    cout << "| " << left << setw(40) << "Client Name";

    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stClient& client : vClients)
    {
        totalBalances += client.AccountBalance;
        PrintClientBalance(client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________" << endl;
    cout << "\t\t\t\t Total Balances :" << totalBalances << endl;

}

void TransactionOparetion(enTransaction op)
{
    switch (op)
    {
    case Deposit:
        system("cls");
        DepositScrean();
        BackToTransactionMenue();
        break;
    case WithDraw:
        system("cls");
        WithDrawScrean();
        BackToTransactionMenue();
        break;
    case TotalBalances:
        system("cls");
        PrintTotalBalances();
        BackToTransactionMenue();
        break;
    default:
        ShowMainMenu();
        break;
    }
}

enTransaction Input()
{
    short num;
    cout << "Choose What do you want do to :";
    cin >> num;
    return (enTransaction)num;
}

void ShowTransActionScrean()
{
    if (!CheckPermision(eTransActionPermission))
    {
        printAccessDenied();
        return;
    }

    system("cls");
    cout << "============================================\n";
    cout << "\tTransaction Menue Screan\n";
    cout << "============================================\n";
    cout << "\t[1] Deposit. \n";
    cout << "\t[2] Withdraw. \n";
    cout << "\t[3] TotalBalances. \n";
    cout << "\t[4] MainMinue. \n";
    cout << "============================================\n";

    TransactionOparetion(Input());
}

void PrintUserRecordForPrintAllUsersFunction(stUser User)
{
    cout << "| " << setw(15) << left << User.username;
    cout << "| " << setw(10) << left << User.pasword;
    cout << "| " << setw(40) << left << User.permissions;

}
void PrintAllUserssData()
{
    vector<stUser>Users = LoadDataFromUserFileToVector(UserFile);

    cout << "\n\t\t\t\t\ User List (" << Users.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permission";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (Users.size() == 0)
    {
        cout << "\nNo Users Found.\n";
    }
    else
    {
        for (stUser& User : Users)
        {
            PrintUserRecordForPrintAllUsersFunction(User);
            cout << endl;
        }
    }



    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

bool IsUserExistSearByUserName(string UserName)
{
    vector<stUser> vUsers = LoadDataFromUserFileToVector(UserFile);
    for (stUser& User : vUsers)
    {
        if (User.username == UserName)
        {
            return true;
        }
    }
    return false;
}

void GetPermission(stUser& User)
{
    User.permissions = 0;
    char access = 'n';
    cout << "Do You Want To Give Full Access?";
    cin >> access;
    if (tolower(access) == 'y')
        User.permissions = eAllAccess;
    else
    {
        cout << "Do you Want to give access to : \n";
        cout << "show Client List?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eShowClientListPermission;

        cout << "Add New Client?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eAddNewClientPermission | User.permissions;

        cout << "Delete Client?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eDeleteClientPermission | User.permissions;

        cout << "Update Client?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eUpdateClientPermission | User.permissions;

        cout << "Find Client?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eFindClientPermission | User.permissions;

        cout << "Transactions?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eTransActionPermission | User.permissions;

        cout << "Manage Users?";
        cin >> access;

        if (tolower(access) == 'y')
            User.permissions = eManageUsersPermission | User.permissions;

    }

}

stUser GetUser()
{
    stUser User;

    cout << "Enter UserName :";
    getline(cin >> ws, User.username);
    while (IsUserExistSearByUserName(User.username))
    {
        cout << "Useer with [" << User.username << "] Already exist ,Enter another username :";
        getline(cin >> ws, User.username);

    }
    cout << "Enter Password : ";
    getline(cin, User.pasword);
    GetPermission(User);
    return User;


}

void AddNewUser()
{
    stUser User = GetUser();
    AddDataLineToFile(UserFile, ConvertUserRecordToLine(User));

}

void AddUsers()
{
    char AddMore = 'Y';
    do
    {

        cout << "\nAdding New User: \n\n";
        AddNewUser();
        //vUsers = LoadDataFromUserFileToVector(UserFile);
        cout << "User Added successfully ,do you Want To Add More?";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');


}

void AddNewUserScrean()
{
    cout << "----------------------------------\n";
    cout << "      Add New User Screan\n";
    cout << "----------------------------------\n";
    // vector<stUser> Users = LoadDataFromUserFileToVector(UserFile);
    AddUsers();
}

void PrintUserRecord(stUser User)
{
    cout << "\n\nThe following is the extracted User record:\n";
    cout << "------------------------------------------------------\n";
    cout << "\nUser Name   : " << User.username;
    cout << "\nPassword    : " << User.pasword;
    cout << "\nPermission  : " << User.permissions;
    cout << "\n------------------------------------------------------\n";

}
void SaveUserDataToFile(string FileName, vector<stUser>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string Line;
        for (stUser& c : vUsers)
        {
            if (c.MarkForDelete == false)
            {
                Line = ConvertUserRecordToLine(c);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}
bool SearchUserByUserName(string UserName, vector<stUser> vUsers, stUser& User)
{
    for (stUser& U : vUsers)
    {
        if (UserName == U.username)
        {
            User = U;
            return true;
        }
    }
    return false;
}
void MarkUserForDelete(string UserName, vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (UserName == U.username)
        {
            U.MarkForDelete = true;
        }
    }
}
bool DeleteUser(string UserNAme, vector<stUser>& vUsers)
{
    char answer;
    stUser User;
    if (SearchUserByUserName(UserNAme, vUsers, User))
    {
        PrintUserRecord(User);
        cout << "\nAreYouSure ";
        cin >> answer;
        if (tolower(answer) == 'y')
        {
            MarkUserForDelete(UserNAme, vUsers);
            SaveUserDataToFile(UserFile, vUsers);
            vUsers = LoadDataFromUserFileToVector(UserFile);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with (" << UserNAme << ") Not Found\n";
        return false;
    }
}
void DeletUserScrean()
{
    cout << "----------------------------------\n";
    cout << "         Delete User Screan\n";
    cout << "----------------------------------\n";
    vector<stUser> vUsers = LoadDataFromUserFileToVector(UserFile);
    string UserName;
    cout << "Enter User Name?";
    cin >> UserName;
    if (UserName == "Admin")
    {
        cout << "\n YOU CANNOT DELETE THIS USER\n";
        return;
    }
    DeleteUser(UserName, vUsers);
}

stUser UpdateUserQuestions(stUser User)
{
    cout << "Enter Password : ";
    getline(cin >> ws, User.pasword);
    GetPermission(User);
    return User;
}
bool UpdateUserByUserName(string username, vector<stUser>& vUsers)
{
    stUser User;
    char answer = 'n';
    if (SearchUserByUserName(username, vUsers, User))
    {
        PrintUserRecord(User);
        cout << "\nAre You Sure ";
        cin >> answer;
        if (tolower(answer) == 'y')
        {
            for (stUser& U : vUsers)
            {
                if (username == U.username)
                {
                    U = UpdateUserQuestions(U);
                    break;
                }
            }
            SaveUserDataToFile(UserFile, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with (" << username << ") Not Found\n";
        return false;
    }
}
void ShowUserUpdateScrean()
{
    cout << "----------------------------------\n";
    cout << "       User Update Screan\n";
    cout << "----------------------------------\n";
    vector<stUser>vUsers = LoadDataFromUserFileToVector(UserFile);
    string UserName;
    cout << "Enter User Name?";
    cin >> UserName;
    UpdateUserByUserName(UserName, vUsers);


}

bool FindUserByUserName(string UserName, vector<stUser>vUsers)
{
    stUser User;
    if (SearchUserByUserName(UserName, vUsers, User))
    {
        PrintUserRecord(User);
        return true;
    }
    else
    {
        cout << "\nUser with (" << UserName << ") Not Found\n";
        return false;
    }
}

void ShowFindUserScrean()
{
    cout << "----------------------------------\n";
    cout << "       Find User Screan\n";
    cout << "----------------------------------\n";
    vector<stUser>vUsers = LoadDataFromUserFileToVector(UserFile);
    string UserName;
    cout << "Enter User Name?";
    cin >> UserName;
    FindUserByUserName(UserName, vUsers);
}

void ManageUsersOperation(enManageUsers op)
{
    switch (op)
    {
    case eListUsers:
        system("cls");
        PrintAllUserssData();
        BackManageUserScrean();
        break;
    case eAddNewUser:
        system("cls");
        AddNewUserScrean();
        BackManageUserScrean();
        break;
    case eDeleteUser:
        system("cls");
        DeletUserScrean();
        BackManageUserScrean();
        break;
    case eUpdateUser:
        system("cls");
        ShowUserUpdateScrean();
        BackManageUserScrean();
        break;
    case eFindUser:
        system("cls");
        ShowFindUserScrean();
        BackManageUserScrean();
        break;
    case eMainmenue:
        ShowMainMenu();
        break;
    default:
        break;
    }
}

enManageUsers ManageUserInput()
{
    short num;
    cout << "Choose What do you want do to :";
    cin >> num;
    return (enManageUsers)num;
}

void ShowManageUserScrean()
{
    if (!CheckPermision(eManageUsersPermission))
    {
        printAccessDenied();
        return;
    }

    system("cls");
    cout << "============================================\n";
    cout << "\tManage Users Menue Screan\n";
    cout << "============================================\n";
    cout << "\t[1] List Users. \n";
    cout << "\t[2] Add New User. \n";
    cout << "\t[3] Delete User. \n";
    cout << "\t[4] Update User. \n";
    cout << "\t[5] Find User. \n";
    cout << "\t[6] Main Menue. \n";
    cout << "============================================\n";
    ManageUsersOperation(ManageUserInput());


}

void Choice(enMainMenueOption option)
{
    switch (option)
    {
    case eShowClientList:
        system("cls");
        PrintAllClientsData();
        BackToMinue();
        break;
    case eAddNewClient:
        system("cls");
        AddNewClientScrean();
        BackToMinue();
        break;
    case eDeleteClient:
        system("cls");
        ShowDeleteScrean();
        BackToMinue();
        break;
    case eUpdateClient:
        system("cls");
        ShowUpdateScrean();
        BackToMinue();
        break;
    case eFindClient:
        system("cls");
        ShowFindClientScrean();
        BackToMinue();
        break;
    case eTransAction:
        system("cls");
        ShowTransActionScrean();
        break;
    case eManageUsers:
        system("cls");
        ShowManageUserScrean();
        break;
    case eLogOut:
        system("cls");
        Login();
        break;
    }
}

enMainMenueOption UserInput()
{
    short num = 0;
    cin >> num;

    return (enMainMenueOption)num;
}

void ShowMainMenu()
{
    system("cls");
    cout << "============================================\n";
    cout << "\tMain Minue Screan\n";
    cout << "============================================\n";
    cout << "\t[1] Show Client List. \n";
    cout << "\t[2] Add New Client. \n";
    cout << "\t[3] Delete Client. \n";
    cout << "\t[4] Update Client info. \n";
    cout << "\t[5] Fined Client.  \n";
    cout << "\t[6] Transaction. \n";
    cout << "\t[7] Manage Users. \n";
    cout << "\t[8] LogOut. \n";
    cout << "============================================\n";
    cout << "What do you want ?";
    Choice(UserInput());

}

bool FindUserByUserNameAndPassword(string username, string password, stUser& currentUser)
{
    vector<stUser> vUsers = LoadDataFromUserFileToVector(UserFile);
    for (stUser& User : vUsers)
    {
        if (User.username == username && User.pasword == password)
        {
            currentUser = User;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string username, string password)
{
    if (FindUserByUserNameAndPassword(username, password, CurrentUser))
    {
        return true;
    }
    return false;
}

void Login()
{

    bool LoginFieled = false;
    string username, password;
    do
    {
        system("cls");
        cout << "----------------------------------\n";
        cout << "         Login Screan \n";
        cout << "----------------------------------\n";
        if (LoginFieled)
        {
            cout << "Invalid Username/Password, Please try again.\n";
        }
        cout << "Entar User Name ?";
        cin >> username;
        cout << "Entar Password?";
        cin >> password;
        LoginFieled = !LoadUserInfo(username, password);

    } while (LoginFieled);
    ShowMainMenu();
}

void AdminSignin(string FileName)
{
    fstream file;
    if (IsUserExistSearByUserName("Admin"))
        return;
    file.open(FileName, ios::out);

    stUser user;
    string DataLine;

    user.username = "Admin";
    user.pasword = "1234";
    user.permissions = eAllAccess;

    DataLine = ConvertUserRecordToLine(user);

    if (file.is_open())
    {
        file << DataLine << endl;
        file.close();
    }
}

int main()
{
    //user name : Admin  Password : 1234    
    AdminSignin(UserFile);
    Login();
}