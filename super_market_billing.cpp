//***************************************************************
//                   HEADER FILE USED IN PROJECT
//****************************************************************

#include<termios.h>
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
#include<pqxx/pqxx> 

using namespace std;
using namespace pqxx;


//***************************************************************
//                   CLASS USED IN PROJECT
//****************************************************************
class product
{
 int pno;
char name[50] ;
 float price,qty,tax,dis;
 public:
	void create_product()
	{
	 cout<<"\nPlease Enter The Product No. of The Product ";
	 cin>>pno;
	 cout<<"\n\nPlease Enter The Name of The Product ";
	 cin>>name;
	 cout<<"\nPlease Enter The Price of The Product ";
	 cin>>price;
	 cout<<"\nPlease Enter The Discount (%) ";
	 cin>>dis;
	 }

	void show_product()
	{
	 cout<<"\nThe Product No. of The Product : "<<pno;
	 cout<<"\nThe Name of The Product : "<<name;
	 
	 cout<<"\nThe Price of The Product : "<<price;
	 cout<<"\nDiscount : "<<dis;
	 }

  int  retpno()
  {return pno;}

  float retprice()
  {return price;}

  char* retname()
  {return name;}

  int retdis()
  {return dis;}

};         //class ends here



//***************************************************************
//    	global declarations
//****************************************************************
connection C("dbname = dbms user = harshit password = aaaa \
      hostaddr = 127.0.0.1 port = 5432");
fstream fp;
product pr;


//***************************************************************
//    	function to write in file
//****************************************************************

int write_product()
   {
    string sql;
    pr.create_product();


        // sql = "INSERT INTO product (id, name, price, discount) "  \
        //  "VALUES (2, 'Paul', 20, 10); " ;

 sql = "INSERT INTO products (id, name, price, discount) "  \
         "VALUES (" + to_string(pr.retpno()) + ",'" + to_string(pr.retname()) +"',"
         +to_string(pr.retprice())+ ","+to_string(pr.retdis()) +");" ;

      // std::cout<sql;
      // getchar();
      /* Create a transactional object. */


            work W(C);
   try {
        W.exec( sql );
      W.commit();
      cout << "Records created successfully" << endl;
    cout<<"\n\nThe Product Has Been Created ";
    //  cout << "Table created successfully" << endl;
      return 1;
      }

      catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 0;
}
          
   }



//***************************************************************
//    	function to read all records from file
//****************************************************************

int display_all()
{
    system("clear");
    string sql;
    cout<<"\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n";
    getchar();
    try {

      /* Create SQL statement */
      sql = "SELECT * from products";

      /* Create a non-transactional object. */
      nontransaction N(C);
      
      /* Execute SQL query */
      result R( N.exec( sql ));
      
      /* List down all the records */
      for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
         cout << "PRODUCT_ID = " << c[0].as<int>() << " ";
         cout << "PRODUCT_NAMEN = " << c[1].as<string>() << " ";
         cout << "PRICE = " << c[2].as<float>() << " ";
         cout << "DISCOUNT = " << c[3].as<float>() <<"%"<<endl;
         cout<<"====================================================\n";

      }
      getchar();
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;

      getchar();
      return 1;
   }

   return 0;
}



//***************************************************************
//    	function to read specific record from file
//****************************************************************


void display_sp(int n)
{
    int flag=0;
    fp.open("Shop.dat",ios::in);
    while(fp.read((char*)&pr,sizeof(product)))
	{
	 if(pr.retpno()==n)
		{
		 system("clear");
		 pr.show_product();
		 flag=1;
		}
	}
    fp.close();
if(flag==0)
 cout<<"\n\nrecord not exist";
    getchar();
}


//***************************************************************
//    	function to modify record of file
//****************************************************************




void modify_product()
{
    int no,found=0;
    system("clear");
    cout<<"\n\n\tTo Modify ";
    cout<<"\n\n\tPlease Enter The Product No. of The Product";
    cin>>no;
    fp.open("Shop.dat",ios::in|ios::out);
    while(fp.read((char*)&pr,sizeof(product)) && found==0)
	   {
	    if(pr.retpno()==no)
		   {
		    pr.show_product();
		    cout<<"\nPlease Enter The New Details of Product"<<endl;
		    pr.create_product();
		    int pos=-1*5;
		    fp.seekp(pos,ios::cur);
		    fp.write((char*)&pr,sizeof(product));
		    cout<<"\n\n\t Record Updated";
		    found=1;
		   }
	     }
    fp.close();
    if(found==0)
    cout<<"\n\n Record Not Found ";
    getchar();
}


//***************************************************************
//    	function to delete record of file
//****************************************************************


void delete_product()
   {
    int no;
    system("clear");
    cout<<"\n\n\n\tDelete Record";
    cout<<"\n\nPlease Enter The product no. of The Product You Want To Delete";
    cin>>no;
    fp.open("Shop.dat",ios::in|ios::out);
    fstream fp2;
    fp2.open("Temp.dat",ios::out);
    fp.seekg(0,ios::beg);
    while(fp.read((char*)&pr,sizeof(product)))
	{
	 if(pr.retpno()!=no)
		{
		 fp2.write((char*)&pr,sizeof(product));
		 }
	 }
    fp2.close();
    fp.close();
    remove("Shop.dat");
    rename("Temp.dat","Shop.dat");
    cout<<"\n\n\tRecord Deleted ..";
    getchar();
    }


