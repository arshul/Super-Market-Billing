//***************************************************************
//                   HEADER FILE USED IN PROJECT
//****************************************************************


#include <termios.h> 
#include <stdio.h> 
#include <cstdlib> 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <unistd.h> 
#include <pqxx/pqxx>
#include <ctime>

using namespace std;
using namespace pqxx;

//***************************************************************
//                   CLASS USED IN PROJECT
//****************************************************************
class product {
  int pno;
  char name[50];
  float price, qty, tax, dis;
  public:
    void create_product() {
      cout<<"\n```````````````````````````````````````````````````````````\n";
      cout << "\nPlease Enter The Product Number: ";
      cin >> pno;
      cout << "\nPlease Enter The Name of The Product: ";
      cin >> name;
      cout << "\nPlease Enter The Price of The Product: ";
      cin >> price;
      cout << "\nPlease Enter The Discount(in %): ";
      cin >> dis;
    }

    void update_product() {
      cout<<"\n```````````````````````````````````````````````````````````";
      cout << "\nPlease Enter The Name of The Product: ";
      cin >> name;
      cout << "\nPlease Enter The Price of The Product: ";
      cin >> price;
      cout << "\nPlease Enter The Discount(in %): ";
      cin >> dis;
    }

  void show_product() {
    cout << "\nThe Product No. of The Product : " << pno;
    cout << "\nThe Name of The Product : " << name;

    cout << "\nThe Price of The Product : " << price;
    cout << "\nDiscount : " << dis;
  }

  int retpno() {
    return pno;
  }

  float retprice() {
    return price;
  }

  char * retname() {
    return name;
  }

  int retdis() {
    return dis;
  }

}; //class ends here

//***************************************************************
//      global declarations
//****************************************************************
connection C("dbname = oops user = postgres password = root \
      hostaddr = 127.0.0.1 port = 5432");
product pr;


//***************************************************************
//      function to authenticate to admin dashboard
//****************************************************************
int auth(){
  string pass = "root";
  string input;
  cout << "\nenter your password: ";
  termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  cin >> input;
  tty.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  if (input==pass){
      return 1;
  }else{
    cout << "Wrong" << endl;
    auth();
  }
}


//***************************************************************
//      function to connect to database
//***************************************************************
int init_database() {
  try {
    if (C.is_open()) {
      cout << "Opened database successfully: " << C.dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception & e) {
    cout << "in exception";
    cerr << e.what() << std::endl;
    return 1;
  }
}


//***************************************************************
//      function to create tables in database
//****************************************************************
int create_tables() {
  string sql;
  sql = "CREATE TABLE PRODUCTS("\
  "ID INT PRIMARY KEY  NOT NULL,"\
  "NAME TEXT NOT NULL,"\
  "price double precision NOT NULL,"\
  "discount double precision );";

  work W(C);
  try {
    W.exec(sql);
    W.commit();
    //  cout << "Table created successfully" << endl;
    return 1;
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 0;
  }
}



//***************************************************************
//      function to insert in table
//****************************************************************
int write_product() {
  string sql;
  pr.create_product();
  sql = "INSERT INTO products (id, name, price, discount) "\
  "VALUES (" + to_string(pr.retpno()) + ",'" + to_string(pr.retname()) + "'," + to_string(pr.retprice()) + "," + to_string(pr.retdis()) + ");";

  /* Create a transactional object. */
  work W(C);
  try {
    W.exec(sql);
    W.commit();
    cout << "  Records created successfully  " << endl;
    cout << "\n\nThe Product Has Been Created ";
    return 1;
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 0;
  }

}

//***************************************************************
//      function to read all records from table
//****************************************************************

int display_all() {
  system("clear");
  string sql;
  cout << "\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n";
  getchar();
  try {

    /* Create SQL statement */
    sql = "SELECT * from products";

    /* Create a non-transactional object. */
    nontransaction N(C);

    /* Execute SQL query */
    result R(N.exec(sql));
    
    /* List down all the records */
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << "PRODUCT_ID = " << c[0].as < int > () << "  |  ";
      cout << "PRODUCT_NAMEN = " << c[1].as < string > () << "  |  ";
      cout << "PRICE = " << c[2].as < float > () << "  |  ";
      cout << "DISCOUNT = " << c[3].as < float > () << "%" << endl;
      cout << "================================================================================\n";

    }
    getchar();
    C.disconnect();
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;

    getchar();
    return 1;
  }

  return 0;
}


