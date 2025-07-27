
#include <iostream>  
#include <string>  
#include <vector>  
#include <fstream>  
#include <limits>  
#include <iomanip>  
using namespace std;
struct stClient
{
    string AccountNum;
    string PinCode;
    string Name;
    string Phone;
    bool MarkForDelete = false;

    double AccountBalance;
};

enum enMenuOptions
{
	eQiuckWithdraw = 1,
	eNormalWithdraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eLogout = 5,
};

enum enQuickWithdrawOptions
{
    eQuickWithdraw_20 = 1,
    eQuickWithdraw_50 = 2,
    eQuickWithdraw_100 = 3,
    eQuickWithdraw_200 = 4,
    eQuickWithdraw_400 = 5,
	eQuickWithdraw_600 = 6,
    eQuickWithdraw_800 = 7,
    eQuickWithdraw_1000 = 8,
    eExitQuickWithdraw= 9,
};

string ClientFile = "D:\\source\\repos\\problems level 4\\Bank project\\Bank project\\ClientFile.txt";
stClient CurrentClient;
void Login();
void ShowMainMenu();

void BackToMainMenu()
{
    cout << "Press any key to go back to main minue...";
	system("pause>0");
    ShowMainMenu();
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

stClient ConvertLinetoRecort(string s1, string seperator = "#//#")
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
            client = ConvertLinetoRecort(Line);
            vClients.push_back(client);
        }
        MyFile.close();
    }
    return vClients;
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

bool DepositClient( double Amount, vector<stClient>& Allclients)
{
    char answer = 'n';
    cout << "\nAre you sure ?";
    cin >> answer;
    if (answer == 'Y' || answer == 'y')
    {
        for (stClient& c : Allclients)
        {
            if (c.AccountNum == CurrentClient.AccountNum)
            {
                
                c.AccountBalance += Amount;
                CurrentClient = c;
                SaveDataToFile(ClientFile, Allclients);
                cout << "\n\nDone Successfully. New balance is: " << c.AccountBalance << endl;;
                return true;
            }
        }
    }
    return false;
}

short GeQuickWithdrawAmount(enQuickWithdrawOptions option)
{
    switch (option)
    {
    case eQuickWithdraw_20:
        return 20;
    case eQuickWithdraw_50:
        return 50;
    case eQuickWithdraw_100:
        return 100;
    case eQuickWithdraw_200:
        return 200;
    case eQuickWithdraw_400:
        return 400;
    case eQuickWithdraw_600:
        return 600;
    case eQuickWithdraw_800:
        return 800;
    case eQuickWithdraw_1000:
        return 1000;
    }
}

void QuickWithdrawOperation(enQuickWithdrawOptions option)
{
    if(option == eExitQuickWithdraw)
        return;
	
    short withdrawAmount = GeQuickWithdrawAmount(option);

    if (withdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "Insufficient balance for this withdrawal.\n";
        return;
    }

    vector<stClient>vClients = LoadDataFromFileToVector(ClientFile);
    DepositClient(-withdrawAmount, vClients);

    BackToMainMenu();
}

enQuickWithdrawOptions GetQuickWithdrawOption()
{
    short option;
	cout << "your current balance is: " << CurrentClient.AccountBalance << endl;
	cout << "Please select a quick withdraw option (1-9): ";
    while (true)
    {

        cin >> option;
        if (cin.fail() || option < 1 || option > 9)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option, please try again.\n";
        }
        else
        {
			return (enQuickWithdrawOptions)option;
        }
    }
}

void ShowQuickWithdrawOptions()
{
    system("cls");
    cout << "----------------------------------\n";
    cout << "         Quick Withdraw Options \n";
    cout << "----------------------------------\n";
    cout << "\t[1]. Withdraw 20\n";
    cout << "\t[2]. Withdraw 50\n";
    cout << "\t[3]. Withdraw 100\n";
    cout << "\t[4]. Withdraw 200\n";
    cout << "\t[5]. Withdraw 400\n";
    cout << "\t[6]. Withdraw 600\n";
    cout << "\t[7]. Withdraw 800\n";
    cout << "\t[8]. Withdraw 1000\n";
    cout << "\t[9]. Exit Quick Withdraw\n";
    cout << "----------------------------------\n";
	QuickWithdrawOperation(GetQuickWithdrawOption());

}

