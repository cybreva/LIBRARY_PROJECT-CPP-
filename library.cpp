#include <iostream>
#include <vector>
#include <ctime>
#include<fstream>

using namespace std;

struct librariandata{
    string librarian_name;
    int librarian_id ;
    int librarian_pin;
};

struct userdata{
     string user_name;
     string book_name;

};

string librarian_name_input(){
    librariandata librarian ;
    cout << "PLEASE ENTER YOUR NAME : ";
    cin >> librarian.librarian_name ;
    string &lib_name = librarian.librarian_name ;
    return lib_name;

}
int librarian_id_input()
{
    int lib_id;
    int attempts = 0;

    while(attempts < 3)
    {
        cout << "PLEASE ENTER YOUR ADMIN_ID : ";
        cin >> lib_id;

        if(lib_id == 12345)
        {
            cout << "DATA FETCHED\n";
            return lib_id;
        }

        cout << "NO ACCOUNT OF THIS ID EXIST IN DATABASE\n";
        cout << "IF YOU ARE A NON REGISTERED MEMBER CONTACT TO ADMIN ASAP!\n";

        attempts++;
    }

    return -1;
}
int librarian_pin_input()
{
    int lib_pass;
    int attempts = 0;

    while(attempts < 3)
    {
        cout << "PLEASE ENTER YOUR PIN : ";
        cin >> lib_pass;

        if(lib_pass == 12345)
        {
            cout << "LOGIN SUCCESSFUL\n";
            return lib_pass;
        }

        cout << "INCORRECT PASSWORD!\n";
        cout << "PLEASE CONTACT THE SYSTEM ADMINISTRATOR IF YOU HAVE FORGOTTEN YOUR PASSWORD.\n";

        attempts++;
    }

    return -1;
}
string get_current_date()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    return to_string(ltm->tm_mday) + "-" +
           to_string(ltm->tm_mon + 1) + "-" +
           to_string(ltm->tm_year + 1900);
}

void issue_book()
{
    userdata user;

    cout << "Enter User Name : ";
    cin.ignore();
    getline(cin, user.user_name);


    cout << "Enter Book Name : ";
    getline(cin, user.book_name);

    string issue_date = get_current_date();

    ofstream file("userdata.csv", ios::app);

    file << user.user_name << ","
         << user.book_name << ","
         << issue_date << "\n";

    file.close();

    cout << "Book Issued Successfully!\n";
}
void view_records()
{
    ifstream file("userdata.csv");

    string line;

    cout << "\n===== RECORDS =====\n";

    while(getline(file, line))
    {
        cout << line << endl;
    }

    file.close();
}
void search_record()
{
    ifstream file("userdata.csv");

    string search_name;
    string line;
    bool found = false;

    cout << "\nEnter Name To Search : ";
    cin.ignore();
    getline(cin, search_name);

    while(getline(file, line))
    {
        if(line.find(search_name) != string::npos)
        {
            cout << line << endl;
            found = true;
        }
    }

    if(!found)
    {
        cout << "\nNo Record Found!\n";
    }

    file.close();
}



int main (){
    
    string librarian_name = librarian_name_input();

    int librarian_id = librarian_id_input();

    if(librarian_id == -1)
    {
        cout << "\nACCESS DENIED!\n";
        return 0;
    }

    int librarian_pin = librarian_pin_input();

    if(librarian_pin == -1)
    {
        cout << "\nACCESS DENIED!\n";
        return 0;
    }
    


    cout << "===========================\n";
    cout << "           WELCOME         \n";
    cout << "===========================\n";



    
    int choice ;   
    
    do
{
    cout << "\n\n===== LIBRARY MENU =====\n";
    cout << "1. Issue Book\n";
    cout << "2. View Records\n";
    cout << "3. Fetch Record\n";
    cout << "4. Exit\n";

    cout << "\nEnter Your Choice : ";
    cin >> choice;

    switch(choice)
    {
        case 1:
            issue_book();
            break;

        case 2:
            view_records();
            break;
        case 3 :
            search_record();
            break;
        case 4:
            cout << "\nThank You For Using Library System!\n";
            break;

        default:
            cout << "\nInvalid Choice! Try Again.\n";
            break;
    }

} while(choice != 4);;
}