//***************************************************************
//      function to read specific record from table
//****************************************************************
void display_sp(int n) {
  try {
  string sql;
  int flag = 0;
  sql = "SELECT * from products WHERE id =" +to_string(n);
  nontransaction N(C);
  result R(N.exec(sql));
  cout<<"====================================================\n";
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << "PRODUCT_ID = " << c[0].as < int > () << "\n";
      cout << "PRODUCT_NAME = " << c[1].as < string > () <<"\n";
      cout << "PRICE = " << c[2].as < float > () << "\n";
      cout << "DISCOUNT(%) = " << c[3].as < float > () << "\n";
      cout << "====================================================\n";
      flag = 1;
    }
    getchar();
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    getchar();
  }
  getchar();
  }

//***************************************************************
//      function to modify record of table
//****************************************************************
int modify_product() {
system("clear");
  try {
  string sql;
   int no;
  cout<<"\n#######################################";
  cout << "\n\tTO MODIFY PRODUCT \n";
  cout<<"#######################################";
  cout << "\n\nPlease Enter The Product Number: ";
  cin >> no;
   sql = "SELECT * from products WHERE id =" +to_string(no);
  nontransaction N(C);
  result R(N.exec(sql));
  cout<<"====================================================\n";
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << "PRODUCT_ID = " << c[0].as < int > () << "\n";
      cout << "PRODUCT_NAME = " << c[1].as < string > () <<"\n";
      cout << "PRICE = " << c[2].as < float > () << "\n";
      cout << "DISCOUNT(%) = " << c[3].as < float > () << "\n";
      cout << "====================================================\n";
    }
    getchar();
    N.commit();
        C.disconnect();

        pr.update_product();
      sql = "UPDATE products set name = '"+ to_string(pr.retname())+ "' , price=" + to_string(pr.retprice()) +",discount=" + to_string(pr.retdis()) + " where id = "+ to_string(no) + ";";

        work W(C);
    W.exec(sql);
    W.commit();
    cout<<"Record Updated";

  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    getchar();
  }

  getchar();
  system("clear");
 
  return 1;
}


//***************************************************************
//      function to delete record of database
//****************************************************************
int delete_product() {
  string sql;
  int no;
  system("clear");
  cout<<"\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
  cout << "\n\n\n\tDELETE RECORD";
  cout<<"\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
  cout << "\n\nPlease Enter The product Number of The Product You Want To Delete : ";
  cin >> no;
  sql = "\nDELETE from products where ID = " + to_string(no) + ";";
  work W(C);
  try {
    W.exec(sql);
    W.commit();
    cout << "\nRecords deleted successfully" << endl;
    cout << "\n\nThe Product Has Been Deleted ";
    //  cout << "Table created successfully"  << endl;
    return 1;
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 0;
  }
  getchar();
}

//***************************************************************
//      function to display all products price list
//****************************************************************
void menu() {
  system("clear");
  string sql;
  sql = "SELECT * from products order by id";
  nontransaction N(C);
  result R(N.exec(sql));
  cout << "\n\n\t\tProduct MENU\n\n";
  cout << "\n====================================================\n";
  cout << "P.NO.\t\tNAME\t\tPRICE\n";
  cout << "====================================================\n";

  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0] << "\t\t" << c[1] << "\t\t" << c[2] << "\n";
  }
  getchar();
}


//***************************************************************
//      function to place order and generating bill for Products
//****************************************************************

void place_order() {
  int order_arr[50], quan[50], c = 0;
  float amt, damt, total = 0;
  char ch = 'Y';
  menu();
  cout << "\n============================";
  cout << "\n    PLACE YOUR ORDER";
  cout << "\n============================\n";
  do {
    cout << "\n\nEnter The Product No. Of The Product : ";
    cin >> order_arr[c];
    cout << "\nQuantity in number : ";
    cin >> quan[c];
    c++;
    cout << "\nDo You Want To Order Another Product ? (y/n)";
    cin >> ch;
  } while (ch == 'y' || ch == 'Y');
  cout << "\n\nThank You For Placing The Order";
  getchar();
  system("clear");
  cout << "\n\n********************************INVOICE************************\n";
  cout << "\nPr No.\tPr Name\t\tQuantity \tPrice \tAmount \t\tAmount after discount\n";
  for (int o=0; o<c;o++){
    string sql;
    sql = "SELECT * from products WHERE id =" +to_string(order_arr[o]);
    nontransaction N(C);
    result R(N.exec(sql));
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        amt =  c[2].as < float > () * quan[o];
        damt = amt - (amt * c[3].as < float > ()/ 100);
        cout << "\n" << order_arr[o] << "\t" << c[1].as < string > () << "\t\t" << quan[o] << "\t\t" << c[2].as < float > () << "\t" << amt << "\t\t" << damt;
        total += damt;
      }
    }
     cout << "\n\n\t\t\t\t\t\t\t\t\t\tTOTAL = " << "₹"<<total<<"\n\n";
     time_t t = time(0);   // get time now
    tm* now = localtime(&t);
    cout <<"Date: "<<  now->tm_mday<< '/'
         << (now->tm_mon + 1) << '/'
         << (now->tm_year + 1900)<< " "<< (now -> tm_hour)<<":"<<(now->tm_min)<<":"<<(now->tm_sec);
    cout<<"\t\t\t\t\t\t\t\t\t\tstamp\n\n";

    getchar();
  
}