int GetWithDrawAmount()
{
    int Amount = 0;
    do
    {
        cout << "Enter the amount Multipe of 5: ";
        cin >> Amount;
        if (cin.fail() || Amount <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount, please try again.\n";
        }
    } while (Amount % 5 != 0 || Amount <= 0);
    return Amount;
}

void PerformNormalWithdraw()
{
    int amount = GetWithDrawAmount();

    if (amount > CurrentClient.AccountBalance)
    {
        cout << "Insufficient balance for this withdrawal.\n";
    }

    vector<stClient>vClients = LoadDataFromFileToVector(ClientFile);
    DepositClient(-amount, vClients);

}

void ShowNormalWithdrawOptions()
{
    system("cls");
    cout << "----------------------------------\n";
    cout << "         Normal Withdraw Options \n";
    cout << "----------------------------------\n";
	PerformNormalWithdraw();
   
}

double ReadDepositAmount()
{
    double Amount = 0.0;
    do
    {
        cout << "Enter a Positive amount to deposit: ";
        cin >> Amount;
        if (cin.fail() || Amount <= 0)
        {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid amount, please try again.\n";
        }
    } while (Amount <= 0);
    return Amount;
}

void PerformDepositOption()
{
    double Amount = ReadDepositAmount();
    
    vector<stClient>vClients = LoadDataFromFileToVector(ClientFile);
    DepositClient(Amount, vClients);
}

void ShowDepositScrean()
{
    system("cls");
    cout << "----------------------------------\n";
    cout << "         Deposit Options \n";
    cout << "----------------------------------\n";
    PerformDepositOption();
    
}

void ShowCheckBalanceScrean()
{
	system("cls");
	cout << "----------------------------------\n";
	cout << "         Check Balance Screan \n";
	cout << "----------------------------------\n";
    cout << "Your Current Balance is: " << CurrentClient.AccountBalance << endl;
    
}

void MainMenuOptions(enMenuOptions option)
{
    switch (option)
    {
    case eQiuckWithdraw:
		system("cls");
		ShowQuickWithdrawOptions();
		BackToMainMenu();
        break;

    case eNormalWithdraw:
		system("cls");
		ShowNormalWithdrawOptions();
		BackToMainMenu();
        break;

    case eDeposit:
		system("cls");
		ShowDepositScrean();
		BackToMainMenu();
        break;

    case eCheckBalance:
		system("cls");
		ShowCheckBalanceScrean();
        BackToMainMenu();
        break;

    case eLogout:
        Login();
        break;
   
    }
}

enMenuOptions GetMenuOption()
{
    int option;
    while (true)
    {
       
        cin >> option;
        if (cin.fail() || option < 1 || option > 5)
        {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid option, please try again.\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return static_cast<enMenuOptions>(option);
        }
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "----------------------------------\n";
    cout << "        ATM Main Menu \n";
    cout << "----------------------------------\n";
    cout << "\t1. Quick Withdraw\n";
    cout << "\t2. Normal Withdraw\n";
    cout << "\t3. Deposit\n";
    cout << "\t4. Check Balance\n";
    cout << "\t5. Logout\n";
    cout << "----------------------------------\n";

    cout << "Please select an option: ";
	MainMenuOptions(GetMenuOption());
}

bool FindClientByAccountNumAndPin(string AccountNum, string PIN, stClient& currentClient)
{
    vector<stClient> vClients = LoadDataFromFileToVector(ClientFile);
    for (stClient& C : vClients)
    {
        if (C.AccountNum == AccountNum && C.PinCode == PIN)
        {
            currentClient = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PIN)
{
    if (FindClientByAccountNumAndPin(AccountNumber, PIN, CurrentClient))
    {
        return true;
    }
    return false;
}

void Login()
{

    bool LoginFieled = false;
    string AccountNumber, PIN;
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
        cout << "Entar Account Number ?";
        cin >>AccountNumber ;
        cout << "Entar PIN?";
        cin >> PIN;
        LoginFieled = !LoadClientInfo(AccountNumber, PIN);

    } while (LoginFieled);
	ShowMainMenu();
}

int main()
{
    Login();
}

