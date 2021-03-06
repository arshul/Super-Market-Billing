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
      cout << "\nPlease Enter The Product No. of The Product ";
      cin >> pno;
      cout << "\n\nPlease Enter The Name of The Product ";
      cin >> name;
      cout << "\nPlease Enter The Price of The Product ";
      cin >> price;
      cout << "\nPlease Enter The Discount (%) ";
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
fstream fp;
product pr;

//***************************************************************
//      function to write in file
//****************************************************************

int write_product() {
  string sql;
  pr.create_product();

  // sql = "INSERT INTO product (id, name, price, discount) "  \
  //  "VALUES (2, 'Paul', 20, 10); " ;

  sql = "INSERT INTO products (id, name, price, discount) "\
  "VALUES (" + to_string(pr.retpno()) + ",'" + to_string(pr.retname()) + "'," + to_string(pr.retprice()) + "," + to_string(pr.retdis()) + ");";

  // std::cout<sql;
  // getchar();
  /* Create a transactional object. */

  work W(C);
  try {
    W.exec(sql);
    W.commit();
    cout << "Records created successfully" << endl;
    cout << "\n\nThe Product Has Been Created ";
    //  cout << "Table created successfully" << endl;
    return 1;
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 0;
  }

}

//***************************************************************
//      function to read all records from file
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
      cout << "PRODUCT_ID = " << c[0].as < int > () << " ";
      cout << "PRODUCT_NAMEN = " << c[1].as < string > () << " ";
      cout << "PRICE = " << c[2].as < float > () << " ";
      cout << "DISCOUNT = " << c[3].as < float > () << "%" << endl;
      cout << "====================================================\n";

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
//      function to read specific record from file
//****************************************************************

void display_sp(int n) {
  try {
  string sql;
  int flag = 0;
  sql = "SELECT * from products WHERE id =" +to_string(n);
  nontransaction N(C);
  result R(N.exec(sql));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      cout << "PRODUCT_ID = " << c[0].as < int > () << " ";
      cout << "PRODUCT_NAMEN = " << c[1].as < string > () << " ";
      cout << "PRICE = " << c[2].as < float > () << " ";
      cout << "DISCOUNT = " << c[3].as < float > () << "%" << endl;
      cout << "====================================================\n";
      flag = 1;
    }
    getchar();
  } catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    getchar();
  }
  // fp.open("Shop.dat", ios:: in );
  // while (fp.read((char * ) & pr, sizeof(product))) {
  //   if (pr.retpno() == n) {
  //     system("clear");
  //     pr.show_product();
  //     flag = 1;
  //   }
  // }
  // fp.close();
  // if (flag == 0)
  //   cout << "\n\nrecord not exist";
  getchar();
  }

//***************************************************************
//      function to modify record of file
//****************************************************************

void modify_product() {
  int no, found = 0;
  system("clear");
  cout << "\n\n\tTo Modify ";
  cout << "\n\n\tPlease Enter The Product No. of The Product";
  cin >> no;
  fp.open("Shop.dat", ios:: in | ios::out);
  while (fp.read((char * ) & pr, sizeof(product)) && found == 0) {
    if (pr.retpno() == no) {
      pr.show_product();
      cout << "\nPlease Enter The New Details of Product" << endl;
      pr.create_product();
      int pos = -1 * 5;
      fp.seekp(pos, ios::cur);
      fp.write((char * ) & pr, sizeof(product));
      cout << "\n\n\t Record Updated";
      found = 1;
    }
  }
  fp.close();
  if (found == 0)
    cout << "\n\n Record Not Found ";
  getchar();
}

//***************************************************************
//      function to delete record of file
//****************************************************************

int delete_product() {
  string sql;
  int no;
  system("clear");
  cout << "\n\n\n\tDelete Record";
  cout << "\n\nPlease Enter The product no. of The Product You Want To Delete";
  cin >> no;
  sql = "DELETE from products where ID = " + to_string(no) + ";";
  work W(C);
  try {
    W.exec(sql);
    W.commit();
    cout << "Records deleted successfully" << endl;
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
  sql = "SELECT * from products";
  nontransaction N(C);
  result R(N.exec(sql));
  cout << "\n\n\t\tProduct MENU\n\n";
  cout << "====================================================\n";
  cout << "P.NO.\t\tNAME\t\tPRICE\n";
  cout << "====================================================\n";

  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << c[0] << "\t\t" << c[1] << "\t\t" << c[2] << endl;
  }
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
    cout<<"value of c"<<c;
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
     cout << "\n\n\t\t\t\t\tTOTAL = " << total;
    getchar();
  
  // cout << "\n\n********************************INVOICE************************\n";
  // cout << "\nPr No.\tPr Name\tQuantity \tPrice \tAmount \tAmount after discount\n";

  // for (int x = 0; x < c; x++) {
  //   fp.open("Shop.dat", ios:: in );
  //   fp.read((char * ) & pr, sizeof(product));
  //   while (!fp.eof()) {
  //     if (pr.retpno() == order_arr[x]) {
  //       amt = pr.retprice() * quan[x];
  //       damt = amt - (amt * pr.retdis() / 100);
  //       cout << "\n" << order_arr[x] << "\t" << pr.retname() << "\t" << quan[x] << "\t\t" << pr.retprice() << "\t" << amt << "\t\t" << damt;
  //       total += damt;
  //     }
  //     fp.read((char * ) & pr, sizeof(product));
  //   }

  //   fp.close();
  // }
  // cout << "\n\n\t\t\t\t\tTOTAL = " << total;
  // getchar();
}

//***************************************************************
//      INTRODUCTION FUNCTION
//****************************************************************

void intro() {
  system("clear");
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

}

//***************************************************************
//      ADMINSTRATOR MENU FUNCTION
//****************************************************************
void admin_menu() {
  system("clear");
  char ch2;
  cout << "\n\n\n\tADMIN MENU";
  cout << "\n\n\t\033[1;34m1.CREATE PRODUCT\033[0m";
  cout << "\n\n\t\033[1;34m2.DISPLAY ALL PRODUCTS\033[0m";
  cout << "\n\n\t\033[1;34m3.QUERY\033[0m ";
  cout << "\n\n\t\033[1;34m4.MODIFY PRODUCT\033[0m";
  cout << "\n\n\t\033[1;34m5.DELETE PRODUCT\033[0m";
  cout << "\n\n\t\033[1;34m6.VIEW PRODUCT MENU\033[0m";
  cout << "\n\n\t\033[1;34m7.BACK TO MAIN MENU\033[0m";
  cout << "\n\n\t\033[1;34mPlease Enter Your Choice (1-7) \033[0m";
  ch2 = getchar();
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
    cout << "\n\n\tPlease Enter The Product No. ";
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

int main() {
  init_database();
  create_tables();
  char ch;
  intro();
  do {
    system("clear");
    cout << "\n\n\n\t\033[1;32mMAIN MENU\033[0m";
    cout << "\n\n\t\033[1;36m01. CUSTOMER\033[0m";
    cout << "\n\n\t\033[1;36m02. ADMINISTRATOR\033[0m";
    cout << "\n\n\t\033[1;31m03. EXIT\033[0m";
    cout << "\n\n\tPlease Select Your Option (1-3) ";
    ch = getchar();
    switch (ch) {
    case '1':
      system("clear");
      place_order();
      getchar();
      break;
    case '2':
      admin_menu();
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
//***************************************************************