//***************************************************************
//      INTRODUCTION FUNCTION
//****************************************************************

void intro() {
  system("clear");
  // cout<<"\n\n\n\n\t\t\t\t\t\t\t";
  //  gotoxy(31,11);
  // cout<<"\n-----------------------------------------------------\n";
  //  gotoxy(31,11);
  cout << "\t\t\t\t\t\t\tSUPER MARKET ";
  //  gotoxy(35,14);
  cout << "BILLING ";
  //  gotoxy(35,17);
  cout << "PROJECT";
  // cout << "\033[1;31mbold red text\033[0m\n";
  cout << "\n\n\t\t\t\t\t\tMADE BY : \033[1;31mHarshit Maheshwari\033[0m and \033[1;31mMohd Arshul\033[0m";
  cout << "\n\n";
  getchar();
  // cout<<"----------------------------------------------------";
  cout << "\n\n";
  getchar();

}

//***************************************************************
//      ADMINSTRATOR MENU FUNCTION
//****************************************************************
void admin_menu() {
  system("clear");
  char ch2;
  cout<<"\n";
  cout<<"\n*****************************************";
  cout << "\n\t\033[1;32mADMIN MENU\033[0m";
  cout<<"\n*****************************************";
  cout << "\n\n\t\033[1;34m1.CREATE PRODUCT\033[0m\n";
  cout << "\n\n\t\033[1;34m2.DISPLAY ALL PRODUCTS\033[0m\n";
  cout << "\n\n\t\033[1;34m3.QUERY\033[0m\n";
  cout << "\n\n\t\033[1;34m4.MODIFY PRODUCT\033[0m\n";
  cout << "\n\n\t\033[1;34m5.DELETE PRODUCT\033[0m\n";
  cout << "\n\n\t\033[1;34m6.VIEW PRODUCT MENU\033[0m\n";
  cout << "\n\n\t\033[1;34m7.BACK TO MAIN MENU\033[0m\n";
  cout << "\n\n\t\033[1;34mPlease Enter Your Choice (1-7) \033[0m\n";
  ch2 = getchar();
  system("clear");
  switch (ch2) {
  case '1':
    system("clear");
    write_product();
    break;
  case '2':
    display_all();
    break;
  case '3':
    int num;
    system("clear");
    cout << "\n\n\tPlease Enter The Product Number : ";
    cin >> num;
    display_sp(num);
    break;
  case '4':
    modify_product();
    break;
  case '5':
    delete_product();
    break;
  case '6':
  menu();
  getchar();
  case '7':
    break;
  default:
    cout << "\a";
    admin_menu();
  }
}

//***************************************************************
//      THE MAIN FUNCTION OF PROGRAM
//****************************************************************

// function to initialize database

int main() {
  init_database();
  create_tables();
  char ch;
  int res;
  intro();
  do {
    system("clear");
    cout<<"\n\n\n";
    cout<<"************************************************************\n";
    cout << "\t\t\033[1;32mMAIN MENU\033[0m\n";
    cout<<"************************************************************\n";
    cout << "\n\n\t\033[1;36m01. CUSTOMER\033[0m\n";
    cout << "\n\n\t\033[1;36m02. ADMINISTRATOR\033[0m\n";
    cout << "\n\n\t\033[1;31m03. EXIT\033[0m\n";
    cout << "\n\n\tPlease Select Your Option (1-3)\n";
    ch = getchar();
    switch (ch) {
    case '1':
      system("clear");
      place_order();
      getchar();
      break;
    case '2':
      res = auth();

      if(res){
        system("clear");
        admin_menu();
      }
      break;
      //   case '3':exit(0);
    default:
      cout << "\a";
    }
  } while (ch != '3');
  return 0;
}

//***************************************************************
//          END OF PROJECT
//*************************************************************** .