//***************************************************************
//    	function to display all products price list
//****************************************************************

    void menu()
    {
     system("clear");
     fp.open("Shop.dat",ios::in);
     if(!fp)
     {
       cout<<"ERROR!!! FILE COULD NOT BE OPEN\n\n\n Go To Admin Menu to create File";
       cout<<"\n\n\n Program is closing ....";
       getchar();
       exit(0);
     }

     cout<<"\n\n\t\tProduct MENU\n\n";
	  cout<<"====================================================\n";
	  cout<<"P.NO.\t\tNAME\t\tPRICE\n";
	  cout<<"====================================================\n";

      while(fp.read((char*)&pr,sizeof(product)))
	 {
	   cout<<pr.retpno()<<"\t\t"<<pr.retname()<<"\t\t"<<pr.retprice()<<endl;
	}
     fp.close();
}



//***************************************************************
//    	function to place order and generating bill for Products
//****************************************************************

   void place_order()
   {
    int  order_arr[50],quan[50],c=0;
    float amt,damt,total=0;
    char ch='Y';
    menu();
    cout<<"\n============================";
    cout<<"\n    PLACE YOUR ORDER";
    cout<<"\n============================\n";
    do{
	 cout<<"\n\nEnter The Product No. Of The Product : ";
	 cin>>order_arr[c];
	 cout<<"\nQuantity in number : ";
	 cin>>quan[c];
	 c++;
	 cout<<"\nDo You Want To Order Another Product ? (y/n)";
	 cin>>ch;
    }while(ch=='y' ||ch=='Y');
    cout<<"\n\nThank You For Placing The Order";getchar();system("clear");
      cout<<"\n\n********************************INVOICE************************\n";
      cout<<"\nPr No.\tPr Name\tQuantity \tPrice \tAmount \tAmount after discount\n";
      for(int x=0;x<=c;x++)
	{
		 fp.open("Shop.dat",ios::in);
		 fp.read((char*)&pr,sizeof(product));
		  while(!fp.eof())
			{
			if(pr.retpno()==order_arr[x])
				{
				 amt=pr.retprice()*quan[x];
				 damt=amt-(amt*pr.retdis()/100);
				 cout<<"\n"<<order_arr[x]<<"\t"<<pr.retname()<<"\t"<<quan[x]<<"\t\t"<<pr.retprice()<<"\t"<<amt<<"\t\t"<<damt;
				 total+=damt;
				}
			fp.read((char*)&pr,sizeof(product));
			}

		 fp.close();
	 }
       cout<<"\n\n\t\t\t\t\tTOTAL = "<<total;
    getchar();
    }

//***************************************************************
//    	INTRODUCTION FUNCTION
//****************************************************************

void intro()
{
  system("clear");
//  gotoxy(31,11);
 cout<<"SUPER MARKET";
//  gotoxy(35,14);
 cout<<"BILLING";
//  gotoxy(35,17);
 cout<<"PROJECT";
 cout<<"\n\nMADE BY : Harshit Maheshwari and Mohd Arshul";
 cout<<"\n\n";
 getchar();

}




//***************************************************************
//    	ADMINSTRATOR MENU FUNCTION
//****************************************************************
void admin_menu()
{
  system("clear");
  char ch2;
  cout<<"\n\n\n\tADMIN MENU";
  cout<<"\n\n\t1.CREATE PRODUCT";
  cout<<"\n\n\t2.DISPLAY ALL PRODUCTS";
  cout<<"\n\n\t3.QUERY ";
  cout<<"\n\n\t4.MODIFY PRODUCT";
  cout<<"\n\n\t5.DELETE PRODUCT";
  cout<<"\n\n\t6.VIEW PRODUCT MENU";
  cout<<"\n\n\t7.BACK TO MAIN MENU";
  cout<<"\n\n\tPlease Enter Your Choice (1-7) ";
  ch2=getchar();
  switch(ch2)
  {
    case '1': system("clear");
	      write_product();
	      break;
    case '2': 
    display_all();break;
    case '3':
	       int num;
	       system("clear");
	       cout<<"\n\n\tPlease Enter The Product No. ";
	       cin>>num;
	       display_sp(num);
	       break;
      case '4': modify_product();break;
      case '5': delete_product();break;
      case '6': menu();
		getchar();
      case '7': break;
      default:cout<<"\a";admin_menu();
   }
}


//***************************************************************
//    	THE MAIN FUNCTION OF PROGRAM
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
   } catch (const std::exception &e) {
    cout<<"in exception";
      cerr << e.what() << std::endl;
      return 1;
   }
}

int create_tables()
{
  string sql;
  sql = "CREATE TABLE PRODUCTS("  \
      "ID INT PRIMARY KEY  NOT NULL," \
      "NAME TEXT NOT NULL," \
      "price double precision NOT NULL," \
      "discount double precision );";

   work W(C);
   try {
       W.exec( sql );
      W.commit();
    //  cout << "Table created successfully" << endl;
      return 1;
      }

      catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 0;
}
}

int main()
{
  init_database();
  create_tables();
  char ch;
  intro();
  do
    {
	  system("clear");
	  cout<<"\n\n\n\tMAIN MENU";
	  cout<<"\n\n\t01. CUSTOMER";
	  cout<<"\n\n\t02. ADMINISTRATOR";
	  cout<<"\n\n\t03. EXIT";
	  cout<<"\n\n\tPlease Select Your Option (1-3) ";
      ch=getchar();
	  switch(ch)
	  {
		 case '1': system("clear");
			   place_order();
			   getchar();
			   break;
		  case '2': admin_menu();
			    break;
		//   case '3':exit(0);
		  default :cout<<"\a";
	}
}while(ch!='3');
return 0;
}

//***************************************************************
//    			END OF PROJECT
//***************************************************************
