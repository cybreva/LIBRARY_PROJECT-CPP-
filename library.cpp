#include <iostream>
#include <vector>
#include <ctime>
#include<fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

string trim(string s)
{
    while(!s.empty() && s.front() == ' ')
        s.erase(0, 1);

    while(!s.empty() && s.back() == ' ')
        s.pop_back();

    return s;
}

struct librariandata{
    string librarian_name;
    int librarian_id ;
    int librarian_pin;
};

struct userdata{
     string user_name;
     string book_name;

};

struct bookdata
{
    string book_name;
    string genre;
    int rental_price;
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
        cout << "PLEASE ENTER YOUR ID : ";
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
string get_current_datetime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    return to_string(ltm->tm_mday) + "-" +
           to_string(ltm->tm_mon + 1) + "-" +
           to_string(ltm->tm_year + 1900) + " " +
           to_string(ltm->tm_hour) + ":" +
           to_string(ltm->tm_min) + ":" +
           to_string(ltm->tm_sec);
}
bool book_exists(string search_book)
{
    ifstream file("books.csv");

    string line;

    while(getline(file, line))
    {
        stringstream ss(line);

        string name, genre, price;

        getline(ss, name, ',');
        getline(ss, genre, ',');
        getline(ss, price, ',');

        if(trim(name) == trim(search_book))
        {
            return true;
        }
    }

    return false;
}

string add_3_days()
{
    time_t now = time(0);
    now += 3 * 24 * 60 * 60;

    tm *ltm = localtime(&now);

    stringstream ss;

    ss << setfill('0') << setw(2) << ltm->tm_mday << "-"
       << setw(2) << ltm->tm_mon + 1 << "-"
       << ltm->tm_year + 1900 << " "
       << setw(2) << ltm->tm_hour << ":"
       << setw(2) << ltm->tm_min << ":"
       << setw(2) << ltm->tm_sec;

    return ss.str();
}
void issue_book()
{
    userdata user;

    cout << "Enter User Name : ";
    cin.ignore();
    getline(cin, user.user_name);

    cout << "Enter Book Name : ";
    getline(cin, user.book_name);

    if(!book_exists(user.book_name))
    {
        cout << "BOOK NOT FOUND IN LIBRARY \n";
        return;
    }

    // 🔥 ISSUE TIME
    string issue_time = get_current_datetime();

    // 🔥 DUE TIME (3 days later)
    string due_time = add_3_days();

    // 🔥 WRITE TO FILE
    ofstream file("transactions.csv", ios::app);

    file << user.user_name << ","
         << user.book_name << ","
         << issue_time << ","
         << due_time << ","
         << "ISSUED"
         << "\n";

    file.close();

    cout << "Book Issued Successfully!\n";
}
void return_book()
{
    ifstream file("transactions.csv");
    ofstream temp("temp.csv");

    string line;

    string user, book;

    cout << "Enter User Name: ";
    cin >> user;

    cout << "Enter Book Name: ";
    cin.ignore();
    getline(cin, book);

    while(getline(file, line))
    {
        stringstream ss(line);

        string u,b,issue,due,status;

        getline(ss,u,',');
        getline(ss,b,',');
        getline(ss,issue,',');
        getline(ss,due,',');
        getline(ss,status,',');

        if(u == user && b == book && status == "ISSUED")
        {
            status = "RETURNED";
        }

        temp << u << "," << b << "," << issue << "," << due << "," << status << "\n";
    }

    file.close();
    temp.close();

    remove("transactions.csv");
    rename("temp.csv","transactions.csv");

    cout << "Book Returned Successfully\n";
}
void view_records()
{
    ifstream file("transactions.csv");

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
    ifstream file("transactions.csv");

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
int get_next_copy_number(string book_name)
{
    ifstream file("books.csv");
    string line;

    int max_copy = 0;

    while(getline(file, line))
    {
        stringstream ss(line);

        string name, copy, genre, price;

        getline(ss, name, ',');
        getline(ss, copy, ',');
        getline(ss, genre, ',');
        getline(ss, price, ',');

        if(name == book_name)
        {
            int c = stoi(copy);
            if(c > max_copy)
                max_copy = c;
        }
    }

    return max_copy + 1;
}
void add_book()
{
    bookdata book;

    cout << "Enter Book Name : ";
    cin.ignore();
    getline(cin, book.book_name);

    cout << "Enter Genre : ";
    getline(cin, book.genre);

    if(book.genre == "programming" || book.genre == "educational")
        book.rental_price = 0;
    else
    {
        cout << "Enter Rental Price : ";
        cin >> book.rental_price;
    }

    int copy_no = get_next_copy_number(book.book_name);

    ofstream file("books.csv", ios::app);

    file << book.book_name << ","
         << copy_no << ","
         << book.genre << ","
         << book.rental_price
         << "\n";

    file.close();

    cout << "Book Added Successfully! Copy #" << copy_no << "\n";
}
void check_overdue_books()
{
    ifstream file("transactions.csv");
    string line;

    time_t now = time(0);

    cout << "\nOVERDUE BOOKS:\n";

    while(getline(file,line))
    {
        stringstream ss(line);

        string u,b,issue,due,status;

        getline(ss,u,',');
        getline(ss,b,',');
        getline(ss,issue,',');
        getline(ss,due,',');
        getline(ss,status,',');

        if(status == "ISSUED")
        {
            // simple skip parsing complexity for now
            cout << u << " -> " << b << " (CHECK MANUALLY)\n";
        }
    }
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

    check_overdue_books();



    
    int choice ;   
    
    do
{
    cout << "\n\n===== LIBRARY MENU =====\n";
    cout << "1. Add Book\n";
    cout << "2. Issue Book\n";
    cout << "3. Return Book\n";
    cout << "4. View Records\n";
    cout << "5. Fetch Record\n";
    cout << "6. Exit\n";

    cout << "\nEnter Your Choice : ";
    cin >> choice;

    switch(choice)
    {
        case 1:
            add_book();
            break;
        case 2:
            issue_book();
            break;
        case 3:
            return_book();
            break;
        case 4:
            view_records();
            break;
        case 5:
            search_record();
            break;
        case 6:
            cout << "\nByy , Have A Nice Day !\n";
            break;
        default:
            cout << "\nInvalid Choice! Try Again.\n";
            break;
    }

} while(choice != 6);;
}